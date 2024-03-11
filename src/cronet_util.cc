
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
  switch (result) {
    case Cronet_RESULT_ILLEGAL_ARGUMENT:
      message = "Cronet_RESULT_ILLEGAL_ARGUMENT";
      break;
    case Cronet_RESULT_ILLEGAL_ARGUMENT_STORAGE_PATH_MUST_EXIST:
      message = "Cronet_RESULT_ILLEGAL_ARGUMENT_STORAGE_PATH_MUST_EXIST";
      break;
    case Cronet_RESULT_ILLEGAL_ARGUMENT_INVALID_PIN:
      message = "Cronet_RESULT_ILLEGAL_ARGUMENT_INVALID_PIN";
      break;
    case Cronet_RESULT_ILLEGAL_ARGUMENT_INVALID_HOSTNAME:
      message = "Cronet_RESULT_ILLEGAL_ARGUMENT_INVALID_HOSTNAME";
      break;
    case Cronet_RESULT_ILLEGAL_ARGUMENT_INVALID_HTTP_METHOD:
      message = "Cronet_RESULT_ILLEGAL_ARGUMENT_INVALID_HTTP_METHOD";
      break;
    case Cronet_RESULT_ILLEGAL_ARGUMENT_INVALID_HTTP_HEADER:
      message = "Cronet_RESULT_ILLEGAL_ARGUMENT_INVALID_HTTP_HEADER";
      break;
    case Cronet_RESULT_ILLEGAL_STATE:
      message = "Cronet_RESULT_ILLEGAL_STATE";
      break;
    case Cronet_RESULT_ILLEGAL_STATE_STORAGE_PATH_IN_USE:
      message = "Cronet_RESULT_ILLEGAL_STATE_STORAGE_PATH_IN_USE";
      break;
    case Cronet_RESULT_ILLEGAL_STATE_CANNOT_SHUTDOWN_ENGINE_FROM_NETWORK_THREAD:
      message = "Cronet_RESULT_ILLEGAL_STATE_CANNOT_SHUTDOWN_ENGINE_FROM_NETWORK_THREAD";
      break;
    case Cronet_RESULT_ILLEGAL_STATE_ENGINE_ALREADY_STARTED:
      message = "Cronet_RESULT_ILLEGAL_STATE_ENGINE_ALREADY_STARTED";
      break;
    case Cronet_RESULT_ILLEGAL_STATE_REQUEST_ALREADY_STARTED:
      message = "Cronet_RESULT_ILLEGAL_STATE_REQUEST_ALREADY_STARTED";
      break;
    case Cronet_RESULT_ILLEGAL_STATE_REQUEST_NOT_INITIALIZED:
      message = "Cronet_RESULT_ILLEGAL_STATE_REQUEST_NOT_INITIALIZED";
      break;
    case Cronet_RESULT_ILLEGAL_STATE_REQUEST_ALREADY_INITIALIZED:
      message = "Cronet_RESULT_ILLEGAL_STATE_REQUEST_ALREADY_INITIALIZED";
      break;
    case Cronet_RESULT_ILLEGAL_STATE_REQUEST_NOT_STARTED:
      message = "Cronet_RESULT_ILLEGAL_STATE_REQUEST_NOT_STARTED";
      break;
    case Cronet_RESULT_ILLEGAL_STATE_UNEXPECTED_REDIRECT:
      message = "Cronet_RESULT_ILLEGAL_STATE_UNEXPECTED_REDIRECT";
      break;
    case Cronet_RESULT_ILLEGAL_STATE_UNEXPECTED_READ:
      message = "Cronet_RESULT_ILLEGAL_STATE_UNEXPECTED_READ";
      break;
    case Cronet_RESULT_ILLEGAL_STATE_READ_FAILED:
      message = "Cronet_RESULT_ILLEGAL_STATE_READ_FAILED";
      break;
    case Cronet_RESULT_NULL_POINTER:
      message = "Cronet_RESULT_NULL_POINTER";
      break;
    case Cronet_RESULT_NULL_POINTER_HOSTNAME:
      message = "Cronet_RESULT_NULL_POINTER_HOSTNAME";
      break;
    case Cronet_RESULT_NULL_POINTER_SHA256_PINS:
      message = "Cronet_RESULT_NULL_POINTER_SHA256_PINS";
      break;
    case Cronet_RESULT_NULL_POINTER_EXPIRATION_DATE:
      message = "Cronet_RESULT_NULL_POINTER_EXPIRATION_DATE";
      break;
    case Cronet_RESULT_NULL_POINTER_ENGINE:
      message = "Cronet_RESULT_NULL_POINTER_ENGINE";
      break;
    case Cronet_RESULT_NULL_POINTER_URL:
      message = "Cronet_RESULT_NULL_POINTER_URL";
      break;
    case Cronet_RESULT_NULL_POINTER_CALLBACK:
      message = "Cronet_RESULT_NULL_POINTER_CALLBACK";
      break;
    case Cronet_RESULT_NULL_POINTER_EXECUTOR:
      message = "Cronet_RESULT_NULL_POINTER_EXECUTOR";
      break;
    case Cronet_RESULT_NULL_POINTER_METHOD:
      message = "Cronet_RESULT_NULL_POINTER_METHOD";
      break;
    case Cronet_RESULT_NULL_POINTER_HEADER_NAME:
      message = "Cronet_RESULT_NULL_POINTER_HEADER_NAME";
      break;
    case Cronet_RESULT_NULL_POINTER_HEADER_VALUE:
      message = "Cronet_RESULT_NULL_POINTER_HEADER_VALUE";
      break;
    case Cronet_RESULT_NULL_POINTER_PARAMS:
      message = "Cronet_RESULT_NULL_POINTER_PARAMS";
      break;
    case Cronet_RESULT_NULL_POINTER_REQUEST_FINISHED_INFO_LISTENER_EXECUTOR:
      message = "Cronet_RESULT_NULL_POINTER_REQUEST_FINISHED_INFO_LISTENER_EXECUTOR";
      break;
    default:
      message = "unknown Cronet_RESULT";
      break;
  }
  DCHECK(napi_throw_error(env, code.c_str(), message));
}
