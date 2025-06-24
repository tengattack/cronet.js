
#include "cronet_engine_params.h"

#include <string>
#include <vector>

#include <cronet_c.h>

#include "addon_node.h"
#include "cronet_util.h"

CronetEngineParams::CronetEngineParams()
    : ptr_(_Cronet_EngineParams_Create()) {
}

CronetEngineParams::~CronetEngineParams() {
  TRACE("~CronetEngineParams()\n");
  _Cronet_EngineParams_Destroy(ptr_);
  ptr_ = nullptr;
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetEngineParams)

static napi_value RegisterCronetEngineParamsHttpCacheMode(napi_env env, napi_value exports) {
  napi_status status;
  napi_value obj;
  napi_value value;
  std::vector<napi_property_descriptor> properties;

  DECLARE_CRONET_CONST(Cronet_EngineParams_HTTP_CACHE_MODE, DISABLED);
  DECLARE_CRONET_CONST(Cronet_EngineParams_HTTP_CACHE_MODE, IN_MEMORY);
  DECLARE_CRONET_CONST(Cronet_EngineParams_HTTP_CACHE_MODE, DISK_NO_HTTP);
  DECLARE_CRONET_CONST(Cronet_EngineParams_HTTP_CACHE_MODE, DISK);

  NODE_API_CALL(env, napi_create_object(env, &obj));
  NODE_API_CALL(env, napi_define_properties(env, obj, properties.size(), &properties[0]));
  NODE_API_CALL(env, napi_set_named_property(env, exports, "HTTP_CACHE_MODE", obj));
  return nullptr;
}

napi_value CronetEngineParams::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    DECLARE_NAPI_MAGIC_METHOD("enableQuic", enable_quic),
    DECLARE_NAPI_MAGIC_METHOD("enableHttp2", enable_http2),
    DECLARE_NAPI_MAGIC_METHOD("enableBrotli", enable_brotli),

    DECLARE_NAPI_MAGIC_METHOD("userAgent", user_agent),
    DECLARE_NAPI_MAGIC_METHOD("storagePath", storage_path),
    DECLARE_NAPI_MAGIC_METHOD("experimentalOptions", experimental_options),

    DECLARE_NAPI_MAGIC_METHOD("httpCacheMode", http_cache_mode),
    DECLARE_NAPI_MAGIC_METHOD("httpCacheMaxSize", http_cache_max_size),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetEngineParams", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));
  RegisterCronetEngineParamsHttpCacheMode(env, cons);

  ref = RegisterClass(env, "CronetEngineParams", cons, exports);
  return exports;
}

napi_value CronetEngineParams::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetEngineParams(...)\n");
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

    CronetEngineParams* obj = new CronetEngineParams();

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetEngineParams::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetEngineParams(...)\n");
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

DECLARE_CLAZZ_BOOL_GETTER_SETTER(EngineParams, enable_quic)
DECLARE_CLAZZ_BOOL_GETTER_SETTER(EngineParams, enable_http2)
DECLARE_CLAZZ_BOOL_GETTER_SETTER(EngineParams, enable_brotli)

DECLARE_CLAZZ_STRING_GETTER_SETTER(EngineParams, user_agent)
DECLARE_CLAZZ_STRING_GETTER_SETTER(EngineParams, storage_path)
DECLARE_CLAZZ_STRING_GETTER_SETTER(EngineParams, experimental_options)

DECLARE_CLAZZ_INT32_GETTER_SETTER(EngineParams, http_cache_mode, Cronet_EngineParams_HTTP_CACHE_MODE)

DECLARE_CLAZZ_INT64_GETTER_SETTER(EngineParams, http_cache_max_size)
