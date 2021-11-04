#ifndef PRODUCER_H
#define PRODUCER_H

#include <stack>
#include "Worker.h"
#include "Buffer.h"

template <typename T> class Producer: public Worker {
  private:
    Buffer<T>& m_buffer;

  protected:
    virtual bool produce(T& data) { return false; };
    bool step() override {
      T data;
      bool produced = this->produce(data);

      if (produced) {
        m_buffer.push(data);
        return true;
      } else {
        return false;
      }
    };
  public:
    Producer(Buffer<T>& buffer) : m_buffer(buffer) {};
    ~Producer(){
    };
};

#endif 
