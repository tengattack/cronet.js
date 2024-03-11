#ifndef _CRONET_UPLOAD_DATA_SINK_H_
#define _CRONET_UPLOAD_DATA_SINK_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetUploadDataSink : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetUploadDataSink)

 public:
  static napi_value WrapUnowned(napi_env env, Cronet_UploadDataSinkPtr ptr);

  Cronet_UploadDataSinkPtr ptr() { return ptr_; }

 private:
  explicit CronetUploadDataSink();
  explicit CronetUploadDataSink(Cronet_UploadDataSinkPtr ptr);
  ~CronetUploadDataSink();

  static napi_value OnReadError(napi_env env, napi_callback_info info);
  static napi_value OnReadSucceeded(napi_env env, napi_callback_info info);
  static napi_value OnRewindError(napi_env env, napi_callback_info info);
  static napi_value OnRewindSucceeded(napi_env env, napi_callback_info info);

  bool is_owned_;
  Cronet_UploadDataSinkPtr ptr_;
};

#endif // _CRONET_UPLOAD_DATA_SINK_H_
