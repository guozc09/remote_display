/*
 * @Descripttion: RemoteDisplay manager test module
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-14 17:18:36
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-15 17:14:29
 */
#include <unistd.h>
#include <iostream>
#include "RemoteDisplayManager.h"

using namespace std;
using namespace remote_display;

class RemoteDisplayPlayerImpl : virtual public RemoteDisplayPlayer {
  public:
    RemoteDisplayPlayerImpl() = default;
    ~RemoteDisplayPlayerImpl() = default;
    int setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) {
        cout << __FILE__ << " [" << __FUNCTION__ << "]@line[" << __LINE__
             << "] widthPixels: " << widthPixels << " heightPixels: " << heightPixels << endl;
    }
    int processFrame(uint8_t* data, size_t length) {
        cout << __FILE__ << " [" << __FUNCTION__ << "]@line[" << __LINE__ << "] data: " << data
             << " ,length: " << length << endl;
    }
};

static void* receivedThread(void* arg) {
    RemoteDisplayManager& instance = RemoteDisplayManager::getInstance();
    string name = "local8368UReceiver";
    shared_ptr<RemoteDisplayPlayer> player =
        shared_ptr<RemoteDisplayPlayer>(new RemoteDisplayPlayerImpl());
    RemoteDisplay* remoteDisplay =
        instance.getRemoteDisplay(name, ROLE_RECEVIER, TYPE_SOCKET_NET, player);
    return nullptr;
}

static void* sendThread(void* arg) {
    RemoteDisplayManager& instance = RemoteDisplayManager::getInstance();
    string name = "local8368USender";
    RemoteDisplay* remoteDisplay =
        instance.getRemoteDisplay(name, ROLE_SENDER, TYPE_SOCKET_NET);
    uint8_t dataBuf[] = "12345";
    while (1) {
        remoteDisplay->setParam(1024, 600, 30);
        remoteDisplay->processFrame(dataBuf, sizeof(dataBuf));
        sleep(1);
    }
    return nullptr;
}

int main(int argc, const char** argv) {
    pthread_t receiveTid;
    pthread_t sendTid;
    pthread_create(&receiveTid, nullptr, receivedThread, nullptr);

    sleep(1);

    pthread_create(&sendTid, nullptr, sendThread, nullptr);

    pthread_join(sendTid, nullptr);
    pthread_join(receiveTid, nullptr);

    return 0;
}
