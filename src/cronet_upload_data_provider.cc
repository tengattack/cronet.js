
#include "cronet_upload_data_provider.h"

#include <string>

#include "addon_node.h"
#include "cronet_buffer.h"
#include "cronet_executor.h"
#include "cronet_upload_data_sink.h"
#include "cronet_util.h"

CronetUploadDataProvider::CronetUploadDataProvider()
    : ptr_(_Cronet_UploadDataProvider_CreateWith(
          CronetUploadDataProvider::GetLength,
          CronetUploadDataProvider::Read,
          CronetUploadDataProvider::Rewind,
          CronetUploadDataProvider::Close
    ))
    , executor_(nullptr)
    , get_length_ref_(nullptr)
    , read_ref_(nullptr)
    , rewind_ref_(nullptr)
    , close_ref_(nullptr) {
  _Cronet_UploadDataProvider_SetClientContext(ptr_, this);
}

CronetUploadDataProvider::~CronetUploadDataProvider() {
  TRACE("~CronetUploadDataProvider()\n");
  _Cronet_UploadDataProvider_Destroy(ptr_);
  ptr_ = nullptr;
  SetExecutor(nullptr);
  if (get_length_ref_) {
    napi_delete_reference(env_, get_length_ref_);
  }
  if (read_ref_) {
    napi_delete_reference(env_, read_ref_);
  }
  if (rewind_ref_) {
    napi_delete_reference(env_, rewind_ref_);
  }
  if (close_ref_) {
    napi_delete_reference(env_, close_ref_);
  }
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetUploadDataProvider)

napi_value CronetUploadDataProvider::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    DECLARE_NAPI_MAGIC_METHOD("getLength", get_length),
    DECLARE_NAPI_MAGIC_METHOD("read", read),
    DECLARE_NAPI_MAGIC_METHOD("rewind", rewind),
    DECLARE_NAPI_MAGIC_METHOD("close", close),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetUploadDataProvider", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));

  ref = RegisterClass(env, "CronetUploadDataProvider", cons, exports);
  return exports;
}

napi_value CronetUploadDataProvider::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetUploadDataProvider(...)\n");
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

    CronetUploadDataProvider* obj = new CronetUploadDataProvider();

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetUploadDataProvider::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetUploadDataProvider(...)\n");
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

void CronetUploadDataProvider::SetExecutor(CronetExecutor* executor) {
  if (executor_) {
    // executor_->ReleaseRef();
  }
  executor_ = executor;
  if (executor_) {
    // FIXME: why crash?
    // executor_->AddRef();
  }
}

DECLARE_CLAZZ_FUNC_GETTER_SETTER(UploadDataProvider, get_length)
DECLARE_CLAZZ_FUNC_GETTER_SETTER(UploadDataProvider, read)
DECLARE_CLAZZ_FUNC_GETTER_SETTER(UploadDataProvider, rewind)
DECLARE_CLAZZ_FUNC_GETTER_SETTER(UploadDataProvider, close)

int64_t CronetUploadDataProvider::GetLength(napi_env env) {
  if (get_length_ref_) {
    napi_status status;
    napi_value cb;
    DCHECK(napi_get_reference_value(env, get_length_ref_, &cb));

    napi_value jsthis;
    DCHECK(napi_get_reference_value(env, wrapper_, &jsthis));

    napi_value result;
    DCHECK(napi_call_function(env, jsthis, cb, 0, nullptr, &result));

    napi_valuetype valuetype;
    DCHECK(napi_typeof(env, result, &valuetype));
    if (valuetype != napi_number) {
      napi_throw_error(env, nullptr, "CronetUploadDataProvider.getLength() should returns a number");
      return 0;
    }

    /**
     * If this is a non-chunked upload, returns the length of the upload. Must
     * always return -1 if this is a chunked upload.
     */
    int64_t length;
    DCHECK(napi_get_value_int64(env, result, &length));
    return length;
  }
  return 0;
}

void CronetUploadDataProvider::Read(napi_env env,
    Cronet_UploadDataSinkPtr upload_data_sink,
    Cronet_BufferPtr buffer) {
  if (read_ref_) {
    napi_status status;
    napi_value cb;
    DCHECK(napi_get_reference_value(env, read_ref_, &cb));

    napi_value jsthis;
    DCHECK(napi_get_reference_value(env, wrapper_, &jsthis));
    size_t argc = 2;
    napi_value argv[argc];
    argv[0] = CronetUploadDataSink::WrapUnowned(env, upload_data_sink);
    argv[1] = CronetBuffer::WrapUnowned(env, buffer);
    DCHECK(napi_call_function(env, jsthis, cb, argc, argv, nullptr));
  }
}

void CronetUploadDataProvider::Rewind(napi_env env,
    Cronet_UploadDataSinkPtr upload_data_sink) {
  if (rewind_ref_) {
    napi_status status;
    napi_value cb;
    DCHECK(napi_get_reference_value(env, rewind_ref_, &cb));

    napi_value jsthis;
    DCHECK(napi_get_reference_value(env, wrapper_, &jsthis));
    size_t argc = 1;
    napi_value argv[argc];
    argv[0] = CronetUploadDataSink::WrapUnowned(env, upload_data_sink);
    DCHECK(napi_call_function(env, jsthis, cb, argc, argv, nullptr));
  }
}

void CronetUploadDataProvider::Close(napi_env env) {
  if (close_ref_) {
    napi_status status;
    napi_value cb;
    DCHECK(napi_get_reference_value(env, close_ref_, &cb));

    napi_value jsthis;
    DCHECK(napi_get_reference_value(env, wrapper_, &jsthis));
    DCHECK(napi_call_function(env, jsthis, cb, 0, nullptr, nullptr));
  }
}

/* static */
CronetUploadDataProvider* CronetUploadDataProvider::GetThis(
    Cronet_UploadDataProviderPtr self) {
  return static_cast<CronetUploadDataProvider*>(
      _Cronet_UploadDataProvider_GetClientContext(self));
}

/* sync */
int64_t CronetUploadDataProvider::GetLength(Cronet_UploadDataProviderPtr self) {
  TRACE("GetLength called.\n");
  auto* that = GetThis(self);
  // This call happened in `CronetUrlRequest.initWithParams(...)`, which in the
  // js main thread, so we call the js function directly.
  int64_t result = that->GetLength(that->env_);
  TRACE("GetLength done.\n");
  return result;
}

void CronetUploadDataProvider::Read(Cronet_UploadDataProviderPtr self,
                                    Cronet_UploadDataSinkPtr upload_data_sink,
                                    Cronet_BufferPtr buffer) {
  TRACE("Read called.\n");
  auto* that = GetThis(self);
  that->executor_->CallInJs([&](napi_env env) {
    that->Read(env, upload_data_sink, buffer);
  });
}

void CronetUploadDataProvider::Rewind(Cronet_UploadDataProviderPtr self,
                                      Cronet_UploadDataSinkPtr upload_data_sink) {
  TRACE("Rewind called.\n");
  auto* that = GetThis(self);
  that->executor_->CallInJs([&](napi_env env) {
    that->Rewind(env, upload_data_sink);
  });
}

void CronetUploadDataProvider::Close(Cronet_UploadDataProviderPtr self) {
  TRACE("Close called.\n");
  auto* that = GetThis(self);
  that->executor_->CallInJs([&](napi_env env) {
    that->Close(env);
  });
}
