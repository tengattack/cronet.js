
#include "cronet_error.h"

#include <string>

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

    Cronet_ErrorPtr ptr = nullptr;
    if (argc == 1) {
      napi_valuetype valuetype;
      DCHECK(napi_typeof(env, args[0], &valuetype));

      if (valuetype == napi_external) {
        DCHECK(napi_get_value_external(env, args[0], (void**)&ptr));
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

    const size_t argc = 1;
    napi_value argv[argc] = {args[0]};

    napi_value instance;
    NODE_API_CALL(env, napi_new_instance(env, Constructor(env), argc, argv, &instance));

    return instance;
  }
}

napi_value CronetError::WrapUnowned(napi_env env, Cronet_ErrorPtr ptr) {
  return WrapInternal(env, ref, ptr);
}

DECLARE_CLAZZ_STRING_GETTER_SETTER(Error, message)

DECLARE_CLAZZ_INT32_GETTER_SETTER(Error, error_code, Cronet_Error_ERROR_CODE)
DECLARE_CLAZZ_INT32_GETTER_SETTER(Error, internal_error_code, Cronet_Error_ERROR_CODE)
