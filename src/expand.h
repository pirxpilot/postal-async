#pragma once

#include <napi.h>

Napi::Value Expand(const Napi::CallbackInfo& info);
void CleanupExpand();
