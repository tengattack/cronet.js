#ifndef _CRONET_URL_RESPONSE_INFO_H_
#define _CRONET_URL_RESPONSE_INFO_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetUrlResponseInfo : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetUrlResponseInfo)

 public:
  static napi_value WrapUnowned(napi_env env, Cronet_UrlResponseInfoPtr ptr);

  Cronet_UrlResponseInfoPtr ptr() { return ptr_; }

 private:
  explicit CronetUrlResponseInfo();
  explicit CronetUrlResponseInfo(Cronet_UrlResponseInfoPtr ptr);
  ~CronetUrlResponseInfo();

  // string
  DECLARE_NAPI_MAGIC_METHOD_HEADER(url)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(negotiated_protocol)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(http_status_text)

  // int32
  DECLARE_NAPI_MAGIC_METHOD_HEADER(http_status_code)

  bool is_owned_;
  Cronet_UrlResponseInfoPtr ptr_;
};

#endif // _CRONET_URL_RESPONSE_INFO_H_
