#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "main.h"

class Logger {
  private:
    std::vector<std::string> l;
    std::mutex m;

  public:
    Logger(){
    };
    ~Logger() {};

    void log(std::string data) {
      {
        std::unique_lock<std::mutex> lock(m);
        l.push_back(data);
      }
    }

    void dumpLog(void) {
      for (unsigned long long i = 0; i < l.size(); i++) {
        std::cout << "LOG: " << l[i] << std::endl;
      }
    }
};

#endif 
