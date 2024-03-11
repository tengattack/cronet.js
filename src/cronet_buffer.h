#ifndef _CRONET_BUFFER_H_
#define _CRONET_BUFFER_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetBuffer : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetBuffer)

 public:
  static napi_value WrapOwned(napi_env env, Cronet_BufferPtr ptr);
  static napi_value WrapUnowned(napi_env env, Cronet_BufferPtr ptr);

  Cronet_BufferPtr ptr() { return ptr_; }

  void Unowned();

 private:
  explicit CronetBuffer();
  explicit CronetBuffer(Cronet_BufferPtr ptr);
  ~CronetBuffer();

  static napi_value GetSize(napi_env env, napi_callback_info info);
  static napi_value GetData(napi_env env, napi_callback_info info);
  static napi_value InitWithAlloc(napi_env env, napi_callback_info info);

  bool is_owned_;
  Cronet_BufferPtr ptr_;
};

#endif // _CRONET_BUFFER_H_
