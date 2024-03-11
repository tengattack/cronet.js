
#include "napi_class.h"

#include "addon_node.h"

NapiClass::NapiClass()
  : env_(nullptr)
  , wrapper_(nullptr) {
}

NapiClass::~NapiClass() {
  napi_delete_reference(env_, wrapper_);
}

napi_ref* NapiClass::RegisterClass(napi_env env, const char* class_name, napi_value cons, napi_value exports) {
  napi_status status;
  // We will need the constructor `cons` later during the life cycle of the
  // addon, so we store a persistent reference as a global static variable.
  // TODO: fix support Node.js worker threads and multiple contexts on a single
  // thread.
  //
  // The finalizer will be called when our addon is unloaded and is responsible
  // for releasing the persistent reference and freeing the heap memory where we
  // stored the persistent reference.
  napi_ref* constructor = new napi_ref;
  DCHECK(napi_create_reference(env, cons, 1, constructor));
  DCHECK(napi_add_finalizer(
      env,
      cons,
      constructor,
      Finalizer,
      (void*)class_name,
      nullptr));

  DCHECK(napi_set_named_property(env, exports, class_name, cons));
  return constructor;
}

void NapiClass::Finalizer(napi_env env, void* data, void* hint) {
  TRACE("%s::Finalizer()\n", (const char*)hint);
  napi_ref* constructor = static_cast<napi_ref*>(data);
  napi_status status;
  DCHECK(napi_delete_reference(env, *constructor));
  delete constructor;
}

napi_value NapiClass::WrapInternal(napi_env env, napi_ref* ref, void* data) {
  napi_status status;

  if (!data) {
    napi_value result;
    DCHECK(napi_get_null(env, &result));
    return result;
  }

  napi_value cons;
  DCHECK(napi_get_reference_value(env, *ref, &cons));

  const size_t argc = 1;
  napi_value argv[argc];
  DCHECK(napi_create_external(env, data, nullptr, nullptr, &argv[0]));

  napi_value instance;
  DCHECK(napi_new_instance(env, cons, argc, argv, &instance));

  return instance;
}

void NapiClass::AddRef() {
  napi_status status;
  DCHECK(napi_reference_ref(env_, wrapper_, nullptr));
}

void NapiClass::ReleaseRef() {
  napi_status status;
  DCHECK(napi_reference_unref(env_, wrapper_, nullptr));
}
