#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include "main.h"

template <typename T> class Buffer {
  private:
    T buffer[1000];
    std::mutex m;
    std::condition_variable cv_pop;
    std::condition_variable cv_push;
    int readPtr = 0, writePtr = 0;
    std::atomic<unsigned long long> current_size = { 0 };
#ifdef LOGGING
    unsigned long long pushedPoints = 0;
    unsigned long long consumedPoints = 0;
#endif

  public:
    Buffer(){
    };
    ~Buffer() {};

    bool isBufferFull(void) {
      return (this->current_size >= 1000);
    }

    bool isBufferEmpty(void) {
      return (current_size == 0);
    }

    unsigned long long getPushedPoints() {
      return this->pushedPoints;
    }

    unsigned long long getConsumedPoints() {
      return this->consumedPoints;
    }

    void push(T& data) {
      {
        std::unique_lock<std::mutex> lock(m);
        cv_push.wait(lock, 
               [this]{return !this->isBufferFull();});

        this->buffer[this->writePtr] = data;
        writePtr+=1;
        this->writePtr = this->writePtr % 1000;
        this->current_size += 1;
#ifdef LOGGING
        this->pushedPoints += 1;
#endif
      }
        cv_pop.notify_one();
    }

    T pop(void) {
      T val;
      {
        std::unique_lock<std::mutex> lock(m);
        if(!cv_pop.wait_for(lock, 
                            std::chrono::seconds(WAIT_TIME_BUFFER), 
                            [this] { return !this->isBufferEmpty();})) {
          throw std::runtime_error("Values aren't being produced");
        }

        val = this->buffer[readPtr];
        this->readPtr += 1;
        this->readPtr = this->readPtr % 1000;
        this->current_size -= 1;
#ifdef LOGGING
        this->consumedPoints += 1;
#endif
      }
      cv_push.notify_one();
      return val;
    }
};

#endif 
