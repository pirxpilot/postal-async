#pragma once

#include <napi.h>

Napi::Value Parse(const Napi::CallbackInfo& info);
void CleanupParser();
