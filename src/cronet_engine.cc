
#include "cronet_engine.h"

#include <string>

#include "addon_node.h"
#include "cronet_engine_params.h"
#include "cronet_util.h"

CronetEngine::CronetEngine()
    : ptr_(_Cronet_Engine_Create())
    , started_(false) {
  _Cronet_Engine_SetClientContext(ptr_, this);
}

CronetEngine::~CronetEngine() {
  TRACE("~CronetEngine()\n");
  if (started_) {
    // TODO
  }
  _Cronet_Engine_Destroy(ptr_);
  ptr_ = NULL;
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetEngine)

napi_value CronetEngine::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    { "versionString", 0, 0, GetVersionString, 0, 0, napi_enumerable, 0 },

    DECLARE_NAPI_METHOD("getPtr", GetPtr),
    DECLARE_NAPI_METHOD("startWithParams", StartWithParams),
    DECLARE_NAPI_METHOD("startNetLogToFile", StartNetLogToFile),
    DECLARE_NAPI_METHOD("stopNetLog", StopNetLog),
    DECLARE_NAPI_METHOD("shutdown", Shutdown),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetEngine", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));

#ifndef LINK_CRONET
  napi_property_descriptor static_prop =
      DECLARE_NAPI_METHOD("loadLibrary", CronetUtil::LoadLibrary);
  NODE_API_CALL(env, napi_define_properties(env, cons, 1, &static_prop));
#endif // !LINK_CRONET

  ref = RegisterClass(env, "CronetEngine", cons, exports);
  return exports;
}

napi_value CronetEngine::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetEngine(...)\n");
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));
    if (argc != 0) {
      CronetUtil::ThrowInvalidArgumentError(env);
      return nullptr;
    }

    // napi_valuetype valuetype;
    // status = napi_typeof(env, args[0], &valuetype);
    // assert(status == napi_ok);

    // if (valuetype != napi_undefined) {
    //   status = napi_get_value_double(env, args[0], &value);
    //   assert(status == napi_ok);
    // }

    CronetEngine* obj = new CronetEngine();

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetEngine::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetEngine(...)\n");
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

napi_value CronetEngine::GetVersionString(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  DCHECK(napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr));

  CronetEngine* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  napi_value result;
  DCHECK(napi_create_string_utf8(env, _Cronet_Engine_GetVersionString(obj->ptr_), NAPI_AUTO_LENGTH, &result));
  return result;
}

napi_value CronetEngine::GetPtr(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetEngine* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  napi_value result;
  DCHECK(napi_create_bigint_int64(env, static_cast<int64_t>(reinterpret_cast<uintptr_t>(obj->ptr_)), &result));
  return result;
}

napi_value CronetEngine::StartWithParams(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value value;
  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_object, *CronetEngineParams::ref)) {
    return nullptr;
  }

  CronetEngine* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  if (obj->started_) {
    napi_throw_error(env, nullptr, "CronetEngine already started");
    return nullptr;
  }

  CronetEngineParams *params;
  DCHECK(napi_unwrap(env, value, reinterpret_cast<void**>(&params)));

  Cronet_RESULT result = _Cronet_Engine_StartWithParams(obj->ptr_, params->ptr());
  if (result == Cronet_RESULT_SUCCESS) {
    obj->AddRef();
    obj->started_ = true;
  } else {
    CronetUtil::ThrowCronetResultError(env, result);
  }
  return nullptr;
}

napi_value CronetEngine::StartNetLogToFile(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[argc];
  napi_valuetype arg_types[] = {napi_string, napi_boolean};
  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, argc, args, arg_types)) {
    return nullptr;
  }

  CronetEngine* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  size_t str_length, copied;
  DCHECK(napi_get_value_string_utf8(env, args[0], nullptr, 0, &str_length));
  std::string filename_str(str_length, '\0');
  DCHECK(napi_get_value_string_utf8(env, args[0], &filename_str[0], filename_str.length() + 1, &copied));

  bool log_all;
  DCHECK(napi_get_value_bool(env, args[1], &log_all));

  bool ret = _Cronet_Engine_StartNetLogToFile(obj->ptr_, filename_str.c_str(), log_all);
  napi_value result;
  DCHECK(napi_get_boolean(env, ret, &result));
  return result;
}

napi_value CronetEngine::StopNetLog(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetEngine* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  _Cronet_Engine_StopNetLog(obj->ptr_);
  return nullptr;
}

napi_value CronetEngine::Shutdown(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetEngine* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  if (!obj->started_) {
    napi_throw_error(env, nullptr, "CronetEngine not started");
    return nullptr;
  }

  Cronet_RESULT result = _Cronet_Engine_Shutdown(obj->ptr_);
  if (result == Cronet_RESULT_SUCCESS) {
    obj->ReleaseRef();
    obj->started_ = false;
  } else {
    CronetUtil::ThrowCronetResultError(env, result);
  }
  return nullptr;
}
