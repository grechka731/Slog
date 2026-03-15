  #include "src/logger.h"
  #include "src/systems/terminalLog.h"
  #include "src/systems/fileLog.h"

  #include <iostream>

  int main() {
    std::cout<<"Slog Software Framework v1.0"<<std::endl;
    std::cout<<"GitHub: https://github.com/grechka731"<<std::endl;
    std::cout<<"Copyright (c) 2026 Grechka. All rights reserved"<<std::endl<<std::endl;



    // Slog code-->
    // Slog code-->
    // Slog code-->

    // connect terminal log output system

    logger.addSystem(new terminalLog);
    logger.addSystem(new fileLog("log"));

    LOG_DEBUG("DEBUG");       // verbose debug info
    LOG_INFO("INFO");         // general information
    LOG_WARN("WARNING");   // something worth attention
    LOG_ERROR("ERROR");       // recoverable error
    LOG_CRITICAL("CRITICAL"); // fatal, execution should stop

    //<--Slog code
    //<--Slog code
    //<--Slog code


    while (1) {
    }
    return 0;
  }