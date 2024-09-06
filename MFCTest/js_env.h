#ifndef JS_ENV_H_
#define JS_ENV_H_

#include <string>

namespace js {

bool RunJSWithNewThread(std::string name, std::string content);
void StopJSThread(const std::string & name);
void StopAll();

}  // namespace js

#endif  // !JS_ENV_H_
