#include "pch.h"

#include "js_env.h"

#include <stdlib.h>
#include "private.h"
#include "quickjs.h"
#include "tjs.h"
#include <string.h>
#include <map>
#include <thread>
#include <iostream>

//bool Initialize();
//void RunJSContent(std::string, std::string content);
//void StopContent(std::string name);
//void StopAll();
//void ReleaseAll();

namespace js {

namespace {

static TJSRuntime* qrt = nullptr;
static std::map<std::string, JSContext*> tjs_name_ctx_map;
static std::thread* js_thread = nullptr;

void LogInfo(std::string log_str) {
  std::cout << "Print Log:" << log_str << std::endl;
}

static void TjsPrintStrHandlerCallback(const char* str) {
  LogInfo(str);
}

bool InitializeJSThread() {
  TJS_Initialize(0, NULL);
  qrt = TJS_NewRuntime();
  if (!qrt) {
    LogInfo("TJS_NewRuntime Error");
    return false;
  }

  tjs_set_print_str_handler(TjsPrintStrHandlerCallback);

  int exit_code = TJS_Run(qrt);
  char tmp[64];
  sprintf(tmp, "TJS_Runtime exit, exit code: %d", exit_code);

  LogInfo(tmp);

  TJS_FreeRuntime(qrt);

  return true;
}

void StopJSThread() {
  TJS_Stop(qrt);
}

}  // namespace

bool Initialize() {
  if (js_thread) {
    return false;
  }
  js_thread = new std::thread(InitializeJSThread);
  return true;
}

void RunJSContent(std::string name, std::string content) {
  Initialize();
  auto ctx = JS_NewContext(qrt->rt);
  if (!ctx) {
    LogInfo("JS_NewContext Error");
    return;
  }

  tjs_name_ctx_map[name] = ctx;

  JSValue val = JS_Eval(ctx, content.c_str(), content.size(), name.c_str(),
                        JS_EVAL_TYPE_MODULE);
  if (JS_IsException(val)) {
    // 打印错误信息
    JSValue e = JS_GetException(ctx);
    JSValue stack = JS_GetPropertyStr(ctx, e, "stack");
    const char* stack_str = JS_ToCString(ctx, stack);
    LogInfo(stack_str);
    JS_FreeCString(ctx, stack_str);
    JS_FreeValue(ctx, stack);
    JS_FreeValue(ctx, e);
    JS_FreeValue(ctx, val);
    JS_FreeContext(ctx);

    return;
  }
}

void StopContent(std::string name) {
  auto it = tjs_name_ctx_map.find(name);
  if (it != tjs_name_ctx_map.end()) {
    JSContext* ctx = it->second;
    JS_FreeContext(ctx);
    tjs_name_ctx_map.erase(it);
  }
}

void StopAll() {
  for (auto& it : tjs_name_ctx_map) {
    JSContext* ctx = it.second;
    JS_FreeContext(ctx);
  }
  tjs_name_ctx_map.clear();
}

void ReleaseAll() {
  StopAll();
  if (js_thread) {
    StopJSThread();
    js_thread->join();
    delete js_thread;
    js_thread = nullptr;
  }
}

}  // namespace js
