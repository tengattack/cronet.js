#ifndef _CRONET_ENGINE_PARAMS_H_
#define _CRONET_ENGINE_PARAMS_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetEngineParams : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetEngineParams)

 public:
  Cronet_EngineParamsPtr ptr() { return ptr_; }

 private:
  explicit CronetEngineParams();
  ~CronetEngineParams();

  // boolean
  DECLARE_NAPI_MAGIC_METHOD_HEADER(enable_quic)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(enable_http2)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(enable_brotli)

  // string
  DECLARE_NAPI_MAGIC_METHOD_HEADER(user_agent)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(storage_path)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(experimental_options)

  // int32
  DECLARE_NAPI_MAGIC_METHOD_HEADER(http_cache_mode)

  // int64
  DECLARE_NAPI_MAGIC_METHOD_HEADER(http_cache_max_size)

  Cronet_EngineParamsPtr ptr_;
};

#endif // _CRONET_ENGINE_PARAMS_H_
