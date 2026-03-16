#include "logSystem.h"
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <ctime>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#ifndef NDEBUG
#define LOG_DEBUG(msg)                                                         \
  logger.addLog(DEBUG, __FILE__, __FUNCTION__, __LINE__, msg)
#define LOG_INFO(msg) logger.addLog(INFO, __FILE__, __FUNCTION__, __LINE__, msg)
#else
#define LOG_DEBUG(msg) ((void)0)
#define LOG_INFO(msg) ((void)0)
#endif
#define LOG_WARN(msg)                                                          \
  logger.addLog(WARNING, __FILE__, __FUNCTION__, __LINE__, msg)
#define LOG_ERROR(msg)                                                         \
  logger.addLog(ERROR, __FILE__, __FUNCTION__, __LINE__, msg)
#define LOG_CRITICAL(msg)                                                      \
  logger.addLog(CRITICAL, __FILE__, __FUNCTION__, __LINE__, msg)

struct LogEntry {
  LogLevel level;
  std::string message;
};

class Logger {
public:
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;
  Logger(Logger &&) = delete;
  Logger &operator=(Logger &&) = delete;

  static Logger &getInstance() {
    static Logger instance;
    return instance;
  }

  void setPath(const char *path) {
    strncpy(this->_path, path, sizeof(this->_path) - 1);
    this->_path[sizeof(this->_path) - 1] = '\0';
  }

  const char *getPath() { return _path; }

  bool addLog(LogLevel type, const char *file, const char *func, int line,
              const char *msg);
  void addSystem(logSystem *system);

private:
  Logger();
  ~Logger();

  char _msg[128];
  std::vector<logSystem *> _allSystem;
  std::string levelToString(LogLevel level);
  char _path[512] = "basicPath.log\0";

  void processQueue();
  std::queue<LogEntry> _logQueue;
  std::mutex _queueMutex;
  std::condition_variable _cv;
  std::thread _workerThread;
  bool _stopWorker = false;
};

extern Logger &logger;
