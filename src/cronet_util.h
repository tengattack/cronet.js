#ifndef _CRONET_UTIL_H_
#define _CRONET_UTIL_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#ifdef LINK_CRONET
// CronetBuffer
#define _Cronet_Buffer_Create Cronet_Buffer_Create
#define _Cronet_Buffer_SetClientContext Cronet_Buffer_SetClientContext
#define _Cronet_Buffer_Destroy Cronet_Buffer_Destroy
#define _Cronet_Buffer_InitWithAlloc Cronet_Buffer_InitWithAlloc
#define _Cronet_Buffer_GetSize Cronet_Buffer_GetSize
#define _Cronet_Buffer_GetData Cronet_Buffer_GetData
// CronetEngineParams
#define _Cronet_EngineParams_Create Cronet_EngineParams_Create
#define _Cronet_EngineParams_Destroy Cronet_EngineParams_Destroy
#define _Cronet_EngineParams_enable_quic_get Cronet_EngineParams_enable_quic_get
#define _Cronet_EngineParams_enable_quic_set Cronet_EngineParams_enable_quic_set
#define _Cronet_EngineParams_enable_http2_get Cronet_EngineParams_enable_http2_get
#define _Cronet_EngineParams_enable_http2_set Cronet_EngineParams_enable_http2_set
#define _Cronet_EngineParams_enable_brotli_get Cronet_EngineParams_enable_brotli_get
#define _Cronet_EngineParams_enable_brotli_set Cronet_EngineParams_enable_brotli_set
#define _Cronet_EngineParams_user_agent_get Cronet_EngineParams_user_agent_get
#define _Cronet_EngineParams_user_agent_set Cronet_EngineParams_user_agent_set
#define _Cronet_EngineParams_storage_path_get Cronet_EngineParams_storage_path_get
#define _Cronet_EngineParams_storage_path_set Cronet_EngineParams_storage_path_set
#define _Cronet_EngineParams_experimental_options_get Cronet_EngineParams_experimental_options_get
#define _Cronet_EngineParams_experimental_options_set Cronet_EngineParams_experimental_options_set
#define _Cronet_EngineParams_http_cache_mode_get Cronet_EngineParams_http_cache_mode_get
#define _Cronet_EngineParams_http_cache_mode_set Cronet_EngineParams_http_cache_mode_set
#define _Cronet_EngineParams_http_cache_max_size_get Cronet_EngineParams_http_cache_max_size_get
#define _Cronet_EngineParams_http_cache_max_size_set Cronet_EngineParams_http_cache_max_size_set
// CronetEngine
#define _Cronet_Engine_Create Cronet_Engine_Create
#define _Cronet_Engine_SetClientContext Cronet_Engine_SetClientContext
#define _Cronet_Engine_Destroy Cronet_Engine_Destroy
#define _Cronet_Engine_GetVersionString Cronet_Engine_GetVersionString
#define _Cronet_Engine_StartWithParams Cronet_Engine_StartWithParams
#define _Cronet_Engine_StartNetLogToFile Cronet_Engine_StartNetLogToFile
#define _Cronet_Engine_StopNetLog Cronet_Engine_StopNetLog
#define _Cronet_Engine_Shutdown Cronet_Engine_Shutdown
// CronetError
#define _Cronet_Error_Create Cronet_Error_Create
#define _Cronet_Error_Destroy Cronet_Error_Destroy
#define _Cronet_Error_message_get Cronet_Error_message_get
#define _Cronet_Error_message_set Cronet_Error_message_set
#define _Cronet_Error_error_code_get Cronet_Error_error_code_get
#define _Cronet_Error_error_code_set Cronet_Error_error_code_set
#define _Cronet_Error_internal_error_code_get Cronet_Error_internal_error_code_get
#define _Cronet_Error_internal_error_code_set Cronet_Error_internal_error_code_set
// CronetExecutor
#define _Cronet_Executor_CreateWith Cronet_Executor_CreateWith
#define _Cronet_Executor_GetClientContext Cronet_Executor_GetClientContext
#define _Cronet_Executor_SetClientContext Cronet_Executor_SetClientContext
#define _Cronet_Executor_Destroy Cronet_Executor_Destroy
#define _Cronet_Runnable_Run Cronet_Runnable_Run
#define _Cronet_Runnable_Destroy Cronet_Runnable_Destroy
// CronetHttpHeader
#define _Cronet_HttpHeader_Create Cronet_HttpHeader_Create
#define _Cronet_HttpHeader_Destroy Cronet_HttpHeader_Destroy
#define _Cronet_HttpHeader_name_get Cronet_HttpHeader_name_get
#define _Cronet_HttpHeader_name_set Cronet_HttpHeader_name_set
#define _Cronet_HttpHeader_value_get Cronet_HttpHeader_value_get
#define _Cronet_HttpHeader_value_set Cronet_HttpHeader_value_set
// CronetUploadDataProvider
#define _Cronet_UploadDataProvider_CreateWith Cronet_UploadDataProvider_CreateWith
#define _Cronet_UploadDataProvider_GetClientContext Cronet_UploadDataProvider_GetClientContext
#define _Cronet_UploadDataProvider_SetClientContext Cronet_UploadDataProvider_SetClientContext
#define _Cronet_UploadDataProvider_Destroy Cronet_UploadDataProvider_Destroy
// CronetUploadDataSink
#define _Cronet_UploadDataSink_Destroy Cronet_UploadDataSink_Destroy
#define _Cronet_UploadDataSink_OnReadError Cronet_UploadDataSink_OnReadError
#define _Cronet_UploadDataSink_OnReadSucceeded Cronet_UploadDataSink_OnReadSucceeded
#define _Cronet_UploadDataSink_OnRewindError Cronet_UploadDataSink_OnRewindError
#define _Cronet_UploadDataSink_OnRewindSucceeded Cronet_UploadDataSink_OnRewindSucceeded
// CronetUrlRequestCallback
#define _Cronet_UrlRequestCallback_CreateWith Cronet_UrlRequestCallback_CreateWith
#define _Cronet_UrlRequestCallback_GetClientContext Cronet_UrlRequestCallback_GetClientContext
#define _Cronet_UrlRequestCallback_SetClientContext Cronet_UrlRequestCallback_SetClientContext
#define _Cronet_UrlRequestCallback_Destroy Cronet_UrlRequestCallback_Destroy
// CronetUrlRequestParams
#define _Cronet_UrlRequestParams_Create Cronet_UrlRequestParams_Create
#define _Cronet_UrlRequestParams_Destroy Cronet_UrlRequestParams_Destroy
#define _Cronet_UrlRequestParams_disable_cache_get Cronet_UrlRequestParams_disable_cache_get
#define _Cronet_UrlRequestParams_disable_cache_set Cronet_UrlRequestParams_disable_cache_set
#define _Cronet_UrlRequestParams_http_method_get Cronet_UrlRequestParams_http_method_get
#define _Cronet_UrlRequestParams_http_method_set Cronet_UrlRequestParams_http_method_set
#define _Cronet_UrlRequestParams_priority_get Cronet_UrlRequestParams_priority_get
#define _Cronet_UrlRequestParams_priority_set Cronet_UrlRequestParams_priority_set
#define _Cronet_UrlRequestParams_upload_data_provider_executor_set Cronet_UrlRequestParams_upload_data_provider_executor_set
#define _Cronet_UrlRequestParams_upload_data_provider_set Cronet_UrlRequestParams_upload_data_provider_set
#define _Cronet_UrlRequestParams_request_headers_add Cronet_UrlRequestParams_request_headers_add
#define _Cronet_UrlRequestParams_request_headers_at Cronet_UrlRequestParams_request_headers_at
#define _Cronet_UrlRequestParams_request_headers_clear Cronet_UrlRequestParams_request_headers_clear
#define _Cronet_UrlRequestParams_request_headers_size Cronet_UrlRequestParams_request_headers_size
// CronetUrlRequest
#define _Cronet_UrlRequest_Create Cronet_UrlRequest_Create
#define _Cronet_UrlRequest_GetClientContext Cronet_UrlRequest_GetClientContext
#define _Cronet_UrlRequest_SetClientContext Cronet_UrlRequest_SetClientContext
#define _Cronet_UrlRequest_Destroy Cronet_UrlRequest_Destroy
#define _Cronet_UrlRequest_InitWithParams Cronet_UrlRequest_InitWithParams
#define _Cronet_UrlRequest_Start Cronet_UrlRequest_Start
#define _Cronet_UrlRequest_Cancel Cronet_UrlRequest_Cancel
#define _Cronet_UrlRequest_FollowRedirect Cronet_UrlRequest_FollowRedirect
#define _Cronet_UrlRequest_Read Cronet_UrlRequest_Read
// CronetUrlResponseInfo
#define _Cronet_UrlResponseInfo_Create Cronet_UrlResponseInfo_Create
#define _Cronet_UrlResponseInfo_Destroy Cronet_UrlResponseInfo_Destroy
#define _Cronet_UrlResponseInfo_url_get Cronet_UrlResponseInfo_url_get
#define _Cronet_UrlResponseInfo_url_set Cronet_UrlResponseInfo_url_set
#define _Cronet_UrlResponseInfo_negotiated_protocol_get Cronet_UrlResponseInfo_negotiated_protocol_get
#define _Cronet_UrlResponseInfo_negotiated_protocol_set Cronet_UrlResponseInfo_negotiated_protocol_set
#define _Cronet_UrlResponseInfo_http_status_text_get Cronet_UrlResponseInfo_http_status_text_get
#define _Cronet_UrlResponseInfo_http_status_text_set Cronet_UrlResponseInfo_http_status_text_set
#define _Cronet_UrlResponseInfo_http_status_code_get Cronet_UrlResponseInfo_http_status_code_get
#define _Cronet_UrlResponseInfo_http_status_code_set Cronet_UrlResponseInfo_http_status_code_set
#define _Cronet_UrlResponseInfo_all_headers_list_add Cronet_UrlResponseInfo_all_headers_list_add
#define _Cronet_UrlResponseInfo_all_headers_list_at Cronet_UrlResponseInfo_all_headers_list_at
#define _Cronet_UrlResponseInfo_all_headers_list_clear Cronet_UrlResponseInfo_all_headers_list_clear
#define _Cronet_UrlResponseInfo_all_headers_list_size Cronet_UrlResponseInfo_all_headers_list_size
#else // !LINK_CRONET
// CronetBuffer
#define _Cronet_Buffer_Create CronetUtil::api_Buffer_Create
#define _Cronet_Buffer_SetClientContext CronetUtil::api_Buffer_SetClientContext
#define _Cronet_Buffer_Destroy CronetUtil::api_Buffer_Destroy
#define _Cronet_Buffer_InitWithAlloc CronetUtil::api_Buffer_InitWithAlloc
#define _Cronet_Buffer_GetSize CronetUtil::api_Buffer_GetSize
#define _Cronet_Buffer_GetData CronetUtil::api_Buffer_GetData
// CronetEngineParams
#define _Cronet_EngineParams_Create CronetUtil::api_EngineParams_Create
#define _Cronet_EngineParams_Destroy CronetUtil::api_EngineParams_Destroy
#define _Cronet_EngineParams_enable_quic_get CronetUtil::api_EngineParams_enable_quic_get
#define _Cronet_EngineParams_enable_quic_set CronetUtil::api_EngineParams_enable_quic_set
#define _Cronet_EngineParams_enable_http2_get CronetUtil::api_EngineParams_enable_http2_get
#define _Cronet_EngineParams_enable_http2_set CronetUtil::api_EngineParams_enable_http2_set
#define _Cronet_EngineParams_enable_brotli_get CronetUtil::api_EngineParams_enable_brotli_get
#define _Cronet_EngineParams_enable_brotli_set CronetUtil::api_EngineParams_enable_brotli_set
#define _Cronet_EngineParams_user_agent_get CronetUtil::api_EngineParams_user_agent_get
#define _Cronet_EngineParams_user_agent_set CronetUtil::api_EngineParams_user_agent_set
#define _Cronet_EngineParams_storage_path_get CronetUtil::api_EngineParams_storage_path_get
#define _Cronet_EngineParams_storage_path_set CronetUtil::api_EngineParams_storage_path_set
#define _Cronet_EngineParams_experimental_options_get CronetUtil::api_EngineParams_experimental_options_get
#define _Cronet_EngineParams_experimental_options_set CronetUtil::api_EngineParams_experimental_options_set
#define _Cronet_EngineParams_http_cache_mode_get CronetUtil::api_EngineParams_http_cache_mode_get
#define _Cronet_EngineParams_http_cache_mode_set CronetUtil::api_EngineParams_http_cache_mode_set
#define _Cronet_EngineParams_http_cache_max_size_get CronetUtil::api_EngineParams_http_cache_max_size_get
#define _Cronet_EngineParams_http_cache_max_size_set CronetUtil::api_EngineParams_http_cache_max_size_set
// CronetEngine
#define _Cronet_Engine_Create CronetUtil::api_Engine_Create
#define _Cronet_Engine_SetClientContext CronetUtil::api_Engine_SetClientContext
#define _Cronet_Engine_Destroy CronetUtil::api_Engine_Destroy
#define _Cronet_Engine_GetVersionString CronetUtil::api_Engine_GetVersionString
#define _Cronet_Engine_StartWithParams CronetUtil::api_Engine_StartWithParams
#define _Cronet_Engine_StartNetLogToFile CronetUtil::api_Engine_StartNetLogToFile
#define _Cronet_Engine_StopNetLog CronetUtil::api_Engine_StopNetLog
#define _Cronet_Engine_Shutdown CronetUtil::api_Engine_Shutdown
// CronetError
#define _Cronet_Error_Create CronetUtil::api_Error_Create
#define _Cronet_Error_Destroy CronetUtil::api_Error_Destroy
#define _Cronet_Error_message_get CronetUtil::api_Error_message_get
#define _Cronet_Error_message_set CronetUtil::api_Error_message_set
#define _Cronet_Error_error_code_get CronetUtil::api_Error_error_code_get
#define _Cronet_Error_error_code_set CronetUtil::api_Error_error_code_set
#define _Cronet_Error_internal_error_code_get CronetUtil::api_Error_internal_error_code_get
#define _Cronet_Error_internal_error_code_set CronetUtil::api_Error_internal_error_code_set
// CronetExecutor
#define _Cronet_Executor_CreateWith CronetUtil::api_Executor_CreateWith
#define _Cronet_Executor_GetClientContext CronetUtil::api_Executor_GetClientContext
#define _Cronet_Executor_SetClientContext CronetUtil::api_Executor_SetClientContext
#define _Cronet_Executor_Destroy CronetUtil::api_Executor_Destroy
#define _Cronet_Runnable_Run CronetUtil::api_Runnable_Run
#define _Cronet_Runnable_Destroy CronetUtil::api_Runnable_Destroy
// CronetHttpHeader
#define _Cronet_HttpHeader_Create CronetUtil::api_HttpHeader_Create
#define _Cronet_HttpHeader_Destroy CronetUtil::api_HttpHeader_Destroy
#define _Cronet_HttpHeader_name_get CronetUtil::api_HttpHeader_name_get
#define _Cronet_HttpHeader_name_set CronetUtil::api_HttpHeader_name_set
#define _Cronet_HttpHeader_value_get CronetUtil::api_HttpHeader_value_get
#define _Cronet_HttpHeader_value_set CronetUtil::api_HttpHeader_value_set
// CronetUploadDataProvider
#define _Cronet_UploadDataProvider_CreateWith CronetUtil::api_UploadDataProvider_CreateWith
#define _Cronet_UploadDataProvider_GetClientContext CronetUtil::api_UploadDataProvider_GetClientContext
#define _Cronet_UploadDataProvider_SetClientContext CronetUtil::api_UploadDataProvider_SetClientContext
#define _Cronet_UploadDataProvider_Destroy CronetUtil::api_UploadDataProvider_Destroy
// CronetUploadDataSink
#define _Cronet_UploadDataSink_Destroy CronetUtil::api_UploadDataSink_Destroy
#define _Cronet_UploadDataSink_OnReadError CronetUtil::api_UploadDataSink_OnReadError
#define _Cronet_UploadDataSink_OnReadSucceeded CronetUtil::api_UploadDataSink_OnReadSucceeded
#define _Cronet_UploadDataSink_OnRewindError CronetUtil::api_UploadDataSink_OnRewindError
#define _Cronet_UploadDataSink_OnRewindSucceeded CronetUtil::api_UploadDataSink_OnRewindSucceeded
// CronetUrlRequestCallback
#define _Cronet_UrlRequestCallback_CreateWith CronetUtil::api_UrlRequestCallback_CreateWith
#define _Cronet_UrlRequestCallback_GetClientContext CronetUtil::api_UrlRequestCallback_GetClientContext
#define _Cronet_UrlRequestCallback_SetClientContext CronetUtil::api_UrlRequestCallback_SetClientContext
#define _Cronet_UrlRequestCallback_Destroy CronetUtil::api_UrlRequestCallback_Destroy
// CronetUrlRequestParams
#define _Cronet_UrlRequestParams_Create CronetUtil::api_UrlRequestParams_Create
#define _Cronet_UrlRequestParams_Destroy CronetUtil::api_UrlRequestParams_Destroy
#define _Cronet_UrlRequestParams_disable_cache_get CronetUtil::api_UrlRequestParams_disable_cache_get
#define _Cronet_UrlRequestParams_disable_cache_set CronetUtil::api_UrlRequestParams_disable_cache_set
#define _Cronet_UrlRequestParams_http_method_get CronetUtil::api_UrlRequestParams_http_method_get
#define _Cronet_UrlRequestParams_http_method_set CronetUtil::api_UrlRequestParams_http_method_set
#define _Cronet_UrlRequestParams_priority_get CronetUtil::api_UrlRequestParams_priority_get
#define _Cronet_UrlRequestParams_priority_set CronetUtil::api_UrlRequestParams_priority_set
#define _Cronet_UrlRequestParams_upload_data_provider_executor_set CronetUtil::api_UrlRequestParams_upload_data_provider_executor_set
#define _Cronet_UrlRequestParams_upload_data_provider_set CronetUtil::api_UrlRequestParams_upload_data_provider_set
#define _Cronet_UrlRequestParams_request_headers_add CronetUtil::api_UrlRequestParams_request_headers_add
#define _Cronet_UrlRequestParams_request_headers_at CronetUtil::api_UrlRequestParams_request_headers_at
#define _Cronet_UrlRequestParams_request_headers_clear CronetUtil::api_UrlRequestParams_request_headers_clear
#define _Cronet_UrlRequestParams_request_headers_size CronetUtil::api_UrlRequestParams_request_headers_size
// CronetUrlRequest
#define _Cronet_UrlRequest_Create CronetUtil::api_UrlRequest_Create
#define _Cronet_UrlRequest_GetClientContext CronetUtil::api_UrlRequest_GetClientContext
#define _Cronet_UrlRequest_SetClientContext CronetUtil::api_UrlRequest_SetClientContext
#define _Cronet_UrlRequest_Destroy CronetUtil::api_UrlRequest_Destroy
#define _Cronet_UrlRequest_InitWithParams CronetUtil::api_UrlRequest_InitWithParams
#define _Cronet_UrlRequest_Start CronetUtil::api_UrlRequest_Start
#define _Cronet_UrlRequest_Cancel CronetUtil::api_UrlRequest_Cancel
#define _Cronet_UrlRequest_FollowRedirect CronetUtil::api_UrlRequest_FollowRedirect
#define _Cronet_UrlRequest_Read CronetUtil::api_UrlRequest_Read
// CronetUrlResponseInfo
#define _Cronet_UrlResponseInfo_Create CronetUtil::api_UrlResponseInfo_Create
#define _Cronet_UrlResponseInfo_Destroy CronetUtil::api_UrlResponseInfo_Destroy
#define _Cronet_UrlResponseInfo_url_get CronetUtil::api_UrlResponseInfo_url_get
#define _Cronet_UrlResponseInfo_url_set CronetUtil::api_UrlResponseInfo_url_set
#define _Cronet_UrlResponseInfo_negotiated_protocol_get CronetUtil::api_UrlResponseInfo_negotiated_protocol_get
#define _Cronet_UrlResponseInfo_negotiated_protocol_set CronetUtil::api_UrlResponseInfo_negotiated_protocol_set
#define _Cronet_UrlResponseInfo_http_status_text_get CronetUtil::api_UrlResponseInfo_http_status_text_get
#define _Cronet_UrlResponseInfo_http_status_text_set CronetUtil::api_UrlResponseInfo_http_status_text_set
#define _Cronet_UrlResponseInfo_http_status_code_get CronetUtil::api_UrlResponseInfo_http_status_code_get
#define _Cronet_UrlResponseInfo_http_status_code_set CronetUtil::api_UrlResponseInfo_http_status_code_set
#define _Cronet_UrlResponseInfo_all_headers_list_add CronetUtil::api_UrlResponseInfo_all_headers_list_add
#define _Cronet_UrlResponseInfo_all_headers_list_at CronetUtil::api_UrlResponseInfo_all_headers_list_at
#define _Cronet_UrlResponseInfo_all_headers_list_clear CronetUtil::api_UrlResponseInfo_all_headers_list_clear
#define _Cronet_UrlResponseInfo_all_headers_list_size CronetUtil::api_UrlResponseInfo_all_headers_list_size
#endif // LINK_CRONET

#define DECLARE_CRONET_CONST(scope, name) \
  DCHECK(napi_create_int32(env, static_cast<int32_t>(scope##_##name), &value)); \
  properties.push_back({ #name, 0, 0, 0, 0, value, napi_enumerable, 0 })

#define DECLARE_CLAZZ_METHOD_NAME(clazz, name, method) _Cronet_##clazz##_##name##method

#define DECLARE_CLAZZ_BOOL_GETTER_SETTER(clazz, name) \
napi_value Cronet##clazz::get_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  napi_value result; \
  DCHECK(napi_get_boolean(env, DECLARE_CLAZZ_METHOD_NAME(clazz, name, _get)(obj->ptr_), &result)); \
  return result; \
} \
napi_value Cronet##clazz::set_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value value; \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_boolean)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  bool result; \
  DCHECK(napi_get_value_bool(env, value, &result)); \
  DECLARE_CLAZZ_METHOD_NAME(clazz, name, _set)(obj->ptr_, result); \
  return nullptr; \
}

#define DECLARE_CLAZZ_STRING_GETTER_SETTER(clazz, name) \
napi_value Cronet##clazz::get_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  napi_value result; \
  DCHECK(napi_create_string_utf8(env, DECLARE_CLAZZ_METHOD_NAME(clazz, name, _get)(obj->ptr_), NAPI_AUTO_LENGTH, &result)); \
  return result; \
} \
napi_value Cronet##clazz::set_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value value; \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_string)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  size_t str_length, copied; \
  DCHECK(napi_get_value_string_utf8(env, value, nullptr, 0, &str_length)); \
  std::string str(str_length, '\0'); \
  DCHECK(napi_get_value_string_utf8(env, value, &str[0], str.length() + 1, &copied)); \
  DECLARE_CLAZZ_METHOD_NAME(clazz, name, _set)(obj->ptr_, str.c_str()); \
  return nullptr; \
}

#define DECLARE_CLAZZ_INT32_GETTER_SETTER(clazz, name, T) \
napi_value Cronet##clazz::get_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  napi_value result; \
  DCHECK(napi_create_int32(env, static_cast<int32_t>(DECLARE_CLAZZ_METHOD_NAME(clazz, name, _get)(obj->ptr_)), &result)); \
  return result; \
} \
napi_value Cronet##clazz::set_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value value; \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_number)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  int32_t result; \
  DCHECK(napi_get_value_int32(env, value, &result)); \
  DECLARE_CLAZZ_METHOD_NAME(clazz, name, _set)(obj->ptr_, (T)result); \
  return nullptr; \
}

#define DECLARE_CLAZZ_INT64_GETTER_SETTER(clazz, name) \
napi_value Cronet##clazz::get_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  napi_value result; \
  DCHECK(napi_create_int64(env, DECLARE_CLAZZ_METHOD_NAME(clazz, name, _get)(obj->ptr_), &result)); \
  return result; \
} \
napi_value Cronet##clazz::set_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value value; \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_number)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  int64_t result; \
  DCHECK(napi_get_value_int64(env, value, &result)); \
  DECLARE_CLAZZ_METHOD_NAME(clazz, name, _set)(obj->ptr_, result); \
  return nullptr; \
}

#define DECLARE_REF_NAME(name, suffix) name##suffix

#define DECLARE_CLAZZ_FUNC_GETTER_SETTER(clazz, name) \
napi_value Cronet##clazz::get_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  napi_value result = nullptr; \
  if (obj->DECLARE_REF_NAME(name, _ref_)) { \
    DCHECK(napi_get_reference_value(env, obj->DECLARE_REF_NAME(name, _ref_), &result)); \
  } \
  return result; \
} \
napi_value Cronet##clazz::set_##name(napi_env env, napi_callback_info info) { \
  napi_status status; \
  \
  napi_value value; \
  napi_value jsthis; \
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_function)) { \
    return nullptr; \
  } \
  \
  Cronet##clazz* obj; \
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj))); \
  \
  if (obj->DECLARE_REF_NAME(name, _ref_)) { \
    DCHECK(napi_delete_reference(env, obj->DECLARE_REF_NAME(name, _ref_))); \
    obj->DECLARE_REF_NAME(name, _ref_) = nullptr; \
  } \
  DCHECK(napi_create_reference(env, value, 1, &obj->DECLARE_REF_NAME(name, _ref_))); \
  return nullptr; \
}

class CronetUtil {
 public:
  static bool ValidateAndGetCbInfo(napi_env env, napi_callback_info info, napi_value* this_arg);
  static bool ValidateAndGetCbInfo(napi_env env, napi_callback_info info, napi_value* this_arg, napi_value* arg, napi_valuetype arg_type, napi_ref arg_cons_ref = nullptr);
  static bool ValidateAndGetCbInfo(napi_env env, napi_callback_info info, napi_value* this_arg, size_t argc_expected, napi_value *argv, napi_valuetype *arg_types);

  static void ThrowInvalidArgumentError(napi_env env);
  static void ThrowCronetResultError(napi_env env, Cronet_RESULT result);

#ifndef LINK_CRONET
  // cronet apis
  static napi_value LoadLibrary(napi_env env, napi_callback_info info);

#define STATIC_DEFINE_API_DECLTYPE(name) static decltype(Cronet_##name)* api_##name
  // CronetBuffer
  STATIC_DEFINE_API_DECLTYPE(Buffer_Create);
  STATIC_DEFINE_API_DECLTYPE(Buffer_SetClientContext);
  STATIC_DEFINE_API_DECLTYPE(Buffer_Destroy);
  STATIC_DEFINE_API_DECLTYPE(Buffer_InitWithAlloc);
  STATIC_DEFINE_API_DECLTYPE(Buffer_GetSize);
  STATIC_DEFINE_API_DECLTYPE(Buffer_GetData);
  // CronetEngineParams
  STATIC_DEFINE_API_DECLTYPE(EngineParams_Create);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_Destroy);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_enable_quic_get);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_enable_quic_set);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_enable_http2_get);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_enable_http2_set);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_enable_brotli_get);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_enable_brotli_set);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_user_agent_get);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_user_agent_set);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_storage_path_get);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_storage_path_set);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_experimental_options_get);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_experimental_options_set);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_http_cache_mode_get);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_http_cache_mode_set);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_http_cache_max_size_get);
  STATIC_DEFINE_API_DECLTYPE(EngineParams_http_cache_max_size_set);
  // CronetEngine
  STATIC_DEFINE_API_DECLTYPE(Engine_Create);
  STATIC_DEFINE_API_DECLTYPE(Engine_SetClientContext);
  STATIC_DEFINE_API_DECLTYPE(Engine_Destroy);
  STATIC_DEFINE_API_DECLTYPE(Engine_GetVersionString);
  STATIC_DEFINE_API_DECLTYPE(Engine_StartWithParams);
  STATIC_DEFINE_API_DECLTYPE(Engine_StartNetLogToFile);
  STATIC_DEFINE_API_DECLTYPE(Engine_StopNetLog);
  STATIC_DEFINE_API_DECLTYPE(Engine_Shutdown);
  // CronetError
  STATIC_DEFINE_API_DECLTYPE(Error_Create);
  STATIC_DEFINE_API_DECLTYPE(Error_Destroy);
  STATIC_DEFINE_API_DECLTYPE(Error_message_get);
  STATIC_DEFINE_API_DECLTYPE(Error_message_set);
  STATIC_DEFINE_API_DECLTYPE(Error_error_code_get);
  STATIC_DEFINE_API_DECLTYPE(Error_error_code_set);
  STATIC_DEFINE_API_DECLTYPE(Error_internal_error_code_get);
  STATIC_DEFINE_API_DECLTYPE(Error_internal_error_code_set);
  // CronetExecutor
  STATIC_DEFINE_API_DECLTYPE(Executor_CreateWith);
  STATIC_DEFINE_API_DECLTYPE(Executor_GetClientContext);
  STATIC_DEFINE_API_DECLTYPE(Executor_SetClientContext);
  STATIC_DEFINE_API_DECLTYPE(Executor_Destroy);
  STATIC_DEFINE_API_DECLTYPE(Runnable_Run);
  STATIC_DEFINE_API_DECLTYPE(Runnable_Destroy);
  // CronetHttpHeader
  STATIC_DEFINE_API_DECLTYPE(HttpHeader_Create);
  STATIC_DEFINE_API_DECLTYPE(HttpHeader_Destroy);
  STATIC_DEFINE_API_DECLTYPE(HttpHeader_name_get);
  STATIC_DEFINE_API_DECLTYPE(HttpHeader_name_set);
  STATIC_DEFINE_API_DECLTYPE(HttpHeader_value_get);
  STATIC_DEFINE_API_DECLTYPE(HttpHeader_value_set);
  // CronetUploadDataProvider
  STATIC_DEFINE_API_DECLTYPE(UploadDataProvider_CreateWith);
  STATIC_DEFINE_API_DECLTYPE(UploadDataProvider_GetClientContext);
  STATIC_DEFINE_API_DECLTYPE(UploadDataProvider_SetClientContext);
  STATIC_DEFINE_API_DECLTYPE(UploadDataProvider_Destroy);
  // CronetUploadDataSink
  STATIC_DEFINE_API_DECLTYPE(UploadDataSink_Destroy);
  STATIC_DEFINE_API_DECLTYPE(UploadDataSink_OnReadError);
  STATIC_DEFINE_API_DECLTYPE(UploadDataSink_OnReadSucceeded);
  STATIC_DEFINE_API_DECLTYPE(UploadDataSink_OnRewindError);
  STATIC_DEFINE_API_DECLTYPE(UploadDataSink_OnRewindSucceeded);
  // CronetUrlRequestCallback
  STATIC_DEFINE_API_DECLTYPE(UrlRequestCallback_CreateWith);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestCallback_GetClientContext);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestCallback_SetClientContext);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestCallback_Destroy);
  // CronetUrlRequestParams
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_Create);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_Destroy);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_disable_cache_get);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_disable_cache_set);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_http_method_get);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_http_method_set);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_priority_get);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_priority_set);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_upload_data_provider_executor_set);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_upload_data_provider_set);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_request_headers_add);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_request_headers_at);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_request_headers_clear);
  STATIC_DEFINE_API_DECLTYPE(UrlRequestParams_request_headers_size);
  // CronetUrlRequest
  STATIC_DEFINE_API_DECLTYPE(UrlRequest_Create);
  STATIC_DEFINE_API_DECLTYPE(UrlRequest_GetClientContext);
  STATIC_DEFINE_API_DECLTYPE(UrlRequest_SetClientContext);
  STATIC_DEFINE_API_DECLTYPE(UrlRequest_Destroy);
  STATIC_DEFINE_API_DECLTYPE(UrlRequest_InitWithParams);
  STATIC_DEFINE_API_DECLTYPE(UrlRequest_Start);
  STATIC_DEFINE_API_DECLTYPE(UrlRequest_Cancel);
  STATIC_DEFINE_API_DECLTYPE(UrlRequest_FollowRedirect);
  STATIC_DEFINE_API_DECLTYPE(UrlRequest_Read);
  // CronetUrlResponseInfo
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_Create);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_Destroy);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_url_get);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_url_set);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_negotiated_protocol_get);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_negotiated_protocol_set);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_http_status_text_get);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_http_status_text_set);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_http_status_code_get);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_http_status_code_set);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_all_headers_list_add);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_all_headers_list_at);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_all_headers_list_clear);
  STATIC_DEFINE_API_DECLTYPE(UrlResponseInfo_all_headers_list_size);
#endif // !LINK_CRONET
};

#endif // _CRONET_UTIL_H_
