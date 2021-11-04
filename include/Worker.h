#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <thread>

class Worker {
  private:
    std::thread m_thread;
    bool m_terminate;
    bool m_running;

  protected:
    virtual bool step() { return false; };

  private:
    bool isRunning(void) {
      return !(this->m_terminate);
    }

    void terminateWorker(void) {
      this->m_terminate = true;
    }

    void work() {
      while(this->isRunning()) {
        if (!this->step()) 
          break;
      }
      this->m_terminate = true;
    }

  public:
    Worker() {
      this->m_running = false;	
      this->m_terminate = false;	
    };
    ~Worker() {
    };

    void start(void) {
      if (this->m_running) {
        return;
      } else {
        this->m_running = true;
        this->m_thread = std::thread(&Worker::work, this);
      }
    };

    void stop() {
      this->terminateWorker();
    };

    void join() {
      this->m_thread.join();
    };
};

#endif 
