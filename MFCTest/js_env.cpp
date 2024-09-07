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

namespace js {

namespace {

static std::map<std::string, TJSRuntime*> tjs_name_runtime_map;
static std::map<std::string, std::thread*> tjs_name_thread_map;

void LogInfo(std::string log_str) {
  std::cout << "Print Log:" << log_str << std::endl;
}

static void TjsPrintStrHandlerCallback(const char* str) {
  LogInfo(str);
}

bool NewJSThread(std::string name, std::string script_content) {
  TJS_Initialize(0, NULL);


  LogInfo("script_content:" + script_content);

  TJSRuntime* qrt = TJS_NewRuntime();
  if (!qrt) {
    LogInfo("TJS_NewRuntime Error");
    tjs_name_thread_map.erase(name);
    tjs_name_runtime_map.erase(name);
    return false;
  }

  tjs_name_runtime_map[name] = qrt;

  TJS_SetCustomLoggerInfoCallback(qrt, [](TJSRuntime *tjs_rt, const char *log_str) {
      std::cout << "Print Log2:" << log_str << std::endl;
  });

  int exit_code = TJS_RunWithEntryScriptContent(qrt, script_content.c_str(), script_content.size(), name.c_str());

  char tmp[64];
  sprintf(tmp, "TJS_Runtime exit, exit code: %d", exit_code);

  LogInfo(tmp);

  TJS_FreeRuntime(qrt);

  tjs_name_thread_map.erase(name);
  tjs_name_runtime_map.erase(name);

  return true;
}

void StopJSThread(const std::string& name) {
  auto it = tjs_name_runtime_map.find(name);
  if (it != tjs_name_runtime_map.end()) {
    TJSRuntime *qrt = it->second;

    TJS_Stop(qrt);
  }
}

}  // namespace

bool RunJSWithNewThread(std::string name, std::string content) {
  auto it = tjs_name_runtime_map.find(name);
if (it != tjs_name_runtime_map.end()) {
	LogInfo("JS Thread already exists");
	return false;
  }
  std::thread *js_thread = new std::thread(NewJSThread, name, std::move(content));
  tjs_name_thread_map[name] = js_thread;

  return true;
}

void StopAll() {
  for (auto &it : tjs_name_runtime_map) {
	TJSRuntime *qrt = it.second;
	TJS_Stop(qrt);
  }
}

}  // namespace js
