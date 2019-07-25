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

  auto results = Napi::Object::New(env);

  for (int i = 0; i < response->num_components; i++) {
    const char *component = response->components[i];
    const char *label = response->labels[i];
    results.Set(label, component);
  }

  libpostal_address_parser_response_destroy(response);
  Callback().MakeCallback(Receiver().Value(), { env.Null(), results });
}

Napi::Value Parse(const Napi::CallbackInfo& info) {
  auto len = info.Length();
  auto address = info[0].As<Napi::String>().Utf8Value();
  auto fn = info[len - 1].As<Napi::Function>();

  (new ParseWorker(fn, address))->Queue();
}

void CleanupParser() {
  ParseWorker::Cleanup();
}
