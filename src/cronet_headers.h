#ifndef _CRONET_HEADERS_H_
#define _CRONET_HEADERS_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#define DECLARE_CLAZZ_CRONET_HEADERS_METHOD(clazz, name) \
  DECLARE_NAPI_METHOD(#name "Add", CronetHeaders<clazz>::add), \
  DECLARE_NAPI_METHOD(#name "At", CronetHeaders<clazz>::at), \
  DECLARE_NAPI_METHOD(#name "Clear", CronetHeaders<clazz>::clear), \
  DECLARE_NAPI_METHOD(#name "Size", CronetHeaders<clazz>::size)

template<typename T>
class CronetHeaders {
 public:
  static napi_value add(napi_env env, napi_callback_info info);
  static napi_value at(napi_env env, napi_callback_info info);
  static napi_value clear(napi_env env, napi_callback_info info);
  static napi_value size(napi_env env, napi_callback_info info);
};

#endif
