#include "../log.h"

int main() {
  base::GetLogger()->AddAppender(std::make_shared<base::LogConsoleAppender>());
  base::GetLogger()->SetLevel(base::LogLevel::DEBUG);

  LOG_DEBUG("test debug");
  LOG_INFO("test info");
  LOG_ERROR("test error");
  LOG_FATAL("test fatal");
  LOG_WARN("test warn");
}
