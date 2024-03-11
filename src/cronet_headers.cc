
#include "cronet_headers.h"

#include <cronet_c.h>

#include "cronet_http_header.h"
#include "cronet_url_request_params.h"
#include "cronet_url_response_info.h"
#include "cronet_util.h"

/* CronetUrlRequestParams */
template<>
napi_value CronetHeaders<CronetUrlRequestParams>::add(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value value;
  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_object, *CronetHttpHeader::ref)) {
    return nullptr;
  }

  CronetUrlRequestParams* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  CronetHttpHeader *http_header;
  DCHECK(napi_unwrap(env, value, reinterpret_cast<void**>(&http_header)));

  Cronet_UrlRequestParams_request_headers_add(obj->ptr(), http_header->ptr());
  return nullptr;
}

template<>
napi_value CronetHeaders<CronetUrlRequestParams>::at(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value value;
  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_number)) {
    return nullptr;
  }

  CronetUrlRequestParams* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  uint32_t i;
  DCHECK(napi_get_value_uint32(env, value, &i));

  return CronetHttpHeader::WrapUnowned(env, Cronet_UrlRequestParams_request_headers_at(obj->ptr(), i));
}

template<>
napi_value CronetHeaders<CronetUrlRequestParams>::clear(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetUrlRequestParams* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  Cronet_UrlRequestParams_request_headers_clear(obj->ptr());
  return nullptr;
}

template<>
napi_value CronetHeaders<CronetUrlRequestParams>::size(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetUrlRequestParams* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  napi_value result;
  DCHECK(napi_create_uint32(env, Cronet_UrlRequestParams_request_headers_size(obj->ptr()), &result));
  return result;
}


/* CronetUrlResponseInfo */
template<>
napi_value CronetHeaders<CronetUrlResponseInfo>::add(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value value;
  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_object, *CronetHttpHeader::ref)) {
    return nullptr;
  }

  CronetUrlResponseInfo* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  CronetHttpHeader *http_header;
  DCHECK(napi_unwrap(env, value, reinterpret_cast<void**>(&http_header)));

  Cronet_UrlResponseInfo_all_headers_list_add(obj->ptr(), http_header->ptr());
  return nullptr;
}

template<>
napi_value CronetHeaders<CronetUrlResponseInfo>::at(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value value;
  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis, &value, napi_number)) {
    return nullptr;
  }

  CronetUrlResponseInfo* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  uint32_t i;
  DCHECK(napi_get_value_uint32(env, value, &i));

  return CronetHttpHeader::WrapUnowned(env, Cronet_UrlResponseInfo_all_headers_list_at(obj->ptr(), i));
}

template<>
napi_value CronetHeaders<CronetUrlResponseInfo>::clear(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetUrlResponseInfo* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  Cronet_UrlResponseInfo_all_headers_list_clear(obj->ptr());
  return nullptr;
}

template<>
napi_value CronetHeaders<CronetUrlResponseInfo>::size(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  if (!CronetUtil::ValidateAndGetCbInfo(env, info, &jsthis)) {
    return nullptr;
  }

  CronetUrlResponseInfo* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  napi_value result;
  DCHECK(napi_create_uint32(env, Cronet_UrlResponseInfo_all_headers_list_size(obj->ptr()), &result));
  return result;
}
