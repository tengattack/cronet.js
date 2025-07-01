
#include "cronet_util.h"

#include <string>

#ifndef LINK_CRONET
#include <dlfcn.h>
#endif // !LINK_CRONET
#include <node_api.h>

#include "addon_node.h"

bool CronetUtil::ValidateAndGetCbInfo(napi_env env, napi_callback_info info, napi_value* this_arg) {
  napi_status status;
  size_t argc = 0;
  DCHECK(napi_get_cb_info(env, info, &argc, nullptr, this_arg, nullptr));
  if (argc != 0) {
    ThrowInvalidArgumentError(env);
    return false;
  }
  return true;
}

bool CronetUtil::ValidateAndGetCbInfo(napi_env env, napi_callback_info info, napi_value* this_arg, napi_value* arg, napi_valuetype arg_type, napi_ref arg_cons_ref) {
  napi_status status;
  size_t argc = 1;
  napi_value value;
  DCHECK(napi_get_cb_info(env, info, &argc, &value, this_arg, nullptr));
  if (argc != 1) {
    ThrowInvalidArgumentError(env);
    return false;
  }
  napi_valuetype valuetype;
  DCHECK(napi_typeof(env, value, &valuetype));
  if (valuetype != arg_type) {
    ThrowInvalidArgumentError(env);
    return false;
  }
  if (arg_cons_ref != nullptr) {
    bool isinstanceof;
    napi_value cons;
    DCHECK(napi_get_reference_value(env, arg_cons_ref, &cons));
    DCHECK(napi_instanceof(env, value, cons, &isinstanceof));
    if (!isinstanceof) {
      ThrowInvalidArgumentError(env);
      return false;
    }
  }
  *arg = value;
  return true;
}

bool CronetUtil::ValidateAndGetCbInfo(napi_env env, napi_callback_info info, napi_value* this_arg, size_t argc_expected, napi_value* argv, napi_valuetype* arg_types) {
  napi_status status;
  size_t argc = argc_expected;
  napi_value args[argc];
  DCHECK(napi_get_cb_info(env, info, &argc, args, this_arg, nullptr));
  if (argc != argc_expected) {
    ThrowInvalidArgumentError(env);
    return false;
  }
  napi_valuetype valuetype;
  for (size_t i = 0; i < argc; i++) {
    DCHECK(napi_typeof(env, args[i], &valuetype));
    if (valuetype != arg_types[i]) {
      ThrowInvalidArgumentError(env);
      return false;
    }
    argv[i] = args[i];
  }
  return true;
}

void CronetUtil::ThrowInvalidArgumentError(napi_env env) {
  napi_status status;
  DCHECK(napi_throw_error(env, nullptr, "InvalidArgument"));
}

void CronetUtil::ThrowCronetResultError(napi_env env, Cronet_RESULT result) {
  napi_status status;
  std::string code = std::to_string(static_cast<int>(result));
  const char* message = nullptr;

#define CASE_CRONET_RESULT(name) \
    case Cronet_RESULT_##name: \
      message = ("Cronet_RESULT_" #name); \
      break

  switch (result) {
    CASE_CRONET_RESULT(ILLEGAL_ARGUMENT);
    CASE_CRONET_RESULT(ILLEGAL_ARGUMENT_STORAGE_PATH_MUST_EXIST);
    CASE_CRONET_RESULT(ILLEGAL_ARGUMENT_INVALID_PIN);
    CASE_CRONET_RESULT(ILLEGAL_ARGUMENT_INVALID_HOSTNAME);
    CASE_CRONET_RESULT(ILLEGAL_ARGUMENT_INVALID_HTTP_METHOD);
    CASE_CRONET_RESULT(ILLEGAL_ARGUMENT_INVALID_HTTP_HEADER);
    CASE_CRONET_RESULT(ILLEGAL_STATE);
    CASE_CRONET_RESULT(ILLEGAL_STATE_STORAGE_PATH_IN_USE);
    CASE_CRONET_RESULT(ILLEGAL_STATE_CANNOT_SHUTDOWN_ENGINE_FROM_NETWORK_THREAD);
    CASE_CRONET_RESULT(ILLEGAL_STATE_ENGINE_ALREADY_STARTED);
    CASE_CRONET_RESULT(ILLEGAL_STATE_REQUEST_ALREADY_STARTED);
    CASE_CRONET_RESULT(ILLEGAL_STATE_REQUEST_NOT_INITIALIZED);
    CASE_CRONET_RESULT(ILLEGAL_STATE_REQUEST_ALREADY_INITIALIZED);
    CASE_CRONET_RESULT(ILLEGAL_STATE_REQUEST_NOT_STARTED);
    CASE_CRONET_RESULT(ILLEGAL_STATE_UNEXPECTED_REDIRECT);
    CASE_CRONET_RESULT(ILLEGAL_STATE_UNEXPECTED_READ);
    CASE_CRONET_RESULT(ILLEGAL_STATE_READ_FAILED);
    CASE_CRONET_RESULT(NULL_POINTER);
    CASE_CRONET_RESULT(NULL_POINTER_HOSTNAME);
    CASE_CRONET_RESULT(NULL_POINTER_SHA256_PINS);
    CASE_CRONET_RESULT(NULL_POINTER_EXPIRATION_DATE);
    CASE_CRONET_RESULT(NULL_POINTER_ENGINE);
    CASE_CRONET_RESULT(NULL_POINTER_URL);
    CASE_CRONET_RESULT(NULL_POINTER_CALLBACK);
    CASE_CRONET_RESULT(NULL_POINTER_EXECUTOR);
    CASE_CRONET_RESULT(NULL_POINTER_METHOD);
    CASE_CRONET_RESULT(NULL_POINTER_HEADER_NAME);
    CASE_CRONET_RESULT(NULL_POINTER_HEADER_VALUE);
    CASE_CRONET_RESULT(NULL_POINTER_PARAMS);
    CASE_CRONET_RESULT(NULL_POINTER_REQUEST_FINISHED_INFO_LISTENER_EXECUTOR);
    default:
      message = "Cronet_RESULT_UNKNOWN";
      break;
  }

#undef CASE_CRONET_RESULT

  DCHECK(napi_throw_error(env, code.c_str(), message));
}

#ifndef LINK_CRONET
// cronet apis
napi_value CronetUtil::LoadLibrary(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value value;
  if (!ValidateAndGetCbInfo(env, info, nullptr, &value, napi_string)) {
    return nullptr;
  }

  size_t str_length, copied;
  DCHECK(napi_get_value_string_utf8(env, value, nullptr, 0, &str_length));
  std::string str(str_length, '\0');
  DCHECK(napi_get_value_string_utf8(env, value, &str[0], str.length() + 1, &copied));

  void* lib = dlopen(str.c_str(), RTLD_NOW);
  if (!lib) {
    const char* err_msg = dlerror();
    if (!err_msg) {
      err_msg = "Unable to load library";
    }
    DCHECK(napi_throw_error(env, nullptr, err_msg));
  }

  typedef struct {
    const char* sym_name;
    void** sym_func;
  } CronetUtilSym;

#define DECLARE_SYM(name) {"Cronet_" #name, (void**)&api_##name}

  CronetUtilSym syms[] = {
    // CronetBuffer
    DECLARE_SYM(Buffer_Create),
    DECLARE_SYM(Buffer_SetClientContext),
    DECLARE_SYM(Buffer_Destroy),
    DECLARE_SYM(Buffer_InitWithAlloc),
    DECLARE_SYM(Buffer_GetSize),
    DECLARE_SYM(Buffer_GetData),
    // CronetEngineParams
    DECLARE_SYM(EngineParams_Create),
    DECLARE_SYM(EngineParams_Destroy),
    DECLARE_SYM(EngineParams_enable_quic_get),
    DECLARE_SYM(EngineParams_enable_quic_set),
    DECLARE_SYM(EngineParams_enable_http2_get),
    DECLARE_SYM(EngineParams_enable_http2_set),
    DECLARE_SYM(EngineParams_enable_brotli_get),
    DECLARE_SYM(EngineParams_enable_brotli_set),
    DECLARE_SYM(EngineParams_user_agent_get),
    DECLARE_SYM(EngineParams_user_agent_set),
    DECLARE_SYM(EngineParams_storage_path_get),
    DECLARE_SYM(EngineParams_storage_path_set),
    DECLARE_SYM(EngineParams_experimental_options_get),
    DECLARE_SYM(EngineParams_experimental_options_set),
    DECLARE_SYM(EngineParams_http_cache_mode_get),
    DECLARE_SYM(EngineParams_http_cache_mode_set),
    DECLARE_SYM(EngineParams_http_cache_max_size_get),
    DECLARE_SYM(EngineParams_http_cache_max_size_set),
    // CronetEngine
    DECLARE_SYM(Engine_Create),
    DECLARE_SYM(Engine_SetClientContext),
    DECLARE_SYM(Engine_Destroy),
    DECLARE_SYM(Engine_GetVersionString),
    DECLARE_SYM(Engine_StartWithParams),
    DECLARE_SYM(Engine_StartNetLogToFile),
    DECLARE_SYM(Engine_StopNetLog),
    DECLARE_SYM(Engine_Shutdown),
    // CronetError
    DECLARE_SYM(Error_Create),
    DECLARE_SYM(Error_Destroy),
    DECLARE_SYM(Error_message_get),
    DECLARE_SYM(Error_message_set),
    DECLARE_SYM(Error_error_code_get),
    DECLARE_SYM(Error_error_code_set),
    DECLARE_SYM(Error_internal_error_code_get),
    DECLARE_SYM(Error_internal_error_code_set),
    // CronetExecutor
    DECLARE_SYM(Executor_CreateWith),
    DECLARE_SYM(Executor_GetClientContext),
    DECLARE_SYM(Executor_SetClientContext),
    DECLARE_SYM(Executor_Destroy),
    DECLARE_SYM(Runnable_Run),
    DECLARE_SYM(Runnable_Destroy),
    // CronetHttpHeader
    DECLARE_SYM(HttpHeader_Create),
    DECLARE_SYM(HttpHeader_Destroy),
    DECLARE_SYM(HttpHeader_name_get),
    DECLARE_SYM(HttpHeader_name_set),
    DECLARE_SYM(HttpHeader_value_get),
    DECLARE_SYM(HttpHeader_value_set),
    // CronetUploadDataProvider
    DECLARE_SYM(UploadDataProvider_CreateWith),
    DECLARE_SYM(UploadDataProvider_GetClientContext),
    DECLARE_SYM(UploadDataProvider_SetClientContext),
    DECLARE_SYM(UploadDataProvider_Destroy),
    // CronetUploadDataSink
    DECLARE_SYM(UploadDataSink_Destroy),
    DECLARE_SYM(UploadDataSink_OnReadError),
    DECLARE_SYM(UploadDataSink_OnReadSucceeded),
    DECLARE_SYM(UploadDataSink_OnRewindError),
    DECLARE_SYM(UploadDataSink_OnRewindSucceeded),
    // CronetUrlRequestCallback
    DECLARE_SYM(UrlRequestCallback_CreateWith),
    DECLARE_SYM(UrlRequestCallback_GetClientContext),
    DECLARE_SYM(UrlRequestCallback_SetClientContext),
    DECLARE_SYM(UrlRequestCallback_Destroy),
    // CronetUrlRequestParams
    DECLARE_SYM(UrlRequestParams_Create),
    DECLARE_SYM(UrlRequestParams_Destroy),
    DECLARE_SYM(UrlRequestParams_disable_cache_get),
    DECLARE_SYM(UrlRequestParams_disable_cache_set),
    DECLARE_SYM(UrlRequestParams_http_method_get),
    DECLARE_SYM(UrlRequestParams_http_method_set),
    DECLARE_SYM(UrlRequestParams_priority_get),
    DECLARE_SYM(UrlRequestParams_priority_set),
    DECLARE_SYM(UrlRequestParams_upload_data_provider_executor_set),
    DECLARE_SYM(UrlRequestParams_upload_data_provider_set),
    DECLARE_SYM(UrlRequestParams_request_headers_add),
    DECLARE_SYM(UrlRequestParams_request_headers_at),
    DECLARE_SYM(UrlRequestParams_request_headers_clear),
    DECLARE_SYM(UrlRequestParams_request_headers_size),
    // CronetUrlRequest
    DECLARE_SYM(UrlRequest_Create),
    DECLARE_SYM(UrlRequest_GetClientContext),
    DECLARE_SYM(UrlRequest_SetClientContext),
    DECLARE_SYM(UrlRequest_Destroy),
    DECLARE_SYM(UrlRequest_InitWithParams),
    DECLARE_SYM(UrlRequest_Start),
    DECLARE_SYM(UrlRequest_Cancel),
    DECLARE_SYM(UrlRequest_FollowRedirect),
    DECLARE_SYM(UrlRequest_Read),
    // CronetUrlResponseInfo
    DECLARE_SYM(UrlResponseInfo_Create),
    DECLARE_SYM(UrlResponseInfo_Destroy),
    DECLARE_SYM(UrlResponseInfo_url_get),
    DECLARE_SYM(UrlResponseInfo_url_set),
    DECLARE_SYM(UrlResponseInfo_negotiated_protocol_get),
    DECLARE_SYM(UrlResponseInfo_negotiated_protocol_set),
    DECLARE_SYM(UrlResponseInfo_http_status_text_get),
    DECLARE_SYM(UrlResponseInfo_http_status_text_set),
    DECLARE_SYM(UrlResponseInfo_http_status_code_get),
    DECLARE_SYM(UrlResponseInfo_http_status_code_set),
    DECLARE_SYM(UrlResponseInfo_all_headers_list_add),
    DECLARE_SYM(UrlResponseInfo_all_headers_list_at),
    DECLARE_SYM(UrlResponseInfo_all_headers_list_clear),
    DECLARE_SYM(UrlResponseInfo_all_headers_list_size),
  };

#undef DECLARE_SYM

  int count = sizeof(syms) / sizeof(syms[0]);
  void* func;
  for (int i = 0; i < count; i++) {
    func = dlsym(lib, syms[i].sym_name);
    if (!func) {
      std::string err_msg_str = "dlsym() failed on '";
      err_msg_str += syms[i].sym_name;
      err_msg_str + "'";
      napi_throw_error(env, nullptr, err_msg_str.c_str());
      return nullptr;
    }
    *syms[i].sym_func = func;
  }

  return nullptr;
}

#define CLAZZ_DEFINE_API_DECLTYPE(name) decltype(Cronet_##name)* CronetUtil::api_##name = nullptr

// CronetBuffer
CLAZZ_DEFINE_API_DECLTYPE(Buffer_Create);
CLAZZ_DEFINE_API_DECLTYPE(Buffer_SetClientContext);
CLAZZ_DEFINE_API_DECLTYPE(Buffer_Destroy);
CLAZZ_DEFINE_API_DECLTYPE(Buffer_InitWithAlloc);
CLAZZ_DEFINE_API_DECLTYPE(Buffer_GetSize);
CLAZZ_DEFINE_API_DECLTYPE(Buffer_GetData);
// CronetEngineParams
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_Create);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_Destroy);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_enable_quic_get);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_enable_quic_set);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_enable_http2_get);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_enable_http2_set);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_enable_brotli_get);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_enable_brotli_set);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_user_agent_get);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_user_agent_set);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_storage_path_get);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_storage_path_set);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_experimental_options_get);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_experimental_options_set);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_http_cache_mode_get);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_http_cache_mode_set);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_http_cache_max_size_get);
CLAZZ_DEFINE_API_DECLTYPE(EngineParams_http_cache_max_size_set);
// CronetEngine
CLAZZ_DEFINE_API_DECLTYPE(Engine_Create);
CLAZZ_DEFINE_API_DECLTYPE(Engine_SetClientContext);
CLAZZ_DEFINE_API_DECLTYPE(Engine_Destroy);
CLAZZ_DEFINE_API_DECLTYPE(Engine_GetVersionString);
CLAZZ_DEFINE_API_DECLTYPE(Engine_StartWithParams);
CLAZZ_DEFINE_API_DECLTYPE(Engine_StartNetLogToFile);
CLAZZ_DEFINE_API_DECLTYPE(Engine_StopNetLog);
CLAZZ_DEFINE_API_DECLTYPE(Engine_Shutdown);
// CronetError
CLAZZ_DEFINE_API_DECLTYPE(Error_Create);
CLAZZ_DEFINE_API_DECLTYPE(Error_Destroy);
CLAZZ_DEFINE_API_DECLTYPE(Error_message_get);
CLAZZ_DEFINE_API_DECLTYPE(Error_message_set);
CLAZZ_DEFINE_API_DECLTYPE(Error_error_code_get);
CLAZZ_DEFINE_API_DECLTYPE(Error_error_code_set);
CLAZZ_DEFINE_API_DECLTYPE(Error_internal_error_code_get);
CLAZZ_DEFINE_API_DECLTYPE(Error_internal_error_code_set);
// CronetExecutor
CLAZZ_DEFINE_API_DECLTYPE(Executor_CreateWith);
CLAZZ_DEFINE_API_DECLTYPE(Executor_GetClientContext);
CLAZZ_DEFINE_API_DECLTYPE(Executor_SetClientContext);
CLAZZ_DEFINE_API_DECLTYPE(Executor_Destroy);
CLAZZ_DEFINE_API_DECLTYPE(Runnable_Run);
CLAZZ_DEFINE_API_DECLTYPE(Runnable_Destroy);
// CronetHttpHeader
CLAZZ_DEFINE_API_DECLTYPE(HttpHeader_Create);
CLAZZ_DEFINE_API_DECLTYPE(HttpHeader_Destroy);
CLAZZ_DEFINE_API_DECLTYPE(HttpHeader_name_get);
CLAZZ_DEFINE_API_DECLTYPE(HttpHeader_name_set);
CLAZZ_DEFINE_API_DECLTYPE(HttpHeader_value_get);
CLAZZ_DEFINE_API_DECLTYPE(HttpHeader_value_set);
// CronetUploadDataProvider
CLAZZ_DEFINE_API_DECLTYPE(UploadDataProvider_CreateWith);
CLAZZ_DEFINE_API_DECLTYPE(UploadDataProvider_GetClientContext);
CLAZZ_DEFINE_API_DECLTYPE(UploadDataProvider_SetClientContext);
CLAZZ_DEFINE_API_DECLTYPE(UploadDataProvider_Destroy);
// CronetUploadDataSink
CLAZZ_DEFINE_API_DECLTYPE(UploadDataSink_Destroy);
CLAZZ_DEFINE_API_DECLTYPE(UploadDataSink_OnReadError);
CLAZZ_DEFINE_API_DECLTYPE(UploadDataSink_OnReadSucceeded);
CLAZZ_DEFINE_API_DECLTYPE(UploadDataSink_OnRewindError);
CLAZZ_DEFINE_API_DECLTYPE(UploadDataSink_OnRewindSucceeded);
// CronetUrlRequestCallback
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestCallback_CreateWith);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestCallback_GetClientContext);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestCallback_SetClientContext);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestCallback_Destroy);
// CronetUrlRequestParams
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_Create);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_Destroy);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_disable_cache_get);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_disable_cache_set);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_http_method_get);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_http_method_set);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_priority_get);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_priority_set);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_upload_data_provider_executor_set);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_upload_data_provider_set);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_request_headers_add);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_request_headers_at);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_request_headers_clear);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequestParams_request_headers_size);
// CronetUrlRequest
CLAZZ_DEFINE_API_DECLTYPE(UrlRequest_Create);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequest_GetClientContext);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequest_SetClientContext);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequest_Destroy);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequest_InitWithParams);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequest_Start);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequest_Cancel);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequest_FollowRedirect);
CLAZZ_DEFINE_API_DECLTYPE(UrlRequest_Read);
// CronetUrlResponseInfo
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_Create);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_Destroy);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_url_get);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_url_set);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_negotiated_protocol_get);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_negotiated_protocol_set);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_http_status_text_get);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_http_status_text_set);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_http_status_code_get);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_http_status_code_set);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_all_headers_list_add);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_all_headers_list_at);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_all_headers_list_clear);
CLAZZ_DEFINE_API_DECLTYPE(UrlResponseInfo_all_headers_list_size);
#endif // !LINK_CRONET
