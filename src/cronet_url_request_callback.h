#ifndef _CRONET_URL_REQUEST_CALLBACK_H_
#define _CRONET_URL_REQUEST_CALLBACK_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetExecutor;

class CronetUrlRequestCallback : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetUrlRequestCallback)

 public:
  Cronet_UrlRequestCallbackPtr ptr() { return ptr_; }

  void SetExecutor(CronetExecutor* executor);

 private:
  explicit CronetUrlRequestCallback();
  ~CronetUrlRequestCallback();

  DECLARE_NAPI_MAGIC_METHOD_HEADER(on_redirect_received)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(on_response_started)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(on_read_completed)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(on_succeeded)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(on_failed)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(on_canceled)

  Cronet_UrlRequestCallbackPtr ptr_;
  // Not owned, but has ref.
  CronetExecutor* executor_;

  napi_ref on_redirect_received_ref_;
  napi_ref on_response_started_ref_;
  napi_ref on_read_completed_ref_;
  napi_ref on_succeeded_ref_;
  napi_ref on_failed_ref_;
  napi_ref on_canceled_ref_;

 protected:
  void OnRedirectReceived(napi_env env,
                          Cronet_UrlRequestPtr request,
                          Cronet_UrlResponseInfoPtr info,
                          Cronet_String newLocationUrl);

  void OnResponseStarted(napi_env env,
                         Cronet_UrlRequestPtr request,
                         Cronet_UrlResponseInfoPtr info);

  void OnReadCompleted(napi_env env,
                       Cronet_UrlRequestPtr request,
                       Cronet_UrlResponseInfoPtr info,
                       Cronet_BufferPtr buffer,
                       uint64_t bytes_read);

  void OnSucceeded(napi_env env,
                   Cronet_UrlRequestPtr request,
                   Cronet_UrlResponseInfoPtr info);

  void OnFailed(napi_env env,
                Cronet_UrlRequestPtr request,
                Cronet_UrlResponseInfoPtr info,
                Cronet_ErrorPtr error);

  void OnCanceled(napi_env env,
                  Cronet_UrlRequestPtr request,
                  Cronet_UrlResponseInfoPtr info);

  void Done(napi_env env, Cronet_UrlRequestPtr request);

  static CronetUrlRequestCallback* GetThis(Cronet_UrlRequestCallbackPtr self);

  // Implementation of Cronet_UrlRequestCallback methods.
  static void OnRedirectReceived(Cronet_UrlRequestCallbackPtr self,
                                 Cronet_UrlRequestPtr request,
                                 Cronet_UrlResponseInfoPtr info,
                                 Cronet_String newLocationUrl);

  static void OnResponseStarted(Cronet_UrlRequestCallbackPtr self,
                                Cronet_UrlRequestPtr request,
                                Cronet_UrlResponseInfoPtr info);

  static void OnReadCompleted(Cronet_UrlRequestCallbackPtr self,
                              Cronet_UrlRequestPtr request,
                              Cronet_UrlResponseInfoPtr info,
                              Cronet_BufferPtr buffer,
                              uint64_t bytesRead);

  static void OnSucceeded(Cronet_UrlRequestCallbackPtr self,
                          Cronet_UrlRequestPtr request,
                          Cronet_UrlResponseInfoPtr info);

  static void OnFailed(Cronet_UrlRequestCallbackPtr self,
                       Cronet_UrlRequestPtr request,
                       Cronet_UrlResponseInfoPtr info,
                       Cronet_ErrorPtr error);

  static void OnCanceled(Cronet_UrlRequestCallbackPtr self,
                         Cronet_UrlRequestPtr request,
                         Cronet_UrlResponseInfoPtr info);
};

#endif // _CRONET_URL_REQUEST_CALLBACK_H_
