/*
 * @Descripttion: RemoteDisplay manager test module
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-14 17:18:36
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-03-14 17:00:42
 */
#include <unistd.h>
#include <iostream>
#include <thread>
#include "RemoteDisplayManager.h"

using namespace std;
using namespace remote_display;

class RemoteDisplayPlayerImpl : virtual public RemoteDisplay {
  public:
    RemoteDisplayPlayerImpl() = default;
    ~RemoteDisplayPlayerImpl() = default;
    int setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) {
        cout << __FILE__ << " [" << __FUNCTION__ << "]@line[" << __LINE__
             << "] widthPixels: " << widthPixels << " heightPixels: " << heightPixels << endl;
        return 0;
    }
    int processFrame(uint8_t* data, size_t length) {
        cout << __FILE__ << " [" << __FUNCTION__ << "]@line[" << __LINE__ << "] data: " << data
             << " ,length: " << length << endl;
        return 0;
    }
};

class RemoteDisplayDevImpl : virtual public RemoteDisplayDev {
  public:
    RemoteDisplayDevImpl() = default;
    ~RemoteDisplayDevImpl() = default;
    void attach(int fd) override {
        cout << __FILE__ << " [" << __FUNCTION__ << "]@line[" << __LINE__
             << "] fd:" << fd << " attached!!\n";
    }
    void detach(int fd) override {
        cout << __FILE__ << " [" << __FUNCTION__ << "]@line[" << __LINE__
             << "] fd:" << fd << " detached!!\n";
    }
};

static void receivedThread() {
    RemoteDisplayManager& instance = RemoteDisplayManager::getInstance();
    string name = "RDReceiver";
    shared_ptr<RemoteDisplay> player = shared_ptr<RemoteDisplay>(new RemoteDisplayPlayerImpl());
    shared_ptr<RemoteDisplayDev> dev = shared_ptr<RemoteDisplayDev>(new RemoteDisplayDevImpl());
    instance.getRemoteDisplay(name, ROLE_RECEVIER, TYPE_SOCKET_NET, player, dev);
    return;
}

static void sendThread() {
    RemoteDisplayManager& instance = RemoteDisplayManager::getInstance();
    string name = "RDSender";
    RemoteDisplay* remoteDisplay = instance.getRemoteDisplay(name, ROLE_SENDER, TYPE_SOCKET_NET);
    uint8_t dataBuf[] = "12345";
    while (1) {
        remoteDisplay->setParam(1024, 600, 30);
        remoteDisplay->processFrame(dataBuf, sizeof(dataBuf));
        sleep(1);
    }
    return;
}

int main(int argc, const char** argv) {
    thread receiveTid(receivedThread);
    sleep(1);
    thread sendTid(sendThread);

    sendTid.join();
    receiveTid.join();

    return 0;
}
