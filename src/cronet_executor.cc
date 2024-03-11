
#include "cronet_executor.h"

#include "addon_node.h"
#include "cronet_util.h"

typedef struct {
  std::function<void(napi_env)> cb_;
  bool called_;
  std::mutex lock_;
  std::condition_variable cv_;
} CallJsContext;

CronetExecutor::CronetExecutor()
    : ptr_(Cronet_Executor_CreateWith(CronetExecutor::Execute))
    , started_(false)
    , stop_thread_loop_(false)
    , work_(nullptr)
    , tsfn_(nullptr) {
  Cronet_Executor_SetClientContext(ptr_, this);
}

CronetExecutor::~CronetExecutor() {
  TRACE("~CronetExecutor()\n");
  Cronet_Executor_Destroy(ptr_);
  ptr_ = nullptr;
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetExecutor)

napi_value CronetExecutor::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    DECLARE_NAPI_METHOD("start", Start),
    DECLARE_NAPI_METHOD("shutdown", Shutdown),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetExecutor", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));

  ref = RegisterClass(env, "CronetExecutor", cons, exports);
  return exports;
}

napi_value CronetExecutor::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetExecutor(...)\n");
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));

    // napi_valuetype valuetype;
    // status = napi_typeof(env, args[0], &valuetype);
    // assert(status == napi_ok);

    // if (valuetype != napi_undefined) {
    //   status = napi_get_value_double(env, args[0], &value);
    //   assert(status == napi_ok);
    // }

    CronetExecutor* obj = new CronetExecutor();

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetExecutor::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetExecutor(...)\n");
    size_t argc_ = 1;
    napi_value args[1];
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc_, args, nullptr, nullptr));

    const size_t argc = 1;
    napi_value argv[argc] = {args[0]};

    napi_value instance;
    NODE_API_CALL(env, napi_new_instance(env, Constructor(env), argc, argv, &instance));

    return instance;
  }
}

napi_value CronetExecutor::Start(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetExecutor* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  if (obj->started_) {
    napi_throw_error(env, nullptr, "CronetExecutor already started");
    return nullptr;
  }

  napi_value work_name;
  // Create a string to describe this asynchronous operation.
  DCHECK(napi_create_string_utf8(env,
                                 "CronetExecutor AsyncWork",
                                 NAPI_AUTO_LENGTH,
                                 &work_name));

  // Convert the callback retrieved from JavaScript into a thread-safe function
  // which we can call from a worker thread.
  DCHECK(napi_create_threadsafe_function(env,
                                         NULL,
                                         NULL,
                                         work_name,
                                         0,
                                         1,
                                         NULL,
                                         NULL,
                                         obj,
                                         CronetExecutor::CallJs,
                                         &(obj->tsfn_)));

  // Create an async work item, passing in the addon data, which will give the
  // worker thread access to the above-created thread-safe function.
  DCHECK(napi_create_async_work(env,
                                NULL,
                                work_name,
                                CronetExecutor::ExecuteWork,
                                CronetExecutor::WorkComplete,
                                obj,
                                &(obj->work_)));

  // Queue the work item for execution.
  DCHECK(napi_queue_async_work(env, obj->work_));
  obj->AddRef();
  obj->started_ = true;
  return nullptr;
}

napi_value CronetExecutor::Shutdown(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetExecutor* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  if (!obj->started_) {
    napi_throw_error(env, nullptr, "CronetExecutor not started");
    return nullptr;
  }

  obj->Shutdown();
  return nullptr;
}

void CronetExecutor::CallInJs(std::function<void(napi_env)> cb) {
  napi_status status;
  CallJsContext* ctx = new CallJsContext;
  ctx->cb_ = cb;
  ctx->called_ = false;

  // Initiate the call into JavaScript. The call into JavaScript will not
  // have happened when this function returns, but it will be queued.
  DCHECK(napi_call_threadsafe_function(tsfn_,
                                       ctx,
                                       napi_tsfn_blocking));

  std::unique_lock<std::mutex> lock(lock_);
  while (!ctx->called_)
    ctx->cv_.wait(lock);
  delete ctx;
}

// This function is responsible for converting data coming in from the worker
// thread to napi_value items that can be passed into JavaScript, and for
// calling the JavaScript function.
void CronetExecutor::CallJs(napi_env env, napi_value js_cb, void* context, void* data) {
  CallJsContext* ctx = (CallJsContext*)data;
  ctx->cb_(env);
  {
    std::lock_guard<std::mutex> lock(ctx->lock_);
    ctx->called_ = true;
  }
  ctx->cv_.notify_one();
}

// This function runs on a worker thread. It has no access to the JavaScript
// environment except through the thread-safe function.
void CronetExecutor::ExecuteWork(napi_env env, void* data) {
  CronetExecutor* executor = (CronetExecutor*)data;
  napi_status status;

  // We bracket the use of the thread-safe function by this thread by a call to
  // napi_acquire_threadsafe_function() here, and by a call to
  // napi_release_threadsafe_function() immediately prior to thread exit.
  DCHECK(napi_acquire_threadsafe_function(executor->tsfn_));

  executor->RunTasksInQueue();

  // Indicate that this thread will make no further use of the thread-safe function.
  DCHECK(napi_release_threadsafe_function(executor->tsfn_,
                                          napi_tsfn_release));
}

// This function runs on the main thread after `ExecuteWork` exits.
void CronetExecutor::WorkComplete(napi_env env, napi_status /* status */, void* data) {
  CronetExecutor* executor = (CronetExecutor*)data;
  napi_status status;

  // Clean up the thread-safe function and the work item associated with this
  // run.
  DCHECK(napi_release_threadsafe_function(executor->tsfn_,
                                          napi_tsfn_release));
  DCHECK(napi_delete_async_work(env, executor->work_));

  // Set both values to NULL so JavaScript can order a new run of the thread.
  executor->work_ = nullptr;
  executor->tsfn_ = nullptr;
  executor->ReleaseRef();
  executor->started_ = false;
}

void CronetExecutor::RunTasksInQueue() {
  // Process runnables in |task_queue_|.
  while (true) {
    Cronet_RunnablePtr runnable = nullptr;
    {
      // Wait for a task to run or stop signal.
      std::unique_lock<std::mutex> lock(lock_);
      while (task_queue_.empty() && !stop_thread_loop_)
        task_available_.wait(lock);

      if (stop_thread_loop_)
        break;

      if (task_queue_.empty())
        continue;

      runnable = task_queue_.front();
      task_queue_.pop();
    }
    Cronet_Runnable_Run(runnable);
    Cronet_Runnable_Destroy(runnable);
  }
  // Delete remaining tasks.
  std::queue<Cronet_RunnablePtr> tasks_to_destroy;
  {
    std::unique_lock<std::mutex> lock(lock_);
    tasks_to_destroy.swap(task_queue_);
  }
  while (!tasks_to_destroy.empty()) {
    Cronet_Runnable_Destroy(tasks_to_destroy.front());
    tasks_to_destroy.pop();
  }
}

void CronetExecutor::Execute(Cronet_RunnablePtr runnable) {
  {
    std::lock_guard<std::mutex> lock(lock_);
    if (!stop_thread_loop_) {
      task_queue_.push(runnable);
      runnable = nullptr;
    }
  }
  if (runnable) {
    Cronet_Runnable_Destroy(runnable);
  } else {
    task_available_.notify_one();
  }
}

void CronetExecutor::Shutdown() {
  // Break tasks loop.
  {
    std::lock_guard<std::mutex> lock(lock_);
    stop_thread_loop_ = true;
  }
  task_available_.notify_one();
  // TODO: Wait for async work.
}

/* static */
void CronetExecutor::Execute(Cronet_ExecutorPtr self,
                             Cronet_RunnablePtr runnable) {
  auto* executor =
      static_cast<CronetExecutor*>(Cronet_Executor_GetClientContext(self));
  executor->Execute(runnable);
}
