/*
 * @Description: Transport client net
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-04-21 22:16:04
 */
#ifndef __TRANSMISSION_CLIENT_NET_H__
#define __TRANSMISSION_CLIENT_NET_H__

#include "TransmissionClientInterface.h"

namespace remote_display {

class TransmissionClientNet : virtual public TransmissionClientIf {
  public:
    TransmissionClientNet() = default;
    ~TransmissionClientNet() = default;

    int transmissionConnect() override;
    void transmissionDisconnect() override;
    int sendData(char* data, size_t length) override;

  private:
    void setnonblocking(int sock);
    int mSockCli{-1};
    int mEpollFd{-1};
};

}  // namespace remote_display

#endif // __TRANSMISSION_CLIENT_NET_H__
