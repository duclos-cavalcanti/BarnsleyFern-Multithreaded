#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "FernProducer.h"
#include "FernConsumer.h"
#include "Buffer.h"

#include "CImg.h"
#include "main.h"

int main(int argc, char* argv[])
{
  int consumer_id = 0;
  int producer_id = 0;

  // Buffer and Log
  Buffer<DataPoint_t> buf;
  Logger log;

  // Image
  static cimg_library::CImg<unsigned char> image(IMAGE_WIDTH, IMAGE_HEIGHT, 1, 3); // 10K x 20K Image
  image.fill(255); // white background


  // Barnsley Fern
  {
    std::vector<std::mutex> image_mutex_vec(IMAGE_WIDTH); // lock per column of image

    FernProducer producer_1(buf, log, producer_id);
    FernConsumer consumer_1(buf, log, image, image_mutex_vec, consumer_id);
    FernConsumer consumer_2(buf, log, image, image_mutex_vec, consumer_id);
    FernConsumer consumer_3(buf, log, image, image_mutex_vec, consumer_id);

    producer_1.start();
    consumer_1.start();
    consumer_2.start();
    consumer_3.start();

    producer_1.join();
    consumer_1.join();
    consumer_2.join();
    consumer_3.join();
  }

#ifdef LOGGING
    log.dumpLog();
    std:: cout << "LOG: Total number of points pushed into the Buffer: " << buf.getPushedPoints() << std::endl;
    std:: cout << "LOG: Total number of points consumed from the Buffer: " << buf.getConsumedPoints() << std::endl;
#endif

  return 0;
}
