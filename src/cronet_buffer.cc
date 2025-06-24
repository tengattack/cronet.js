
#include "cronet_buffer.h"

#include "addon_node.h"
#include "cronet_util.h"

CronetBuffer::CronetBuffer()
    : ptr_(_Cronet_Buffer_Create())
    , is_owned_(true) {
  _Cronet_Buffer_SetClientContext(ptr_, this);
}

CronetBuffer::CronetBuffer(Cronet_BufferPtr ptr)
    : ptr_(ptr)
    , is_owned_(false) {
}

CronetBuffer::~CronetBuffer() {
  TRACE("~CronetBuffer()\n");
  if (is_owned_) {
    _Cronet_Buffer_Destroy(ptr_);
  }
  ptr_ = nullptr;
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetBuffer)

napi_value CronetBuffer::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    { "size", 0, 0, GetSize, 0, 0, napi_enumerable, 0 },
    { "data", 0, 0, GetData, 0, 0, napi_enumerable, 0 },

    DECLARE_NAPI_METHOD("initWithAlloc", InitWithAlloc),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetBuffer", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));

  ref = RegisterClass(env, "CronetBuffer", cons, exports);
  return exports;
}

void CronetBuffer::Unowned() {
  _Cronet_Buffer_SetClientContext(ptr_, nullptr);
  ptr_ = nullptr;
  is_owned_ = false;
}

napi_value CronetBuffer::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetBuffer(...)\n");
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));
    if (argc > 1) {
      CronetUtil::ThrowInvalidArgumentError(env);
      return nullptr;
    }

    Cronet_BufferPtr ptr = nullptr;
    if (argc == 1) {
      napi_valuetype valuetype;
      DCHECK(napi_typeof(env, args[0], &valuetype));

      if (valuetype == napi_external) {
        DCHECK(napi_get_value_external(env, args[0], (void**)&ptr));
      }
    }

    CronetBuffer* obj;
    if (ptr) {
      obj = new CronetBuffer(ptr);
    } else {
      obj = new CronetBuffer();
    }

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetBuffer::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetBuffer(...)\n");
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

napi_value CronetBuffer::WrapOwned(napi_env env, Cronet_BufferPtr ptr) {
  napi_status status;

  // TODO: optimize
  napi_value result = WrapUnowned(env, ptr);
  CronetBuffer* obj;
  DCHECK(napi_unwrap(env, result, reinterpret_cast<void**>(&obj)));

  _Cronet_Buffer_SetClientContext(obj->ptr_, obj);
  obj->is_owned_ = true;

  return result;
}

napi_value CronetBuffer::WrapUnowned(napi_env env, Cronet_BufferPtr ptr) {
  return WrapInternal(env, ref, ptr);
}

napi_value CronetBuffer::GetSize(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  DCHECK(napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr));

  CronetBuffer* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  napi_value result;
  DCHECK(napi_create_uint32(env, static_cast<uint32_t>(_Cronet_Buffer_GetSize(obj->ptr_)), &result));
  return result;
}

napi_value CronetBuffer::GetData(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  DCHECK(napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr));

  CronetBuffer* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  napi_value result;
  DCHECK(napi_create_external_buffer(env,
                                     static_cast<size_t>(_Cronet_Buffer_GetSize(obj->ptr_)),
                                     _Cronet_Buffer_GetData(obj->ptr_),
                                     nullptr,
                                     nullptr,
                                     &result));
  return result;
}

napi_value CronetBuffer::InitWithAlloc(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value value;
  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_number)) {
    return nullptr;
  }

  CronetBuffer* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  uint32_t result;
  DCHECK(napi_get_value_uint32(env, value, &result));
  TRACE("InitWithAlloc: %u\n", result);

  _Cronet_Buffer_InitWithAlloc(obj->ptr_, static_cast<uint64_t>(result));
  return nullptr;
}
