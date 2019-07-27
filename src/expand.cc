#include <string>
#include <libpostal/libpostal.h>

#include "expand.h"

class ExpandWorker : public Napi::AsyncWorker
{
public:
  ExpandWorker(const Napi::Function& fn, const std::string& address):
    Napi::AsyncWorker(fn),
    address(address)
    {}

  void Execute() override;
  void OnOK() override;

private:
  std::string address;
  size_t num_expansions = 0;
  char** expansions;

  static bool initialized;

  static bool Init() {
    if (!initialized) {
      initialized = libpostal_setup() && libpostal_setup_language_classifier();
    }
    return initialized;
  }

  static void Cleanup() {
    if (initialized) {
      libpostal_teardown_language_classifier();
      libpostal_teardown();
      initialized = false;
    }
  }

  friend void CleanupExpand();
};

bool ExpandWorker::initialized = false;

void ExpandWorker::Execute() {
  if (!Init()) {
    SetError("Error parsing address");
    return;
  }
  auto options = libpostal_get_default_options();
  expansions = libpostal_expand_address(const_cast<char*>(address.c_str()), options, &num_expansions);
}

void ExpandWorker::OnOK() {
  auto env = Env();

  auto results = Napi::Array::New(env, num_expansions);
  for (int i = 0; i < num_expansions; i++) {
    results.Set(i, Napi::String::New(env, expansions[i]));
  }

  libpostal_expansion_array_destroy(expansions, num_expansions);
  Callback().MakeCallback(Receiver().Value(), { env.Null(), results });
}

Napi::Value Expand(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  auto len = info.Length();

  if (len < 2 || len > 3) {
    Napi::TypeError::New(env, "Wrong number of arguments")
      .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Need address to expand")
      .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[len - 1].IsFunction()) {
    Napi::TypeError::New(env, "Need callback function")
      .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto address = info[0].As<Napi::String>();
  auto fn = info[len - 1].As<Napi::Function>();

  (new ExpandWorker(fn, address))->Queue();
  return env.Undefined();
}

void CleanupExpand() {
  ExpandWorker::Cleanup();
}
