
#include "cronet_util.h"

#include <string>

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

bool CronetUtil::ValidateAndGetCbInfo(napi_env env, napi_callback_info info, napi_value* this_arg, size_t argc_expected, napi_value *argv, napi_valuetype *arg_types) {
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
