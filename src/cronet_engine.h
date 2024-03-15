#ifndef _CRONET_ENGINE_H_
#define _CRONET_ENGINE_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetEngine : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetEngine)

 public:
  Cronet_EnginePtr ptr() { return ptr_; }

 private:
  explicit CronetEngine();
  ~CronetEngine();

  static napi_value GetVersionString(napi_env env, napi_callback_info info);
  static napi_value StartWithParams(napi_env env, napi_callback_info info);
  static napi_value StartNetLogToFile(napi_env env, napi_callback_info info);
  static napi_value StopNetLog(napi_env env, napi_callback_info info);
  static napi_value Shutdown(napi_env env, napi_callback_info info);

  Cronet_EnginePtr ptr_;
  bool started_;
};

#endif // _CRONET_ENGINE_H_
