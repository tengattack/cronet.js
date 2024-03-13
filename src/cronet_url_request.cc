
#include "cronet_url_request.h"

#include <string>

#include "addon_node.h"
#include "cronet_buffer.h"
#include "cronet_engine.h"
#include "cronet_executor.h"
#include "cronet_upload_data_provider.h"
#include "cronet_url_request_callback.h"
#include "cronet_url_request_params.h"
#include "cronet_util.h"

CronetUrlRequest::CronetUrlRequest()
    : ptr_(_Cronet_UrlRequest_Create())
    , is_owned_(true)
    , inited_(false)
    , started_(false)
    , callback_(nullptr)
    , upload_data_provider_(nullptr) {
  _Cronet_UrlRequest_SetClientContext(ptr_, this);
}

CronetUrlRequest::CronetUrlRequest(Cronet_UrlRequestPtr ptr)
    : ptr_(ptr)
    , is_owned_(false)
    , inited_(false)
    , started_(false)
    , callback_(nullptr)
    , upload_data_provider_(nullptr) {
}

CronetUrlRequest::~CronetUrlRequest() {
  TRACE("~CronetUrlRequest()\n");
  if (is_owned_) {
    _Cronet_UrlRequest_Destroy(ptr_);
  }
  ptr_ = nullptr;
  SetCallback(nullptr);
  SetUploadDataProvider(nullptr);
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetUrlRequest)

napi_value CronetUrlRequest::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    DECLARE_NAPI_METHOD("initWithParams", InitWithParams),
    DECLARE_NAPI_METHOD("start", Start),
    DECLARE_NAPI_METHOD("cancel", Cancel),
    DECLARE_NAPI_METHOD("followRedirect", FollowRedirect),
    DECLARE_NAPI_METHOD("read", Read),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetUrlRequest", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));

  ref = RegisterClass(env, "CronetUrlRequest", cons, exports);
  return exports;
}

void CronetUrlRequest::SetCallback(CronetUrlRequestCallback *callback) {
  if (callback_) {
    callback_->ReleaseRef();
  }
  callback_ = callback;
  if (callback_) {
    callback_->AddRef();
  }
}

void CronetUrlRequest::SetUploadDataProvider(CronetUploadDataProvider* provider) {
  if (upload_data_provider_) {
    upload_data_provider_->ReleaseRef();
  }
  upload_data_provider_ = provider;
  if (upload_data_provider_) {
    upload_data_provider_->AddRef();
  }
}

void CronetUrlRequest::Done() {
  TRACE("CronetUrlRequest::Done()\n");
  SetCallback(nullptr);
  SetUploadDataProvider(nullptr);
}

napi_value CronetUrlRequest::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetUrlRequest(...)\n");
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));

    Cronet_UrlRequestPtr ptr = nullptr;
    if (argc == 1) {
      napi_valuetype valuetype;
      DCHECK(napi_typeof(env, args[0], &valuetype));

      if (valuetype == napi_external) {
        DCHECK(napi_get_value_external(env, args[0], (void**)&ptr));
      }
    }

    CronetUrlRequest* obj;
    if (ptr) {
      obj = new CronetUrlRequest(ptr);
    } else {
      obj = new CronetUrlRequest();
    }

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetUrlRequest::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetUrlRequest(...)\n");
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

napi_value CronetUrlRequest::WrapUnowned(napi_env env, Cronet_UrlRequestPtr ptr) {
  return WrapInternal(env, ref, ptr);
}

napi_value CronetUrlRequest::InitWithParams(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 5;
  napi_value args[argc];
  napi_valuetype arg_types[] = {napi_object, napi_string, napi_object, napi_object, napi_object};
  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, argc, args, arg_types)) {
    return nullptr;
  }

  CronetUrlRequest* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  if (!obj->is_owned_) {
    napi_throw_error(env, nullptr, "CronetUrlRequest not owned");
    return nullptr;
  }
  if (obj->inited_) {
    napi_throw_error(env, nullptr, "CronetUrlRequest already inited");
    return nullptr;
  }

  bool isinstanceof;
  napi_value cons;

  DCHECK(napi_get_reference_value(env, *CronetEngine::ref, &cons));
  DCHECK(napi_instanceof(env, args[0], cons, &isinstanceof));
  TRACE("isinstanceof CronetEngine: %d\n", isinstanceof ? 1 : 0);
  if (!isinstanceof) {
    CronetUtil::ThrowInvalidArgumentError(env);
    return nullptr;
  }
  CronetEngine *engine;
  DCHECK(napi_unwrap(env, args[0], reinterpret_cast<void**>(&engine)));

  size_t url_str_length, copied;
  DCHECK(napi_get_value_string_utf8(env, args[1], nullptr, 0, &url_str_length));
  std::string url_str(url_str_length, '\0');
  DCHECK(napi_get_value_string_utf8(env, args[1], &url_str[0], url_str.length() + 1, &copied));
  TRACE("url: %s\n", url_str.c_str());

  DCHECK(napi_get_reference_value(env, *CronetUrlRequestParams::ref, &cons));
  DCHECK(napi_instanceof(env, args[2], cons, &isinstanceof));
  TRACE("isinstanceof CronetUrlRequestParams: %d\n", isinstanceof ? 1 : 0);
  if (!isinstanceof) {
    CronetUtil::ThrowInvalidArgumentError(env);
    return nullptr;
  }
  CronetUrlRequestParams *params;
  DCHECK(napi_unwrap(env, args[2], reinterpret_cast<void**>(&params)));

  DCHECK(napi_get_reference_value(env, *CronetUrlRequestCallback::ref, &cons));
  DCHECK(napi_instanceof(env, args[3], cons, &isinstanceof));
  TRACE("isinstanceof CronetUrlRequestCallback: %d\n", isinstanceof ? 1 : 0);
  if (!isinstanceof) {
    CronetUtil::ThrowInvalidArgumentError(env);
    return nullptr;
  }
  CronetUrlRequestCallback *callback;
  DCHECK(napi_unwrap(env, args[3], reinterpret_cast<void**>(&callback)));

  DCHECK(napi_get_reference_value(env, *CronetExecutor::ref, &cons));
  DCHECK(napi_instanceof(env, args[4], cons, &isinstanceof));
  TRACE("isinstanceof CronetExecutor: %d\n", isinstanceof ? 1 : 0);
  if (!isinstanceof) {
    CronetUtil::ThrowInvalidArgumentError(env);
    return nullptr;
  }
  CronetExecutor *executor;
  DCHECK(napi_unwrap(env, args[4], reinterpret_cast<void**>(&executor)));

  if (params->upload_data_provider_ && params->upload_data_provider_executor_) {
    params->upload_data_provider_->SetExecutor(params->upload_data_provider_executor_);
  }
  callback->SetExecutor(executor);

  Cronet_RESULT result = _Cronet_UrlRequest_InitWithParams(obj->ptr_,
      engine->ptr(), url_str.c_str(), params->ptr(),
      callback->ptr(), executor->ptr());
  if (result == Cronet_RESULT_SUCCESS) {
    obj->SetCallback(callback);
    if (params->upload_data_provider_ && params->upload_data_provider_executor_) {
      obj->SetUploadDataProvider(params->upload_data_provider_);
    }
    obj->inited_ = true;
  } else {
    callback->SetExecutor(nullptr);
    if (params->upload_data_provider_ && params->upload_data_provider_executor_) {
      params->upload_data_provider_->SetExecutor(nullptr);
    }
    CronetUtil::ThrowCronetResultError(env, result);
  }
  return nullptr;
}

napi_value CronetUrlRequest::Start(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetUrlRequest* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  if (!obj->is_owned_) {
    napi_throw_error(env, nullptr, "CronetUrlRequest not owned");
    return nullptr;
  }
  if (!obj->inited_) {
    napi_throw_error(env, nullptr, "CronetUrlRequest not inited");
    return nullptr;
  }
  if (obj->started_) {
    napi_throw_error(env, nullptr, "CronetUrlRequest already started");
    return nullptr;
  }

  Cronet_RESULT result = _Cronet_UrlRequest_Start(obj->ptr_);
  if (result == Cronet_RESULT_SUCCESS) {
    obj->started_ = true;
  } else {
    CronetUtil::ThrowCronetResultError(env, result);
  }
  return nullptr;
}

napi_value CronetUrlRequest::Cancel(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetUrlRequest* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  _Cronet_UrlRequest_Cancel(obj->ptr_);
  return nullptr;
}

napi_value CronetUrlRequest::FollowRedirect(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetUrlRequest* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  Cronet_RESULT result = _Cronet_UrlRequest_FollowRedirect(obj->ptr_);
  if (result != Cronet_RESULT_SUCCESS) {
    CronetUtil::ThrowCronetResultError(env, result);
  }
  return nullptr;
}

napi_value CronetUrlRequest::Read(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value value;
  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_object, *CronetBuffer::ref)) {
    return nullptr;
  }

  CronetUrlRequest* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  CronetBuffer *buffer;
  DCHECK(napi_unwrap(env, value, reinterpret_cast<void**>(&buffer)));

  Cronet_RESULT result = _Cronet_UrlRequest_Read(obj->ptr_, buffer->ptr());
  if (result == Cronet_RESULT_SUCCESS) {
    buffer->Unowned();
  } else {
    CronetUtil::ThrowCronetResultError(env, result);
  }
  return nullptr;
}
