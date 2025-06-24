
#include "cronet_url_response_info.h"

#include <string>

#include <cronet_c.h>

#include "addon_node.h"
#include "cronet_headers.h"
#include "cronet_util.h"

CronetUrlResponseInfo::CronetUrlResponseInfo()
    : ptr_(_Cronet_UrlResponseInfo_Create())
    , is_owned_(true) {
}

CronetUrlResponseInfo::CronetUrlResponseInfo(Cronet_UrlResponseInfoPtr ptr)
    : ptr_(ptr)
    , is_owned_(false) {
}

CronetUrlResponseInfo::~CronetUrlResponseInfo() {
  TRACE("~CronetUrlResponseInfo()\n");
  if (is_owned_) {
    _Cronet_UrlResponseInfo_Destroy(ptr_);
  }
  ptr_ = nullptr;
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetUrlResponseInfo)

napi_value CronetUrlResponseInfo::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    DECLARE_NAPI_MAGIC_METHOD("url", url),
    DECLARE_NAPI_MAGIC_METHOD("negotiatedProtocol", negotiated_protocol),
    DECLARE_NAPI_MAGIC_METHOD("httpStatusText", http_status_text),

    DECLARE_NAPI_MAGIC_METHOD("httpStatusCode", http_status_code),

    DECLARE_CLAZZ_CRONET_HEADERS_METHOD(CronetUrlResponseInfo, allHeadersList),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetUrlResponseInfo", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));

  ref = RegisterClass(env, "CronetUrlResponseInfo", cons, exports);
  return exports;
}

napi_value CronetUrlResponseInfo::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetUrlResponseInfo(...)\n");
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));
    if (argc > 1) {
      CronetUtil::ThrowInvalidArgumentError(env);
      return nullptr;
    }

    Cronet_UrlResponseInfoPtr ptr = nullptr;
    if (argc == 1) {
      napi_valuetype valuetype;
      DCHECK(napi_typeof(env, args[0], &valuetype));

      if (valuetype == napi_external) {
        DCHECK(napi_get_value_external(env, args[0], (void**)&ptr));
      }
    }

    CronetUrlResponseInfo* obj;
    if (ptr) {
      obj = new CronetUrlResponseInfo(ptr);
    } else {
      obj = new CronetUrlResponseInfo();
    }

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetUrlResponseInfo::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetUrlResponseInfo(...)\n");
    size_t argc_ = 1;
    napi_value args[1];
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc_, args, nullptr, nullptr));
    if (argc_ != 0) {
      CronetUtil::ThrowInvalidArgumentError(env);
      return nullptr;
    }

    napi_value instance;
    NODE_API_CALL(env, napi_new_instance(env, Constructor(env), argc_, args, &instance));

    return instance;
  }
}

napi_value CronetUrlResponseInfo::WrapUnowned(napi_env env, Cronet_UrlResponseInfoPtr ptr) {
  return WrapInternal(env, ref, ptr);
}

DECLARE_CLAZZ_STRING_GETTER_SETTER(UrlResponseInfo, url)
DECLARE_CLAZZ_STRING_GETTER_SETTER(UrlResponseInfo, negotiated_protocol)
DECLARE_CLAZZ_STRING_GETTER_SETTER(UrlResponseInfo, http_status_text)

DECLARE_CLAZZ_INT32_GETTER_SETTER(UrlResponseInfo, http_status_code, int32_t)
