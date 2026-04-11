#include "fileLog.h"
#include "logSystem.h"
#include "logger.h"
#include "terminalLog.h"

#include <iostream>

int main() {
  std::cout << "Slog Software Framework v12042026" << std::endl;
  std::cout << "GitHub: https://github.com/grechka731" << std::endl;
  std::cout << "Copyright (c) 2026 Grechka. All rights reserved" << std::endl
            << std::endl;

  // Slog code-->
  // Slog code-->
  // Slog code-->

  // connect terminal log output system

  auto *term = new terminalLog;
  logger.addSystem(term);
  logger.addSystem(new fileLog("log"));

  term->setStyle(CRITICAL, {255, 0, 0, 255, 255, 255, true});
  term->setStyle(WARNING, {255, 140, 0, 0, 0, 0, false});

  LOG_DEBUG("DEBUG");
  LOG_INFO("INFO");
  LOG_WARN("WARNING");
  LOG_ERROR("ERROR");
  LOG_CRITICAL("CRITICAL");

  //<--Slog code
  //<--Slog code
  //<--Slog code

  while (1) {
  }
  return 0;
}