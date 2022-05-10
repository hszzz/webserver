#ifndef LOG_H
#define LOG_H

#include <list>
#include <memory>
#include <string>

#define LOG_LEVEL(level, message) \
  base::GetLogger()->log(         \
      level, std::make_shared<base::LogEvent>(__FILE__, __LINE__, message))

#define LOG_DEBUG(message) LOG_LEVEL(base::LogLevel::DEBUG, message)
#define LOG_INFO(message) LOG_LEVEL(base::LogLevel::INFO, message)
#define LOG_WARN(message) LOG_LEVEL(base::LogLevel::WARN, message)
#define LOG_ERROR(message) LOG_LEVEL(base::LogLevel::ERROR, message)
#define LOG_FATAL(message) LOG_LEVEL(base::LogLevel::FATAL, message)

namespace base {

class LogLevel {
 public:
  enum Level {
    UNKNOWN = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
  };

  static const char* ToString(Level level);
};

class LogEvent {
 public:
  using ptr = std::shared_ptr<LogEvent>;

  LogEvent(const char* file, uint32_t line, const std::string& message);

  std::string ToString(LogLevel::Level level);

 private:
  const char* file_;
  uint32_t line_;
  uint32_t thread_id_;
  uint64_t time_;
  std::string message_;
};

class LogAppender {
 public:
  using ptr = std::shared_ptr<LogAppender>;

  virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;
  virtual ~LogAppender() = default;
};

class LogConsoleAppender : public LogAppender {
 public:
  using ptr = std::shared_ptr<LogConsoleAppender>;

  void log(LogLevel::Level level, LogEvent::ptr event) override;
};

class Logger {
 public:
  using ptr = std::shared_ptr<Logger>;

  Logger() : level_(LogLevel::UNKNOWN) {}

  void log(LogLevel::Level level, LogEvent::ptr event);
  void debug(LogEvent::ptr event);
  void info(LogEvent::ptr event);
  void warn(LogEvent::ptr event);
  void error(LogEvent::ptr event);
  void fatal(LogEvent::ptr event);

  void SetLevel(LogLevel::Level level) { level_ = level; }

  void AddAppender(LogAppender::ptr appender);
  void RemoveAppender(LogAppender::ptr appender);

 private:
  std::string name_;
  LogLevel::Level level_;
  std::list<LogAppender::ptr> appenders_;
};

Logger::ptr GetLogger();

}  // namespace base

#endif  // LOG_H
