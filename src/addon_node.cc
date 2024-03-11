
#include "addon_node.h"

#include <node_api.h>

#include "cronet_buffer.h"
#include "cronet_engine_params.h"
#include "cronet_engine.h"
#include "cronet_error.h"
#include "cronet_executor.h"
#include "cronet_http_header.h"
#include "cronet_upload_data_provider.h"
#include "cronet_upload_data_sink.h"
#include "cronet_url_request_callback.h"
#include "cronet_url_request_params.h"
#include "cronet_url_request.h"
#include "cronet_url_response_info.h"

napi_value create_addon(napi_env env, napi_value exports) {
  napi_status status;

  CronetBuffer::Register(env, exports);
  CronetEngineParams::Register(env, exports);
  CronetEngine::Register(env, exports);
  CronetError::Register(env, exports);
  CronetExecutor::Register(env, exports);
  CronetHttpHeader::Register(env, exports);
  CronetUploadDataProvider::Register(env, exports);
  CronetUploadDataSink::Register(env, exports);
  CronetUrlRequestCallback::Register(env, exports);
  CronetUrlRequestParams::Register(env, exports);
  CronetUrlRequest::Register(env, exports);
  CronetUrlResponseInfo::Register(env, exports);

  // Return the decorated exports object.
  return exports;
}

NAPI_MODULE_INIT(/* napi_env env, napi_value exports */) {
  // This function body is expected to return a `napi_value`.
  // The variables `napi_env env` and `napi_value exports` may be used within
  // the body, as they are provided by the definition of `NAPI_MODULE_INIT()`.
  return create_addon(env, exports);
}
