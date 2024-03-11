#ifndef _CRONET_ERROR_H_
#define _CRONET_ERROR_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetError : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetError)

 public:
  static napi_value WrapUnowned(napi_env env, Cronet_ErrorPtr ptr);

  Cronet_ErrorPtr ptr() { return ptr_; }

 private:
  explicit CronetError();
  explicit CronetError(Cronet_ErrorPtr ptr);
  ~CronetError();

  // string
  DECLARE_NAPI_MAGIC_METHOD_HEADER(message)

  // int32
  DECLARE_NAPI_MAGIC_METHOD_HEADER(error_code)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(internal_error_code)

  bool is_owned_;
  Cronet_ErrorPtr ptr_;
};

#endif // _CRONET_ERROR_H_
