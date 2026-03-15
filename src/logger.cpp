#include "logger.h"
#include <chrono>
#include <cstring>
#include <ctime>


Logger &logger = Logger::getInstance();

Logger::Logger() { _workerThread = std::thread(&Logger::processQueue, this); }

Logger::~Logger() {
  {
    std::lock_guard<std::mutex> lock(_queueMutex);
    _stopWorker = true;
  }

  _cv.notify_one();

  if (_workerThread.joinable()) {
    _workerThread.join();
  }

  for (auto sys : _allSystem) {
    delete sys;
  }
  _allSystem.clear();
}

std::string Logger::levelToString(LogLevel level) {
  switch (level) {
  case LogLevel::DEBUG:
    return "DEBUG";
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::WARNING:
    return "WARNING";
  case LogLevel::ERROR:
    return "ERROR";
  case LogLevel::CRITICAL:
    return "CRITICAL";
  default:
    return "UNKNOWN";
  }
}

bool Logger::addLog(LogLevel type, const char *file, const char *func, int line,
                    const char *msg) {
  {
    std::lock_guard<std::mutex> lock(_queueMutex);
    if (_stopWorker)
      return false;
  }

  auto now = std::chrono::system_clock::now();
  std::time_t time = std::chrono::system_clock::to_time_t(now);
  std::tm *local_time = std::localtime(&time);

  const char *fileName =
      strrchr(file, '/')
          ? strrchr(file, '/') + 1
          : (strrchr(file, '\\') ? strrchr(file, '\\') + 1 : file);

  char buffer[512];
  snprintf(buffer, sizeof(buffer),
           "[%04d-%02d-%02d %02d:%02d:%02d] [%-7s] [%s:%d] [%s] - %s",
           local_time->tm_year + 1900, local_time->tm_mon + 1,
           local_time->tm_mday, local_time->tm_hour, local_time->tm_min,
           local_time->tm_sec, levelToString(type).c_str(), fileName, line,
           func, msg);

  {
    std::lock_guard<std::mutex> lock(_queueMutex);
    _logQueue.push({type, std::string(buffer)});
  }
  _cv.notify_one();

  return true;
}

void Logger::processQueue() {
  while (true) {
    LogEntry entry;
    {
      std::unique_lock<std::mutex> lock(_queueMutex);
      _cv.wait(lock, [this] { return _stopWorker || !_logQueue.empty(); });

      if (_logQueue.empty()) {
        if (_stopWorker)
          break;
        continue;
      }

      entry = std::move(_logQueue.front());
      _logQueue.pop();
    }

    for (auto sys : _allSystem) {
      if (sys)
        sys->addLog(entry.message.c_str(), entry.level);
    }
  }
}

void Logger::addSystem(logSystem *system) {
  if (system != nullptr) {
    _allSystem.push_back(system);
  }
}