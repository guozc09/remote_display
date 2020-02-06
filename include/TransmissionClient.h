/*
 * @Descripttion: Transport client
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-15 16:49:43
 */
#ifndef __TRANSMISSION_CLIENT_H__
#define __TRANSMISSION_CLIENT_H__

#include "TransmissionCommon.h"

namespace remote_display {

class TransmissionClient {
  public:
    virtual int transmissionConnect() = 0;
    virtual void transmissionDisconnect() = 0;
    virtual int sendData(char* data, size_t length) = 0;
};

class TransmissionClientNet : virtual public TransmissionClient {
  public:
    TransmissionClientNet() = default;
    virtual ~TransmissionClientNet() = default;

    int transmissionConnect() override;
    void transmissionDisconnect() override;
    int sendData(char* data, size_t length) override;

  private:
    int mSockClient{-1};
};

}  // namespace remote_display

#endif // __TRANSMISSION_CLIENT_H__
