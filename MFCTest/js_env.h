#ifndef JS_ENV_H_
#define JS_ENV_H_

#include <string>

namespace js {

bool Initialize();
void RunJSContent(std::string name, std::string content);
void StopContent(std::string name);
void StopAll();
void ReleaseAll();

}  // namespace js

#endif  // !JS_ENV_H_
