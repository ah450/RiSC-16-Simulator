#pragma once
#include <iostream>

struct ILogger {
    virtual ILogger& operator<<(const std::string &) = 0;
    virtual ILogger& operator<<(char c) = 0;
    virtual ~ILogger(){};
};


struct BasicLogger : public ILogger {
    ILogger& operator<<(const std::string &str) override {std::cerr << str << '\n'; return *this;}
    ILogger& operator<<(char c) override {std::cerr << c << '\n'; return *this;}
};


