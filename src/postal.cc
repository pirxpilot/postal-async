#include <libpostal/libpostal.h>

#include <napi.h>

#include "expand.h"
#include "parse.h"

static void teardown(void*) {
  CleanupParser();
  CleanupExpand();
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "parse"), Napi::Function::New(env, Parse));
  exports.Set(Napi::String::New(env, "expand"), Napi::Function::New(env, Expand));

  napi_add_env_cleanup_hook(env, teardown, nullptr);
  return exports;
}

NODE_API_MODULE(addon, Init)
