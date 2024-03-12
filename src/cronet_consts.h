#ifndef _CRONET_CONSTS_H_
#define _CRONET_CONSTS_H_

#include <cronet_c.h>
#include <js_native_api_types.h>

class CronetConsts {
 public:
  static napi_value Register(napi_env env, napi_value exports);
};

#endif // _CRONET_CONSTS_H_
