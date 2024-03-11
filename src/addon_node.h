#ifndef _ADDON_NODE_H_
#define _ADDON_NODE_H_

#include <assert.h>
#include <node_api.h>
#include <js_native_api.h>
#include <stdio.h>

#ifdef _DEBUG
#define TRACE(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define TRACE(fmt, ...)
#endif

#define DCHECK(call) \
  status = (call);           \
  assert(status == napi_ok); \

#define NODE_API_CALL(env, call)                                  \
  do {                                                            \
    status = (call);                                              \
    if (status != napi_ok) {                                      \
      const napi_extended_error_info* error_info = NULL;          \
      napi_get_last_error_info((env), &error_info);               \
      const char* err_message = error_info->error_message;        \
      bool is_pending;                                            \
      napi_is_exception_pending((env), &is_pending);              \
      /* If an exception is already pending, don't rethrow it */  \
      if (!is_pending) {                                          \
        const char* message = (err_message == NULL)               \
            ? "empty error message"                               \
            : err_message;                                        \
        napi_throw_error((env), NULL, message);                   \
      }                                                           \
      return NULL;                                                \
    }                                                             \
  } while (0)

#endif  // _ADDON_NODE_H_
