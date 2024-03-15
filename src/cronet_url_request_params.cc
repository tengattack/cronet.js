
#include "cronet_url_request_params.h"

#include <string>
#include <vector>

#include <cronet_c.h>

#include "addon_node.h"
#include "cronet_executor.h"
#include "cronet_headers.h"
#include "cronet_upload_data_provider.h"
#include "cronet_util.h"

CronetUrlRequestParams::CronetUrlRequestParams()
    : ptr_(_Cronet_UrlRequestParams_Create())
    , upload_data_provider_executor_(nullptr)
    , upload_data_provider_(nullptr)
    , upload_data_provider_executor_ref_(nullptr)
    , upload_data_provider_ref_(nullptr) {
}

CronetUrlRequestParams::~CronetUrlRequestParams() {
  TRACE("~CronetUrlRequestParams()\n");
  _Cronet_UrlRequestParams_Destroy(ptr_);
  ptr_ = nullptr;
  if (upload_data_provider_executor_ref_) {
    napi_delete_reference(env_, upload_data_provider_executor_ref_);
  }
  if (upload_data_provider_ref_) {
    napi_delete_reference(env_, upload_data_provider_ref_);
  }
}

DECLARE_CLAZZ_LIFECYCLE_BASE(CronetUrlRequestParams)

static napi_value RegisterCronetUrlRequestParamsRequestPriority(napi_env env, napi_value exports) {
  napi_status status;
  napi_value obj;
  napi_value value;
  std::vector<napi_property_descriptor> properties;

  DECLARE_CRONET_CONST(Cronet_UrlRequestParams_REQUEST_PRIORITY, REQUEST_PRIORITY_IDLE);
  DECLARE_CRONET_CONST(Cronet_UrlRequestParams_REQUEST_PRIORITY, REQUEST_PRIORITY_LOWEST);
  DECLARE_CRONET_CONST(Cronet_UrlRequestParams_REQUEST_PRIORITY, REQUEST_PRIORITY_LOW);
  DECLARE_CRONET_CONST(Cronet_UrlRequestParams_REQUEST_PRIORITY, REQUEST_PRIORITY_MEDIUM);
  DECLARE_CRONET_CONST(Cronet_UrlRequestParams_REQUEST_PRIORITY, REQUEST_PRIORITY_HIGHEST);

  NODE_API_CALL(env, napi_create_object(env, &obj));
  NODE_API_CALL(env, napi_define_properties(env, obj, properties.size(), &properties[0]));
  NODE_API_CALL(env, napi_set_named_property(env, exports, "REQUEST_PRIORITY", obj));
  return nullptr;
}

napi_value CronetUrlRequestParams::Register(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
    DECLARE_NAPI_MAGIC_METHOD("disableCache", disable_cache),

    DECLARE_NAPI_MAGIC_METHOD("httpMethod", http_method),

    DECLARE_NAPI_MAGIC_METHOD("priority", priority),

    DECLARE_CLAZZ_CRONET_HEADERS_METHOD(CronetUrlRequestParams, requestHeaders),

    DECLARE_NAPI_MAGIC_METHOD("uploadDataProviderExecutor", upload_data_provider_executor),
    DECLARE_NAPI_MAGIC_METHOD("uploadDataProvider", upload_data_provider),
  };

  napi_value cons;
  NODE_API_CALL(env, napi_define_class(
      env, "CronetUrlRequestParams", NAPI_AUTO_LENGTH, New, nullptr,
      sizeof(properties) / sizeof(properties[0]), properties, &cons));
  RegisterCronetUrlRequestParamsRequestPriority(env, cons);

  ref = RegisterClass(env, "CronetUrlRequestParams", cons, exports);
  return exports;
}

napi_value CronetUrlRequestParams::New(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value target;
  NODE_API_CALL(env, napi_get_new_target(env, info, &target));
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    TRACE("new CronetUrlRequestParams(...)\n");
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));

    // napi_valuetype valuetype;
    // status = napi_typeof(env, args[0], &valuetype);
    // assert(status == napi_ok);

    // if (valuetype != napi_undefined) {
    //   status = napi_get_value_double(env, args[0], &value);
    //   assert(status == napi_ok);
    // }

    CronetUrlRequestParams* obj = new CronetUrlRequestParams();

    obj->env_ = env;
    NODE_API_CALL(env, napi_wrap(env,
                                 jsthis,
                                 reinterpret_cast<void*>(obj),
                                 CronetUrlRequestParams::Destructor,
                                 nullptr,  // finalize_hint
                                 &obj->wrapper_));

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    TRACE("CronetUrlRequestParams(...)\n");
    size_t argc_ = 1;
    napi_value args[1];
    NODE_API_CALL(env, napi_get_cb_info(env, info, &argc_, args, nullptr, nullptr));

    const size_t argc = 1;
    napi_value argv[argc] = {args[0]};

    napi_value instance;
    NODE_API_CALL(env, napi_new_instance(env, Constructor(env), argc, argv, &instance));

    return instance;
  }
}

DECLARE_CLAZZ_BOOL_GETTER_SETTER(UrlRequestParams, disable_cache)

DECLARE_CLAZZ_STRING_GETTER_SETTER(UrlRequestParams, http_method)

DECLARE_CLAZZ_INT32_GETTER_SETTER(UrlRequestParams, priority, Cronet_UrlRequestParams_REQUEST_PRIORITY)

napi_value CronetUrlRequestParams::get_upload_data_provider_executor(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  DCHECK(napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr));

  CronetUrlRequestParams* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  napi_value result = nullptr;
  if (obj->upload_data_provider_executor_) {
    DCHECK(napi_get_reference_value(env, obj->upload_data_provider_executor_ref_, &result));
  }
  return result;
}

napi_value CronetUrlRequestParams::set_upload_data_provider_executor(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[argc];
  napi_value jsthis;
  DCHECK(napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));

  CronetUrlRequestParams* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  napi_valuetype valuetype;
  DCHECK(napi_typeof(env, args[0], &valuetype));
  if (valuetype == napi_null || valuetype == napi_undefined) {
    if (obj->upload_data_provider_executor_ref_) {
      DCHECK(napi_delete_reference(env, obj->upload_data_provider_executor_ref_));
      obj->upload_data_provider_executor_ref_ = nullptr;
    }
    _Cronet_UrlRequestParams_upload_data_provider_executor_set(obj->ptr_, nullptr);
    obj->upload_data_provider_executor_ = nullptr;
    return nullptr;
  }
  if (valuetype != napi_object) {
    CronetUtil::ThrowInvalidArgumentError(env);
    return nullptr;
  }

  bool isinstanceof;
  napi_value cons;
  DCHECK(napi_get_reference_value(env, *CronetExecutor::ref, &cons));
  DCHECK(napi_instanceof(env, args[0], cons, &isinstanceof));
  TRACE("isinstanceof CronetExecutor: %d\n", isinstanceof ? 1 : 0);
  if (!isinstanceof) {
    CronetUtil::ThrowInvalidArgumentError(env);
    return nullptr;
  }
  CronetExecutor *executor;
  DCHECK(napi_unwrap(env, args[0], reinterpret_cast<void**>(&executor)));

  if (obj->upload_data_provider_executor_ref_) {
    DCHECK(napi_delete_reference(env, obj->upload_data_provider_executor_ref_));
    obj->upload_data_provider_executor_ref_ = nullptr;
  }
  DCHECK(napi_create_reference(env, args[0], 1, &obj->upload_data_provider_executor_ref_));
  _Cronet_UrlRequestParams_upload_data_provider_executor_set(obj->ptr_, executor->ptr());
  obj->upload_data_provider_executor_ = executor;
  return nullptr;
}

napi_value CronetUrlRequestParams::get_upload_data_provider(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  DCHECK(napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr));

  CronetUrlRequestParams* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  napi_value result = nullptr;
  if (obj->upload_data_provider_ref_) {
    DCHECK(napi_get_reference_value(env, obj->upload_data_provider_ref_, &result));
  }
  return result;
}

napi_value CronetUrlRequestParams::set_upload_data_provider(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[argc];
  napi_value jsthis;
  DCHECK(napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr));

  CronetUrlRequestParams* obj;
  DCHECK(napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj)));

  napi_valuetype valuetype;
  DCHECK(napi_typeof(env, args[0], &valuetype));
  if (valuetype == napi_null || valuetype == napi_undefined) {
    if (obj->upload_data_provider_ref_) {
      DCHECK(napi_delete_reference(env, obj->upload_data_provider_ref_));
      obj->upload_data_provider_ref_ = nullptr;
    }
    _Cronet_UrlRequestParams_upload_data_provider_set(obj->ptr_, nullptr);
    obj->upload_data_provider_ = nullptr;
    return nullptr;
  }
  if (valuetype != napi_object) {
    CronetUtil::ThrowInvalidArgumentError(env);
    return nullptr;
  }

  bool isinstanceof;
  napi_value cons;
  DCHECK(napi_get_reference_value(env, *CronetUploadDataProvider::ref, &cons));
  DCHECK(napi_instanceof(env, args[0], cons, &isinstanceof));
  TRACE("isinstanceof CronetUploadDataProvider: %d\n", isinstanceof ? 1 : 0);
  if (!isinstanceof) {
    CronetUtil::ThrowInvalidArgumentError(env);
    return nullptr;
  }
  CronetUploadDataProvider *upload_data_provider;
  DCHECK(napi_unwrap(env, args[0], reinterpret_cast<void**>(&upload_data_provider)));

  if (obj->upload_data_provider_ref_) {
    DCHECK(napi_delete_reference(env, obj->upload_data_provider_ref_));
    obj->upload_data_provider_ref_ = nullptr;
  }
  DCHECK(napi_create_reference(env, args[0], 1, &obj->upload_data_provider_ref_));
  _Cronet_UrlRequestParams_upload_data_provider_set(obj->ptr_, upload_data_provider->ptr());
  obj->upload_data_provider_ = upload_data_provider;
  return nullptr;
}
