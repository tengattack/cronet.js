
#include "cronet_upload_data_sink.h"

#include <string>

#include "addon_node.h"
#include "cronet_util.h"

CronetUploadDataSink::CronetUploadDataSink()
    : ptr_(nullptr /* Cronet_UploadDataSink_Create() */)
    , is_owned_(true) {
}

CronetUploadDataSink::CronetUploadDataSink(Cronet_UploadDataSinkPtr ptr)
    : ptr_(ptr)
    , is_owned_(false) {
}

CronetUploadDataSink::~CronetUploadDataSink() {
  TRACE("~CronetUploadDataSink()\n");
  if (is_owned_) {
    _Cronet_UploadDataSink_Destroy(ptr_);
  }
  ptr_ = nullptr;
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetUploadDataSink)

napi_value CronetUploadDataSink::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    DECLARE_NAPI_METHOD("onReadError", OnReadError),
    DECLARE_NAPI_METHOD("onReadSucceeded", OnReadSucceeded),
    DECLARE_NAPI_METHOD("onRewindError", OnRewindError),
    DECLARE_NAPI_METHOD("onRewindSucceeded", OnRewindSucceeded),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetUploadDataSink", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));

  ref = RegisterClass(env, "CronetUploadDataSink", cons, exports);
  return exports;
}

napi_value CronetUploadDataSink::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetUploadDataSink(...)\n");
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));
    if (argc > 1) {
      CronetUtil::ThrowInvalidArgumentError(env);
      return nullptr;
    }

    Cronet_UploadDataSinkPtr ptr = nullptr;
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

    CronetUploadDataSink* obj;
    if (ptr) {
      obj = new CronetUploadDataSink(ptr);
    } else {
      // obj = new CronetUploadDataSink();
      DCHECK(napi_throw_error(env, nullptr, "No implementation for CronetUploadDataSink"));
      return nullptr;
    }

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetUploadDataSink::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetUploadDataSink(...)\n");
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

napi_value CronetUploadDataSink::WrapUnowned(napi_env env, Cronet_UploadDataSinkPtr ptr) {
  return WrapInternal(env, ref, ptr);
}

#define DECLARE_SINK_ACTION_ERROR(action) \
napi_value CronetUploadDataSink::On##action##Error(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value value; \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_string)) { \
    return nullptr; \
  } \
  \
  CronetUploadDataSink* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  size_t str_length, copied; \
  DCHECK(napi_get_value_string_utf8(env, value, nullptr, 0, &str_length)); \
  std::string str(str_length, '\0'); \
  DCHECK(napi_get_value_string_utf8(env, value, &str[0], str.length() + 1, &copied)); \
  \
  _Cronet_UploadDataSink_On##action##Error(obj->ptr_, str.c_str()); \
  return nullptr; \
}

DECLARE_SINK_ACTION_ERROR(Read)

napi_value CronetUploadDataSink::OnReadSucceeded(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[argc];
  napi_valuetype arg_types[] = {napi_number, napi_boolean};
  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, argc, args, arg_types)) {
    return nullptr;
  }

  CronetUploadDataSink* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  uint32_t bytes_read;
  DCHECK(napi_get_value_uint32(env, args[0], &bytes_read));

  bool final_chunk;
  DCHECK(napi_get_value_bool(env, args[1], &final_chunk));

  _Cronet_UploadDataSink_OnReadSucceeded(obj->ptr_, bytes_read, final_chunk);
  return nullptr;
}

DECLARE_SINK_ACTION_ERROR(Rewind)

napi_value CronetUploadDataSink::OnRewindSucceeded(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetUploadDataSink* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  _Cronet_UploadDataSink_OnRewindSucceeded(obj->ptr_);
  return nullptr;
}
