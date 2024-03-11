#ifndef _CRONET_URL_REQUEST_H_
#define _CRONET_URL_REQUEST_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetUploadDataProvider;
class CronetUrlRequestCallback;

class CronetUrlRequest : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetUrlRequest)

 public:
  static napi_value WrapUnowned(napi_env env, Cronet_UrlRequestPtr ptr);

  Cronet_UrlRequestPtr ptr() { return ptr_; }

  void SetCallback(CronetUrlRequestCallback* callback);
  void SetUploadDataProvider(CronetUploadDataProvider* provider);
  void Done();

 private:
  explicit CronetUrlRequest();
  explicit CronetUrlRequest(Cronet_UrlRequestPtr ptr);
  ~CronetUrlRequest();

  static napi_value InitWithParams(napi_env env, napi_callback_info info);
  static napi_value Start(napi_env env, napi_callback_info info);
  static napi_value Cancel(napi_env env, napi_callback_info info);
  static napi_value FollowRedirect(napi_env env, napi_callback_info info);
  static napi_value Read(napi_env env, napi_callback_info info);

  bool is_owned_;
  Cronet_UrlRequestPtr ptr_;
  bool inited_;
  bool started_;
  // Not owned, but has ref.
  CronetUrlRequestCallback* callback_;
  CronetUploadDataProvider* upload_data_provider_;
};

#endif // _CRONET_URL_REQUEST_H_
