
#include "cronet_error.h"

#include <string>
#include <vector>

#include "addon_node.h"
#include "cronet_util.h"

CronetError::CronetError()
    : ptr_(_Cronet_Error_Create())
    , is_owned_(true) {
}

CronetError::CronetError(Cronet_ErrorPtr ptr)
    : ptr_(ptr)
    , is_owned_(false) {
}

CronetError::~CronetError() {
  TRACE("~CronetError()\n");
  if (is_owned_) {
    _Cronet_Error_Destroy(ptr_);
  }
  ptr_ = nullptr;
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetError)

static napi_value RegisterCronetErrorErrorCode(napi_env env, napi_value exports) {
  napi_status status;
  napi_value obj;
  napi_value value;
  std::vector<napi_property_descriptor> properties;

  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_CALLBACK);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_HOSTNAME_NOT_RESOLVED);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_INTERNET_DISCONNECTED);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_NETWORK_CHANGED);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_TIMED_OUT);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_CONNECTION_CLOSED);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_CONNECTION_TIMED_OUT);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_CONNECTION_REFUSED);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_CONNECTION_RESET);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_ADDRESS_UNREACHABLE);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_QUIC_PROTOCOL_FAILED);
  DECLARE_CRONET_CONST(Cronet_Error_ERROR_CODE, ERROR_OTHER);

  NODE_API_CALL(env, napi_create_object(env, &obj));
  NODE_API_CALL(env, napi_define_properties(env, obj, properties.size(), &properties[0]));
  NODE_API_CALL(env, napi_set_named_property(env, exports, "ERROR_CODE", obj));
  return nullptr;
}

napi_value CronetError::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    DECLARE_NAPI_MAGIC_METHOD("message", message),

    DECLARE_NAPI_MAGIC_METHOD("errorCode", error_code),
    DECLARE_NAPI_MAGIC_METHOD("internalErrorCode", internal_error_code),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetError", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));
  RegisterCronetErrorErrorCode(env, cons);

  ref = RegisterClass(env, "CronetError", cons, exports);
  return exports;
}

napi_value CronetError::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetError(...)\n");
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));
    if (argc > 1) {
      CronetUtil::ThrowInvalidArgumentError(env);
      return nullptr;
    }

    Cronet_ErrorPtr ptr = nullptr;
    if (argc == 1) {
      napi_valuetype valuetype;
      DCHECK(napi_typeof(env, args[0], &valuetype));

      if (valuetype == napi_external) {
        DCHECK(napi_get_value_external(env, args[0], (void**)&ptr));
      } else {
        CronetUtil::ThrowInvalidArgumentError(env);
        return nullptr;
      }
    }

    CronetError* obj;
    if (ptr) {
      obj = new CronetError(ptr);
    } else {
      obj = new CronetError();
    }

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetError::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetError(...)\n");
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

napi_value CronetError::WrapUnowned(napi_env env, Cronet_ErrorPtr ptr) {
  return WrapInternal(env, ref, ptr);
}

DECLARE_CLAZZ_STRING_GETTER_SETTER(Error, message)

DECLARE_CLAZZ_INT32_GETTER_SETTER(Error, error_code, Cronet_Error_ERROR_CODE)
DECLARE_CLAZZ_INT32_GETTER_SETTER(Error, internal_error_code, Cronet_Error_ERROR_CODE)
