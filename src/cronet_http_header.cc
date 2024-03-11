
#include "cronet_http_header.h"

#include <string>

#include "addon_node.h"
#include "cronet_util.h"

CronetHttpHeader::CronetHttpHeader()
    : ptr_(Cronet_HttpHeader_Create())
    , is_owned_(true) {
}

CronetHttpHeader::CronetHttpHeader(Cronet_HttpHeaderPtr ptr)
    : ptr_(ptr)
    , is_owned_(false) {
}

CronetHttpHeader::~CronetHttpHeader() {
  TRACE("~CronetHttpHeader()\n");
  if (is_owned_) {
    Cronet_HttpHeader_Destroy(ptr_);
  }
  ptr_ = nullptr;
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetHttpHeader)

napi_value CronetHttpHeader::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    DECLARE_NAPI_MAGIC_METHOD("name", name),
    DECLARE_NAPI_MAGIC_METHOD("value", value),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetHttpHeader", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));

  ref = RegisterClass(env, "CronetHttpHeader", cons, exports);
  return exports;
}

napi_value CronetHttpHeader::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetHttpHeader(...)\n");
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));

    Cronet_HttpHeaderPtr ptr = nullptr;
    if (argc == 1) {
      napi_valuetype valuetype;
      DCHECK(napi_typeof(env, args[0], &valuetype));

      if (valuetype == napi_external) {
        DCHECK(napi_get_value_external(env, args[0], (void**)&ptr));
      }
    }

    CronetHttpHeader* obj;
    if (ptr) {
      obj = new CronetHttpHeader(ptr);
    } else {
      obj = new CronetHttpHeader();
    }

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetHttpHeader::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetHttpHeader(...)\n");
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

napi_value CronetHttpHeader::WrapUnowned(napi_env env, Cronet_HttpHeaderPtr ptr) {
  return WrapInternal(env, ref, ptr);
}

DECLARE_CLAZZ_STRING_GETTER_SETTER(HttpHeader, name)
DECLARE_CLAZZ_STRING_GETTER_SETTER(HttpHeader, value)
