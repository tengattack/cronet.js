#ifndef _CRONET_UTIL_H_
#define _CRONET_UTIL_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#define DECLARE_CLAZZ_METHOD_NAME(clazz, name, method) Cronet_##clazz##_##name##method

#define DECLARE_CLAZZ_BOOL_GETTER_SETTER(clazz, name) \
napi_value Cronet##clazz::get_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  napi_value result; \
  DCHECK(napi_get_boolean(env, DECLARE_CLAZZ_METHOD_NAME(clazz, name, _get)(obj->ptr_), &result)); \
  return result; \
} \
napi_value Cronet##clazz::set_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value value; \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_boolean)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  bool result; \
  DCHECK(napi_get_value_bool(env, value, &result)); \
  DECLARE_CLAZZ_METHOD_NAME(clazz, name, _set)(obj->ptr_, result); \
  return nullptr; \
}

#define DECLARE_CLAZZ_STRING_GETTER_SETTER(clazz, name) \
napi_value Cronet##clazz::get_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  napi_value result; \
  DCHECK(napi_create_string_utf8(env, DECLARE_CLAZZ_METHOD_NAME(clazz, name, _get)(obj->ptr_), NAPI_AUTO_LENGTH, &result)); \
  return result; \
} \
napi_value Cronet##clazz::set_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value value; \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_string)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  size_t str_length, copied; \
  DCHECK(napi_get_value_string_utf8(env, value, nullptr, 0, &str_length)); \
  std::string str(str_length, '\0'); \
  DCHECK(napi_get_value_string_utf8(env, value, &str[0], str.length() + 1, &copied)); \
  DECLARE_CLAZZ_METHOD_NAME(clazz, name, _set)(obj->ptr_, str.c_str()); \
  return nullptr; \
}

#define DECLARE_CLAZZ_INT32_GETTER_SETTER(clazz, name, T) \
napi_value Cronet##clazz::get_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  napi_value result; \
  DCHECK(napi_create_int32(env, static_cast<int32_t>(DECLARE_CLAZZ_METHOD_NAME(clazz, name, _get)(obj->ptr_)), &result)); \
  return result; \
} \
napi_value Cronet##clazz::set_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value value; \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_number)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  int32_t result; \
  DCHECK(napi_get_value_int32(env, value, &result)); \
  DECLARE_CLAZZ_METHOD_NAME(clazz, name, _set)(obj->ptr_, (T)result); \
  return nullptr; \
}

#define DECLARE_REF_NAME(name, suffix) name##suffix

#define DECLARE_CLAZZ_FUNC_GETTER_SETTER(clazz, name) \
napi_value Cronet##clazz::get_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  napi_value result = nullptr; \
  if (obj->DECLARE_REF_NAME(name, _ref_)) { \
    DCHECK(napi_get_reference_value(env, obj->DECLARE_REF_NAME(name, _ref_), &result)); \
  } \
  return result; \
} \
napi_value Cronet##clazz::set_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value value; \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_function)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  if (obj->DECLARE_REF_NAME(name, _ref_)) { \
    DCHECK(napi_delete_reference(env, obj->DECLARE_REF_NAME(name, _ref_))); \
    obj->DECLARE_REF_NAME(name, _ref_) = nullptr; \
  } \
  DCHECK(napi_create_reference(env, value, 1, &obj->DECLARE_REF_NAME(name, _ref_))); \
  return nullptr; \
}

class CronetUtil {
 public:
  static bool ValidateAndGetCbInfo(napi_env env, napi_callback_info info, napi_value* this_arg);
  static bool ValidateAndGetCbInfo(napi_env env, napi_callback_info info, napi_value* this_arg, napi_value* arg, napi_valuetype arg_type, napi_ref arg_cons_ref = nullptr);
  static bool ValidateAndGetCbInfo(napi_env env, napi_callback_info info, napi_value* this_arg, size_t argc_expected, napi_value *argv, napi_valuetype *arg_types);

  static void ThrowInvalidArgumentError(napi_env env);
  static void ThrowCronetResultError(napi_env env, Cronet_RESULT result);
};

#endif // _CRONET_UTIL_H_
