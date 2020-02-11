/*
 * @Descripttion: Transport server
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-02-11 23:16:38
 */
#ifndef __TRANSMISSION_SERVER_H__
#define __TRANSMISSION_SERVER_H__

#include <thread>
#include <poll.h>

#include "TransmissionCommon.h"

#define OPEN_MAX 10

using namespace std;
namespace remote_display {

class TransmissionHandler {
  public:
    virtual int setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) = 0;
    virtual int processFrame(uint8_t* data, size_t length) = 0;
};

class TransmissionServer {
  public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

class TransmissionServerNet : virtual public TransmissionServer {
  public:
    TransmissionServerNet(TransmissionHandler* transmissionHandler);
    virtual ~TransmissionServerNet();

    void start() override;
    void stop() override;

  private:
    bool recvAll(int sock, char *buffer, size_t size);
    void handleData(int& sock);
    void transServThread();
    TransmissionHandler* mTransmissionHandler{nullptr};
    struct pollfd mClient[OPEN_MAX];
    thread* mTransThread{nullptr};
};

}  // namespace remote_display

#endif // __TRANSMISSION_SERVER_H__
