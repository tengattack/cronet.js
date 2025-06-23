
#include "cronet_url_request_callback.h"

#include <string>

#include <cronet_c.h>

#include "addon_node.h"
#include "cronet_buffer.h"
#include "cronet_error.h"
#include "cronet_executor.h"
#include "cronet_url_request.h"
#include "cronet_url_response_info.h"
#include "cronet_util.h"

CronetUrlRequestCallback::CronetUrlRequestCallback()
    : ptr_(_Cronet_UrlRequestCallback_CreateWith(
          CronetUrlRequestCallback::OnRedirectReceived,
          CronetUrlRequestCallback::OnResponseStarted,
          CronetUrlRequestCallback::OnReadCompleted,
          CronetUrlRequestCallback::OnSucceeded,
          CronetUrlRequestCallback::OnFailed,
          CronetUrlRequestCallback::OnCanceled))
    , on_redirect_received_ref_(nullptr)
    , on_response_started_ref_(nullptr)
    , on_read_completed_ref_(nullptr)
    , on_succeeded_ref_(nullptr)
    , on_failed_ref_(nullptr)
    , on_canceled_ref_(nullptr) {
  _Cronet_UrlRequestCallback_SetClientContext(ptr_, this);
}

CronetUrlRequestCallback::~CronetUrlRequestCallback() {
  TRACE("~CronetUrlRequestCallback()\n");
  _Cronet_UrlRequestCallback_Destroy(ptr_);
  ptr_ = nullptr;
  SetExecutor(nullptr);
  if (on_redirect_received_ref_) {
    napi_delete_reference(env_, on_redirect_received_ref_);
  }
  if (on_response_started_ref_) {
    napi_delete_reference(env_, on_response_started_ref_);
  }
  if (on_read_completed_ref_) {
    napi_delete_reference(env_, on_read_completed_ref_);
  }
  if (on_succeeded_ref_) {
    napi_delete_reference(env_, on_succeeded_ref_);
  }
  if (on_failed_ref_) {
    napi_delete_reference(env_, on_failed_ref_);
  }
  if (on_canceled_ref_) {
    napi_delete_reference(env_, on_canceled_ref_);
  }
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetUrlRequestCallback)

napi_value CronetUrlRequestCallback::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    DECLARE_NAPI_MAGIC_METHOD("onRedirectReceived", on_redirect_received),
    DECLARE_NAPI_MAGIC_METHOD("onResponseStarted", on_response_started),
    DECLARE_NAPI_MAGIC_METHOD("onReadCompleted", on_read_completed),
    DECLARE_NAPI_MAGIC_METHOD("onSucceeded", on_succeeded),
    DECLARE_NAPI_MAGIC_METHOD("onFailed", on_failed),
    DECLARE_NAPI_MAGIC_METHOD("onCanceled", on_canceled),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetUrlRequestCallback", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));

  ref = RegisterClass(env, "CronetUrlRequestCallback", cons, exports);
  return exports;
}

napi_value CronetUrlRequestCallback::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetUrlRequestCallback(...)\n");
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

    CronetUrlRequestCallback* obj = new CronetUrlRequestCallback();

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetUrlRequestCallback::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetUrlRequestCallback(...)\n");
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

void CronetUrlRequestCallback::SetExecutor(CronetExecutor* executor) {
  if (executor_) {
    // executor_->ReleaseRef();
  }
  executor_ = executor;
  if (executor_) {
    // FIXME: why crash?
    // executor_->AddRef();
  }
}

DECLARE_CLAZZ_FUNC_GETTER_SETTER(UrlRequestCallback, on_redirect_received)
DECLARE_CLAZZ_FUNC_GETTER_SETTER(UrlRequestCallback, on_response_started)
DECLARE_CLAZZ_FUNC_GETTER_SETTER(UrlRequestCallback, on_read_completed)
DECLARE_CLAZZ_FUNC_GETTER_SETTER(UrlRequestCallback, on_succeeded)
DECLARE_CLAZZ_FUNC_GETTER_SETTER(UrlRequestCallback, on_failed)
DECLARE_CLAZZ_FUNC_GETTER_SETTER(UrlRequestCallback, on_canceled)

void CronetUrlRequestCallback::OnRedirectReceived(
    napi_env env,
    Cronet_UrlRequestPtr request,
    Cronet_UrlResponseInfoPtr info,
    Cronet_String newLocationUrl) {
  if (on_redirect_received_ref_) {
    napi_status status;
    napi_value cb;
    DCHECK(napi_get_reference_value(env, on_redirect_received_ref_, &cb));

    napi_value jsthis;
    DCHECK(napi_get_reference_value(env, wrapper_, &jsthis));

    napi_value jsrequest = nullptr;
    CronetUrlRequest* ctx = static_cast<CronetUrlRequest*>(
        _Cronet_UrlRequest_GetClientContext(request));
    if (ctx) {
      DCHECK(napi_get_reference_value(env, ctx->wrapper_, &jsrequest));
    }

    size_t argc = 3;
    napi_value argv[argc];
    argv[0] = jsrequest;
    argv[1] = CronetUrlResponseInfo::WrapUnowned(env, info);
    DCHECK(napi_create_string_utf8(env, newLocationUrl, NAPI_AUTO_LENGTH, &argv[2]));
    DCHECK(napi_call_function(env, jsthis, cb, argc, argv, nullptr));
  }
}

void CronetUrlRequestCallback::OnResponseStarted(
    napi_env env,
    Cronet_UrlRequestPtr request,
    Cronet_UrlResponseInfoPtr info) {
  if (on_response_started_ref_) {
    napi_status status;
    napi_value cb;
    DCHECK(napi_get_reference_value(env, on_response_started_ref_, &cb));

    napi_value jsthis;
    DCHECK(napi_get_reference_value(env, wrapper_, &jsthis));

    napi_value jsrequest = nullptr;
    CronetUrlRequest* ctx = static_cast<CronetUrlRequest*>(
        _Cronet_UrlRequest_GetClientContext(request));
    if (ctx) {
      DCHECK(napi_get_reference_value(env, ctx->wrapper_, &jsrequest));
    }

    size_t argc = 2;
    napi_value argv[argc];
    argv[0] = jsrequest;
    argv[1] = CronetUrlResponseInfo::WrapUnowned(env, info);
    DCHECK(napi_call_function(env, jsthis, cb, argc, argv, nullptr));
  }
}

void CronetUrlRequestCallback::OnReadCompleted(napi_env env,
                                               Cronet_UrlRequestPtr request,
                                               Cronet_UrlResponseInfoPtr info,
                                               Cronet_BufferPtr buffer,
                                               uint64_t bytes_read) {
  if (on_read_completed_ref_) {
    napi_status status;
    napi_value cb;
    DCHECK(napi_get_reference_value(env, on_read_completed_ref_, &cb));

    napi_value jsthis;
    DCHECK(napi_get_reference_value(env, wrapper_, &jsthis));

    napi_value jsrequest = nullptr;
    CronetUrlRequest* ctx = static_cast<CronetUrlRequest*>(
        _Cronet_UrlRequest_GetClientContext(request));
    if (ctx) {
      DCHECK(napi_get_reference_value(env, ctx->wrapper_, &jsrequest));
    }

    size_t argc = 4;
    napi_value argv[argc];
    argv[0] = jsrequest;
    argv[1] = CronetUrlResponseInfo::WrapUnowned(env, info);
    argv[2] = CronetBuffer::WrapOwned(env, buffer);
    DCHECK(napi_create_uint32(env, static_cast<uint32_t>(bytes_read), &argv[3]));
    DCHECK(napi_call_function(env, jsthis, cb, argc, argv, nullptr));
  }
}

void CronetUrlRequestCallback::OnSucceeded(napi_env env,
                                           Cronet_UrlRequestPtr request,
                                           Cronet_UrlResponseInfoPtr info) {
  if (on_succeeded_ref_) {
    napi_status status;
    napi_value cb;
    DCHECK(napi_get_reference_value(env, on_succeeded_ref_, &cb));

    napi_value jsthis;
    DCHECK(napi_get_reference_value(env, wrapper_, &jsthis));

    napi_value jsrequest = nullptr;
    CronetUrlRequest* ctx = static_cast<CronetUrlRequest*>(
        _Cronet_UrlRequest_GetClientContext(request));
    if (ctx) {
      DCHECK(napi_get_reference_value(env, ctx->wrapper_, &jsrequest));
    }

    size_t argc = 2;
    napi_value argv[argc];
    argv[0] = jsrequest;
    argv[1] = CronetUrlResponseInfo::WrapUnowned(env, info);
    DCHECK(napi_call_function(env, jsthis, cb, argc, argv, nullptr));
  }
}

void CronetUrlRequestCallback::OnFailed(napi_env env,
                                        Cronet_UrlRequestPtr request,
                                        Cronet_UrlResponseInfoPtr info,
                                        Cronet_ErrorPtr error) {
  if (on_failed_ref_) {
    napi_status status;
    napi_value cb;
    DCHECK(napi_get_reference_value(env, on_failed_ref_, &cb));

    napi_value jsthis;
    DCHECK(napi_get_reference_value(env, wrapper_, &jsthis));

    napi_value jsrequest = nullptr;
    CronetUrlRequest* ctx = static_cast<CronetUrlRequest*>(
        _Cronet_UrlRequest_GetClientContext(request));
    if (ctx) {
      DCHECK(napi_get_reference_value(env, ctx->wrapper_, &jsrequest));
    }

    size_t argc = 3;
    napi_value argv[argc];
    argv[0] = jsrequest;
    argv[1] = CronetUrlResponseInfo::WrapUnowned(env, info);
    argv[2] = CronetError::WrapUnowned(env, error);
    DCHECK(napi_call_function(env, jsthis, cb, argc, argv, nullptr));
  }
}

void CronetUrlRequestCallback::OnCanceled(napi_env env,
                                          Cronet_UrlRequestPtr request,
                                          Cronet_UrlResponseInfoPtr info) {
  if (on_canceled_ref_) {
    napi_status status;
    napi_value cb;
    DCHECK(napi_get_reference_value(env, on_canceled_ref_, &cb));

    napi_value jsthis;
    DCHECK(napi_get_reference_value(env, wrapper_, &jsthis));

    napi_value jsrequest = nullptr;
    CronetUrlRequest* ctx = static_cast<CronetUrlRequest*>(
        _Cronet_UrlRequest_GetClientContext(request));
    if (ctx) {
      DCHECK(napi_get_reference_value(env, ctx->wrapper_, &jsrequest));
    }

    size_t argc = 2;
    napi_value argv[argc];
    argv[0] = jsrequest;
    argv[1] = CronetUrlResponseInfo::WrapUnowned(env, info);
    DCHECK(napi_call_function(env, jsthis, cb, argc, argv, nullptr));
  }
}

void CronetUrlRequestCallback::Done(napi_env env, Cronet_UrlRequestPtr request) {
  CronetUrlRequest* ctx = static_cast<CronetUrlRequest*>(
      _Cronet_UrlRequest_GetClientContext(request));
  if (ctx) {
    ctx->Done();
  }
}

/* static */
CronetUrlRequestCallback* CronetUrlRequestCallback::GetThis(
    Cronet_UrlRequestCallbackPtr self) {
  return static_cast<CronetUrlRequestCallback*>(
      _Cronet_UrlRequestCallback_GetClientContext(self));
}

/* static */
void CronetUrlRequestCallback::OnRedirectReceived(
    Cronet_UrlRequestCallbackPtr self,
    Cronet_UrlRequestPtr request,
    Cronet_UrlResponseInfoPtr info,
    Cronet_String newLocationUrl) {
  TRACE("OnRedirectReceived called: %s\n", newLocationUrl);
  auto* that = GetThis(self);
  that->executor_->CallInJs([&](napi_env env) {
    that->OnRedirectReceived(env, request, info, newLocationUrl);
  });
}

/* static */
void CronetUrlRequestCallback::OnResponseStarted(
    Cronet_UrlRequestCallbackPtr self,
    Cronet_UrlRequestPtr request,
    Cronet_UrlResponseInfoPtr info) {
  TRACE("OnResponseStarted called.\n");
  TRACE("HTTP %s Status %d %s\n",
        _Cronet_UrlResponseInfo_negotiated_protocol_get(info),
        _Cronet_UrlResponseInfo_http_status_code_get(info),
        _Cronet_UrlResponseInfo_http_status_text_get(info));
  auto* that = GetThis(self);
  that->executor_->CallInJs([&](napi_env env) {
    that->OnResponseStarted(env, request, info);
  });
  TRACE("OnResponseStarted done.\n");
}

/* static */
void CronetUrlRequestCallback::OnReadCompleted(
    Cronet_UrlRequestCallbackPtr self,
    Cronet_UrlRequestPtr request,
    Cronet_UrlResponseInfoPtr info,
    Cronet_BufferPtr buffer,
    uint64_t bytesRead) {
  TRACE("OnReadCompleted called: %zu bytes read.\n", static_cast<size_t>(bytesRead));
  // std::string last_read_data(
  //     reinterpret_cast<char*>(_Cronet_Buffer_GetData(buffer)), bytesRead);
  // response_as_string_ += last_read_data;
  auto* that = GetThis(self);
  that->executor_->CallInJs([&](napi_env env) {
    that->OnReadCompleted(env, request, info, buffer, bytesRead);
  });
}

/* static */
void CronetUrlRequestCallback::OnSucceeded(Cronet_UrlRequestCallbackPtr self,
                                           Cronet_UrlRequestPtr request,
                                           Cronet_UrlResponseInfoPtr info) {
  TRACE("OnSucceeded called.\n");
  auto* that = GetThis(self);
  that->executor_->CallInJs([&](napi_env env) {
    that->OnSucceeded(env, request, info);
    that->Done(env, request);
  });
}

/* static */
void CronetUrlRequestCallback::OnFailed(Cronet_UrlRequestCallbackPtr self,
                                        Cronet_UrlRequestPtr request,
                                        Cronet_UrlResponseInfoPtr info,
                                        Cronet_ErrorPtr error) {
  TRACE("OnFailed called: %s\n", _Cronet_Error_message_get(error));
  // last_error_message_ = _Cronet_Error_message_get(error);
  auto* that = GetThis(self);
  that->executor_->CallInJs([&](napi_env env) {
    that->OnFailed(env, request, info, error);
    that->Done(env, request);
  });
}

/* static */
void CronetUrlRequestCallback::OnCanceled(Cronet_UrlRequestCallbackPtr self,
                                          Cronet_UrlRequestPtr request,
                                          Cronet_UrlResponseInfoPtr info) {
  TRACE("OnCanceled called.\n");
  auto* that = GetThis(self);
  that->executor_->CallInJs([&](napi_env env) {
    that->OnCanceled(env, request, info);
    that->Done(env, request);
  });
}
