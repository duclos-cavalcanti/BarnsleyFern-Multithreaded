#ifndef FERN_CONSUMER_H
#define FERN_CONSUMER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <sstream>
#include <condition_variable>
#include <cmath>
#include "Consumer.h"
#include "FernProducer.h"
#include "Buffer.h"
#include "Logger.h"
#include "main.h"

#include "CImg.h"

class FernConsumer: public Consumer<DataPoint_t> {
  private:
    Logger& log;
    cimg_library::CImg<unsigned char>& img;
    std::vector<std::mutex>& m_vec; // size 4
    int id; // which consumer this is among the total consumers
    int& consumers;

    unsigned char color[3] = { 0, 0, 0 };
    unsigned char *colorPixel;

    void logActivity(void) {
        std::ostringstream ss;
        ss << "Consuming from " 
           << this->id << " with color "
           << (int) this->color[0] 
           << ", "
           << (int) this->color[1] 
           << ", "
           << (int) this->color[2];

        std::string str = ss.str();
        log.log(str);
    }

  protected:
    bool consume(const DataPoint_t& data) override {
      int index = 0;
      unsigned int x = ceil((data.x1 + 2.2) * IMAGE_WIDTH/ 4.9);
      unsigned int y = ceil((data.x2 + 0) * IMAGE_HEIGHT/ 10);

      // flip image
      if (y < IMAGE_HEIGHT / 2) {
        y = (IMAGE_HEIGHT/2) + ((IMAGE_HEIGHT/2) - y);
      } else if (y > IMAGE_HEIGHT / 2) {
        y = (IMAGE_HEIGHT/2) - (y - (IMAGE_HEIGHT/2));
      } 

      index = x; // lock per column of image

      {
        std::lock_guard<std::mutex> lock(m_vec[index]);

#ifdef LOGGING
        this->logActivity();
#endif
        img.draw_point(x, y, this->color);
        if (*colorPixel < 255)
          *colorPixel = *colorPixel + 1;
      }
      
      return true;
    };

  public:
    FernConsumer(Buffer<DataPoint_t>& buf, 
                 Logger& logger, 
                 cimg_library::CImg<unsigned char>& image, 
                 std::vector<std::mutex>& m_vec_global,
                 int& consumer_id)
      : Consumer(buf), 
        log(logger), 
        img(image), 
        m_vec(m_vec_global), 
        consumers(consumer_id){

        this->consumers++;
        this->id = this->consumers;
        colorPixel = &color[0]; // pixel color 0 = red, 1 = green, 2 = blue
    };
    ~FernConsumer() {
      if (this->consumers > 1) {
        this->consumers--;
      } else {
        img.save_png("../fern.png");
      }
    };
};

#endif 
