#ifndef FERN_PRODUCER_H
#define FERN_PRODUCER_H

#include <iostream>
#include <stack>
#include <random>
#include <vector>
#include <sstream>
#include "main.h"
#include "Producer.h"
#include "Logger.h"

typedef struct DataPoint {
  float x1;
  float x2;
} DataPoint_t;

class FernProducer: public Producer<DataPoint_t> {
  private:
    Logger& log;
    DataPoint_t zustandsvector; // x1 and x2

    float params[4][6] = {
      { 0, 0, 0, 0.16, 0, 0 }, // f1
      { 0.85, 0.04, -0.04, 0.85, 0, 1.6 }, // f2
      { 0.2, -0.26, 0.23, 0.22, 0, 1.6 }, // f3
      { -0.15, 0.28, -0.26, 0.24, 0, 0.44 }, // f4
    };

    float wkeiten[4] = {
        0.01, 0.85, 0.07, 0.07
    };

    std::random_device rd;
    int cnt;
    int id;
    int& producers;

    void logActivity(void) {
        std::ostringstream ss;
        ss << "Produced by " 
           << this->id;

        std::string str = ss.str();
        log.log(str);
    }

  public:
    FernProducer(Buffer<DataPoint_t> &buf, Logger& logger, int& producer_id)
      : Producer(buf), 
        log(logger), 
        rd(), 
        producers(producer_id){

      this->producers++;
      this->id = this->producers;
      this->zustandsvector.x1 = 0;
      this->zustandsvector.x2 = 0;
      this->cnt = 0;
    };
    ~FernProducer(){
    };

  protected:
    bool produce(DataPoint_t &data) override {
      if (cnt < MAX_NUMBER_PRODUCED_POINTS) {
        std::uniform_int_distribution<int> dist(0, 100);
        float r = dist(this->rd);

        if (r < (wkeiten[0] * 100)) {
          zustandsvector.x1 = zustandsvector.x1*params[0][0] + zustandsvector.x2*params[0][1] + params[0][4];
          zustandsvector.x2 = zustandsvector.x1*params[0][2] + zustandsvector.x2*params[0][3] + params[0][5];

        } else if (r < ((wkeiten[0] + wkeiten[1]) * 100)) {
          zustandsvector.x1 = zustandsvector.x1*params[1][0] + zustandsvector.x2*params[1][1] + params[1][4];
          zustandsvector.x2 = zustandsvector.x1*params[1][2] + zustandsvector.x2*params[1][3] + params[1][5];

        } else if (r < ((wkeiten[0] + wkeiten[1] + wkeiten[2]) * 100)) {
          zustandsvector.x1 = zustandsvector.x1*params[2][0] + zustandsvector.x2*params[2][1] + params[2][4];
          zustandsvector.x2 = zustandsvector.x1*params[2][2] + zustandsvector.x2*params[2][3] + params[2][5];

        } else {
          zustandsvector.x1 = zustandsvector.x1*params[3][0] + zustandsvector.x2*params[3][1] + params[3][4];
          zustandsvector.x2 = zustandsvector.x1*params[3][2] + zustandsvector.x2*params[3][3] + params[3][5];
        }

        data.x1 = zustandsvector.x1;
        data.x2 = zustandsvector.x2;
        cnt += 1;

#ifdef LOGGING
        this->logActivity();
#endif
        return true;
      } else {
        return false;
      }

    };
};

#endif 
