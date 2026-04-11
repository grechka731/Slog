#pragma once
#include "logSystem.h"
#include "logger.h"
#include <iostream>
#include <fstream>

class fileLog : public logSystem {
  char _final_msg[256];
  std::ofstream file;

public:
  fileLog(const char *path) : file(path, std::ios::trunc) {
    if (!file.is_open()) {
      std::cerr << "Failed to open log file: " << path << std::endl;
    } else {
      std::cerr << "Log file opened successfully: " << path << std::endl;
    }
  }

  ~fileLog() {
    if (file.is_open())
      file.close();
  }

  void addLog(const char *msg, const LogLevel level) {
    file << msg << std::endl;
    file.flush();
  }
};
