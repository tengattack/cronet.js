#ifndef _CRONET_EXECUTOR_H_
#define _CRONET_EXECUTOR_H_

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include <cronet_c.h>
#include <js_native_api_types.h>
#include <node_api_types.h>

#include "napi_class.h"

class CronetExecutor : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER()

 public:
  Cronet_ExecutorPtr ptr() { return ptr_; }

  void CallInJs(std::function<void(napi_env)> cb);

 private:
  explicit CronetExecutor();
  ~CronetExecutor();

  static napi_value Start(napi_env env, napi_callback_info info);
  static napi_value Shutdown(napi_env env, napi_callback_info info);

  // async work
  static void CallJs(napi_env env, napi_value js_cb, void* context, void* data);

  void WorkComplete();

  napi_threadsafe_function tsfn_;

  std::thread native_thread_;

  Cronet_ExecutorPtr ptr_;
  bool started_;

 protected:
  // Runs tasks in |task_queue_| until |stop_thread_loop_| is set to true.
  void RunTasksInQueue();
  // Adds |runnable| to |task_queue_| to execute on |executor_thread_|.
  void Execute(Cronet_RunnablePtr runnable);
  void Shutdown();

  // Implementation of Cronet_Executor methods.
  static void Execute(Cronet_ExecutorPtr self, Cronet_RunnablePtr runnable);

  // Synchronise access to |task_queue_| and |stop_thread_loop_|;
  std::mutex lock_;
  // Tasks to run.
  std::queue<Cronet_RunnablePtr> task_queue_;
  // Notified if task is added to |task_queue_| or |stop_thread_loop_| is set.
  std::condition_variable task_available_;
  // Set to true to stop running tasks.
  bool stop_thread_loop_;
};

#endif // _CRONET_EXECUTOR_H_
