#ifndef CONSUMER_H
#define CONSUMER_H

#include <thread>
#include "Worker.h"
#include "Buffer.h"

template <typename T> class Consumer: public Worker {
  private:
    Buffer<T>& m_buffer;

  protected:
    virtual bool consume(const T& data) { return false; };
    bool step() override {
      T data;
      try {
        data = this->m_buffer.pop();
      } catch(std::runtime_error) {
        return false; // should stop base class
      }
      return this->consume(data);
    };
  public:
    Consumer(Buffer<T>& buffer) : m_buffer(buffer){};
    ~Consumer(){
    };

};

#endif 
