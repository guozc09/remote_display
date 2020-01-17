/*
 * @Descripttion: Transport server
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-17 00:45:54
 */

#pragma once

#include <thread>

#include "TransmissionCommon.h"

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
    void receiveThread();
    TransmissionHandler* mTransmissionHandler{nullptr};
    int mSockConn{-1};
    bool mIsRun{false};
    thread* mRecvThread{nullptr};
};

}  // namespace remote_display
