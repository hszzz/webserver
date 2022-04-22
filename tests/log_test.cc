#include "../log.h"

int main() {
  base::GetLogger()->AddAppender(std::make_shared<base::LogConsoleAppender>());
  base::GetLogger()->SetLevel(base::LogLevel::DEBUG);

  base::LOG_DEBUG("test debug");
  base::LOG_INFO("test info");
  base::LOG_ERROR("test error");
  base::LOG_FATAL("test fatal");
  base::LOG_WARN("test warn");
}
