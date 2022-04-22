#include <pthread.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "log.h"

namespace base {

const char* LogLevel::ToString(Level level) {
  switch (level) {
#define XX(name)       \
  case LogLevel::name: \
    return #name;      \
    break;

    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);

#undef XX
    default:
      return "UNKNOW";
  }
  return "UNKNOW";
}

LogEvent::LogEvent(const char* file, uint32_t line, const std::string& message)
    : file_(file), line_(line), message_(message) {
  time_ = std::chrono::system_clock::now().time_since_epoch().count();
  thread_id_ = ::pthread_self();
}

std::string LogEvent::ToString(LogLevel::Level level) {
  std::ostringstream ss;
  ss << LogLevel::ToString(level) << " " << time_ << " " << file_ << ":"
     << line_ << " " << message_ << std::endl;
  return ss.str();
}

void LogConsoleAppender::log(LogLevel::Level level, LogEvent::ptr event) {
  std::cout << event->ToString(level);
}

void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
  for (auto appender : appenders_) {
    if (level_ <= level) appender->log(level, event);
  }
}

void Logger::debug(LogEvent::ptr event) { log(LogLevel::DEBUG, event); }

void Logger::info(LogEvent::ptr event) { log(LogLevel::INFO, event); }

void Logger::warn(LogEvent::ptr event) { log(LogLevel::WARN, event); }

void Logger::error(LogEvent::ptr event) { log(LogLevel::ERROR, event); }

void Logger::fatal(LogEvent::ptr event) { log(LogLevel::FATAL, event); }

void Logger::AddAppender(LogAppender::ptr appender) {
  appenders_.push_back(appender);
}

void Logger::RemoveAppender(LogAppender::ptr appender) {
  for (auto it = appenders_.begin(); it != appenders_.end(); ++it) {
    if (*it == appender) {
      appenders_.remove(appender);
      break;
    }
  }
}

static Logger::ptr logger = std::make_shared<Logger>();
Logger::ptr GetLogger() { return logger; }

}  // namespace base
