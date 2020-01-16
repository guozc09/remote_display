/*
 * @Descripttion: Remote video playback interface
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-11 10:30:24
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-17 00:49:19
 */
#include "RemoteDisplay.h"
#include <iostream>

using namespace std;
namespace remote_display {

RemoteDisplaySender::RemoteDisplaySender(TransmissonType type) {
    switch (type) {
        case TYPE_SOCKET_NET:
            mTransmissionClient = new TransmissionClientNet();
            break;
        default:
            cerr << "failed!! Not currently supported type: " << type << endl;
            break;
    }
    mTransmissionClient->transmissionConnect();
}

RemoteDisplaySender::~RemoteDisplaySender() {
    if (mTransmissionClient) {
        mTransmissionClient->transmissionDisconnect();
        delete mTransmissionClient;
    }
}

int RemoteDisplaySender::setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) {
    if (mTransmissionClient) {
        DisplayHeader *displayHeader = new DisplayHeader(sizeof(DisplayParam), TYPE_PARAM);
        DisplayParam *displayParam = new DisplayParam(widthPixels, heightPixels, fps);
        mTransmissionClient->sendData((char *)displayHeader, sizeof(DisplayHeader));
        mTransmissionClient->sendData((char *)displayParam, sizeof(DisplayParam));
        delete displayHeader;
        delete displayParam;
    }
}

int RemoteDisplaySender::processFrame(uint8_t *data, size_t length) {
    if (mTransmissionClient) {
        DisplayHeader *displayHeader = new DisplayHeader(length, TYPE_DATA);
        mTransmissionClient->sendData((char *)displayHeader, sizeof(DisplayHeader));
        mTransmissionClient->sendData((char *)data, length);
        delete displayHeader;
    }
}

RemoteDisplayReceiver::RemoteDisplayReceiver(TransmissonType type,
                                             shared_ptr<RemoteDisplayPlayer> &player) {
    switch (type) {
        case TYPE_SOCKET_NET:
            mTransmissionServer = new TransmissionServerNet(this);
            break;
        default:
            cerr << "failed!! Not currently supported type: " << type << endl;
            break;
    }
    mPlayer = player;
    mTransmissionServer->start();
}

RemoteDisplayReceiver::~RemoteDisplayReceiver() {
    if (mTransmissionServer) {
        mTransmissionServer->stop();
        delete mTransmissionServer;
    }
}

int RemoteDisplayReceiver::setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) {
    if (mPlayer) {
        mPlayer->setParam(widthPixels, heightPixels, fps);
    }
    return 0;
}

int RemoteDisplayReceiver::processFrame(uint8_t *data, size_t length) {
    if (mPlayer) {
        mPlayer->processFrame(data, length);
    }
}

}  // namespace remote_display
