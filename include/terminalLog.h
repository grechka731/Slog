#pragma once

#include "../include/logSysem.h"
#include "../include/logger.h"

class terminalLog : public logSystem {
   char _final_msg[256];

    public:
  void addLog(const char *msg, const LogLevel level) {
    const char *colorCode = "\033[0m";
    switch (level) {
    case DEBUG:
      colorCode = "\033[38;2;150;150;150m";
      break;
    case INFO:
      colorCode = "\033[38;2;100;200;100m";
      break;
    case WARNING:
      colorCode = "\033[38;2;255;200;0m";
      break;
    case ERROR:
      colorCode = "\033[38;2;255;100;100m";
      break;
    case CRITICAL:
      colorCode = "\033[38;2;255;0;0m";
      break;
    }

    snprintf(_final_msg, sizeof(_final_msg), "%s%s\033[0m\n", colorCode, msg);

    printf("%s", _final_msg);
    fflush(stdout); 
  }
};