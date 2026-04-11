#pragma once

enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class logSystem{
public:
    virtual ~logSystem()=default;
    virtual void addLog(const char* msg, const LogLevel level)=0;
};