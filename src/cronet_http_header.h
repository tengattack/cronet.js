#ifndef _CRONET_HTTP_HEADER_H_
#define _CRONET_HTTP_HEADER_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetHttpHeader : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetHttpHeader)

 public:
  static napi_value WrapUnowned(napi_env env, Cronet_HttpHeaderPtr ptr);

  Cronet_HttpHeaderPtr ptr() { return ptr_; }

 private:
  explicit CronetHttpHeader();
  explicit CronetHttpHeader(Cronet_HttpHeaderPtr ptr);
  ~CronetHttpHeader();

  // string
  DECLARE_NAPI_MAGIC_METHOD_HEADER(name)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(value)

  bool is_owned_;
  Cronet_HttpHeaderPtr ptr_;
};

#endif // _CRONET_HTTP_HEADER_H_
