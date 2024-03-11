#ifndef _CRONET_UPLOAD_DATA_PROVIDER_H_
#define _CRONET_UPLOAD_DATA_PROVIDER_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

#include "napi_class.h"

class CronetExecutor;

class CronetUploadDataProvider : public NapiClass {
  DECLARE_CLAZZ_LIFECYCLE_HEADER(CronetUploadDataProvider)

 public:
  Cronet_UploadDataProviderPtr ptr() { return ptr_; }

  void SetExecutor(CronetExecutor* executor);

 private:
  explicit CronetUploadDataProvider();
  ~CronetUploadDataProvider();

  DECLARE_NAPI_MAGIC_METHOD_HEADER(get_length)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(read)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(rewind)
  DECLARE_NAPI_MAGIC_METHOD_HEADER(close)

  Cronet_UploadDataProviderPtr ptr_;
  // Not owned, but has ref.
  CronetExecutor* executor_;

  napi_ref get_length_ref_;
  napi_ref read_ref_;
  napi_ref rewind_ref_;
  napi_ref close_ref_;

 protected:
  int64_t GetLength(napi_env env);
  void Read(napi_env env,
            Cronet_UploadDataSinkPtr upload_data_sink,
            Cronet_BufferPtr buffer);
  void Rewind(napi_env env,
              Cronet_UploadDataSinkPtr upload_data_sink);
  void Close(napi_env env);

  static CronetUploadDataProvider* GetThis(Cronet_UploadDataProviderPtr self);

  // Implementation of Cronet_UploadDataProvider methods.
  static int64_t GetLength(Cronet_UploadDataProviderPtr self);
  static void Read(Cronet_UploadDataProviderPtr self,
                   Cronet_UploadDataSinkPtr upload_data_sink,
                   Cronet_BufferPtr buffer);
  static void Rewind(Cronet_UploadDataProviderPtr self,
                     Cronet_UploadDataSinkPtr upload_data_sink);
  static void Close(Cronet_UploadDataProviderPtr self);
};

#endif // _CRONET_UPLOAD_DATA_PROVIDER_H_
