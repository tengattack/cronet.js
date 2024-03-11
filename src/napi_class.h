#ifndef _NAPI_CLASS_H_
#define _NAPI_CLASS_H_

#include <js_native_api_types.h>

#include "addon_node.h"

#define DECLARE_NAPI_MAGIC_METHOD_HEADER(field) \
  static napi_value get_##field(napi_env env, napi_callback_info info); \
  static napi_value set_##field(napi_env env, napi_callback_info info); \

#define DECLARE_NAPI_METHOD(name, func)                           \
  { name, 0, func, 0, 0, 0, napi_default_method, 0 }

#define DECLARE_NAPI_MAGIC_METHOD(name, field)                    \
  { name, 0, 0, get_##field, set_##field, 0, napi_default_jsproperty, 0 }

#define DECLARE_CLAZZ_LIFECYCLE_HEADER(clazz) \
 public: \
  static napi_value Register(napi_env env, napi_value exports); \
  static void Destructor(napi_env env, void* nativeObject, void* finalize_hint); \
  \
  static napi_ref* ref; \
  \
 private: \
  static napi_value New(napi_env env, napi_callback_info info); \
  static inline napi_value Constructor(napi_env env);

#define DECLARE_CLAZZ_LIFECYCLE_BASE(clazz) \
napi_ref* clazz::ref = nullptr; \
\
napi_value clazz::Constructor(napi_env env) { \
  napi_status status; \
  napi_value cons; \
  NODE_API_CALL(env, napi_get_reference_value(env, *ref, &cons)); \
  return cons; \
} \
void clazz::Destructor(napi_env env, \
                       void* nativeObject, \
                       void* /*finalize_hint*/) { \
  TRACE("%s::Destructor()\n", #clazz); \
  /* reinterpret_cast<clazz*>(nativeObject)->~##clazz(); */ \
  delete reinterpret_cast<clazz*>(nativeObject); \
}

class NapiClass {
 public:
  void AddRef();
  void ReleaseRef();

 protected:
  explicit NapiClass();
  ~NapiClass();

  static napi_ref* RegisterClass(napi_env env, const char* class_name, napi_value cons, napi_value exports);
  static void Finalizer(napi_env env, void* data, void* hint);

  static napi_value WrapInternal(napi_env env, napi_ref* ref, void* data);

  napi_env env_;
  napi_ref wrapper_;
};

#endif // _NAPI_CLASS_H_
