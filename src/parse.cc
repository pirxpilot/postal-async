#include <string>
#include <libpostal/libpostal.h>

#include "parse.h"

class ParseWorker : public Napi::AsyncWorker
{
public:
  ParseWorker(const Napi::Function& fn, const std::string& address):
    Napi::AsyncWorker(fn),
    address(address)
    {}

  void Execute() override;
  void OnOK() override;
private:
  std::string address;
  libpostal_address_parser_response_t *response;

  static bool initialized;

  static bool Init() {
    if (!initialized) {
      initialized = libpostal_setup() && libpostal_setup_parser();
    }
    return initialized;
  }

  static void Cleanup() {
    if (initialized) {
      libpostal_teardown_parser();
      libpostal_teardown();
      initialized = false;
    }
  }

  friend void CleanupParser();
};

bool ParseWorker::initialized = false;

void ParseWorker::Execute() {
  if (!Init()) {
    SetError("Error parsing address");
    return;
  }
  auto options = libpostal_get_address_parser_default_options();
  response = libpostal_parse_address(const_cast<char*>(address.c_str()), options);
  if (response == NULL) {
    SetError("Error parsing address");
  }
}

void ParseWorker::OnOK() {
  auto env = Env();

  auto results = Napi::Array::New(env, response->num_components);

  for (int i = 0; i < response->num_components; i++) {
    const char *component = response->components[i];
    const char *label = response->labels[i];
    auto o = Napi::Object::New(env);

    o.Set("label", label);
    o.Set("value", component);

    results.Set(i, o);
  }

  libpostal_address_parser_response_destroy(response);
  Callback().MakeCallback(Receiver().Value(), { env.Null(), results });
}

Napi::Value Parse(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  auto len = info.Length();

  if (len != 2) {
    Napi::TypeError::New(env, "Wrong number of arguments")
      .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Need address to parse")
      .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[1].IsFunction()) {
    Napi::TypeError::New(env, "Need callback function")
      .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto address = info[0].As<Napi::String>();
  auto fn = info[1].As<Napi::Function>();

  (new ParseWorker(fn, address))->Queue();
  return env.Undefined();
}

void CleanupParser() {
  ParseWorker::Cleanup();
}
