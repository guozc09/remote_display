/*
 * @Description: Transport server net
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-04-21 22:25:52
 */
#ifndef __TRANSMISSION_SERVER_NET_H__
#define __TRANSMISSION_SERVER_NET_H__

#include <thread>

#include "TransmissionServerInterface.h"

using namespace std;
namespace remote_display {

class TransmissionServerNet : virtual public TransmissionServerIf {
  public:
    TransmissionServerNet(TransmissionServCbkIf* transServCbk);
    ~TransmissionServerNet();

    void start() override;
    void stop() override;

  private:
    bool recvAll(int sock, char* buffer, size_t size);
    int handleData(int sock);
    void transServThread();
    char *mDataBuf{nullptr};
    unsigned mDataLen{0};
    int mEpollFd{-1};
    thread* mTransThread{nullptr};
    TransmissionServCbkIf* mTransServCbk;
};

}  // namespace remote_display

#endif  // __TRANSMISSION_SERVER_NET_H__
