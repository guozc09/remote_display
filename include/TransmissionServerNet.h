/*
 * @Descripttion: Transport server net
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-02-24 23:50:38
 */
#ifndef __TRANSMISSION_SERVER_NET_H__
#define __TRANSMISSION_SERVER_NET_H__

#include <poll.h>
#include <thread>

#include "TransmissionServerInterface.h"

#define OPEN_MAX 10

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
    int handleData(int& sock);
    void transServThread();
    char *mDataBuf{nullptr};
    unsigned mDataLen{0};
    struct pollfd mClient[OPEN_MAX];
    thread* mTransThread{nullptr};
    TransmissionServCbkIf* mTransServCbk;
};

}  // namespace remote_display

#endif  // __TRANSMISSION_SERVER_NET_H__
