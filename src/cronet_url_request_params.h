#ifndef _CRONET_URL_REQUEST_PARAMS_H_
#define _CRONET_URL_REQUEST_PARAMS_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetExecutor;
class CronetUploadDataProvider;
class CronetUrlRequest;

class CronetUrlRequestParams : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetUrlRequestParams)

 public:
  Cronet_UrlRequestParamsPtr ptr() { return ptr_; }

 private:
  explicit CronetUrlRequestParams();
  ~CronetUrlRequestParams();

  // boolean
  DECLARE_NAPI_MAGIC_METHOD_HEADER(disable_cache)

  // string
  DECLARE_NAPI_MAGIC_METHOD_HEADER(http_method)

  // int32
  DECLARE_NAPI_MAGIC_METHOD_HEADER(priority)

  DECLARE_NAPI_MAGIC_METHOD_HEADER(upload_data_provider_executor)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(upload_data_provider)

  Cronet_UrlRequestParamsPtr ptr_;
  // Not owned, but has ref.
  CronetExecutor* upload_data_provider_executor_;
  CronetUploadDataProvider* upload_data_provider_;

  napi_ref upload_data_provider_executor_ref_;
  napi_ref upload_data_provider_ref_;

  friend class CronetUrlRequest;
};

#endif // _CRONET_URL_REQUEST_PARAMS_H_
