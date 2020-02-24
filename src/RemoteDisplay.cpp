/*
 * @Descripttion: Remote video playback interface
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-11 10:30:24
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-02-25 00:31:10
 */
#include "RemoteDisplay.h"
#include <iostream>
#include "TransmissionClientNet.h"
#include "TransmissionCommon.h"
#include "TransmissionServerNet.h"

using namespace std;
namespace remote_display {

struct DisplayParam {
    DisplayParam() = default;
    DisplayParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps)
        : mWidthPixels(widthPixels), mHeightPixels(heightPixels), mFps(fps) {
    }
    uint32_t mWidthPixels;
    uint32_t mHeightPixels;
    uint32_t mFps;
};

class TransmissionServCbk : virtual public TransmissionServCbkIf {
  public:
    TransmissionServCbk(RemoteDisplayReceiver *RDReceiver) : mRDReceiver(RDReceiver) {
    }
    ~TransmissionServCbk() = default;
    int dataIsAvailable(uint32_t type, char *data, size_t dataLen) override {
        switch (type) {
            case TYPE_PARAM: {
                DisplayParam *param = (DisplayParam *)data;
                mRDReceiver->setParam(param->mWidthPixels, param->mHeightPixels, param->mFps);
                break;
            }
            case TYPE_DATA: {
                mRDReceiver->processFrame((uint8_t *)data, dataLen);
                break;
            }
            default: {
                cout << "Types not currently supported!! type[" << type << "]" << endl;
                break;
            }
        }
    }

  private:
    RemoteDisplayReceiver *mRDReceiver;
};  // namespace remote_display

RemoteDisplaySender::RemoteDisplaySender(TransmissonType type) {
    switch (type) {
        case TYPE_SOCKET_NET:
            mTransCli = new TransmissionClientNet();
            break;
        default:
            cerr << "failed!! Not currently supported type: " << type << endl;
            break;
    }
    mTransCli->transmissionConnect();
}

RemoteDisplaySender::~RemoteDisplaySender() {
    if (mTransCli) {
        mTransCli->transmissionDisconnect();
        delete mTransCli;
    }
}

int RemoteDisplaySender::setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) {
    if (mTransCli) {
        TransHeader transHeader(sizeof(DisplayParam), TYPE_PARAM);
        DisplayParam displayParam(widthPixels, heightPixels, fps);
        mTransCli->sendData((char *)(&transHeader), sizeof(TransHeader));
        mTransCli->sendData((char *)(&displayParam), sizeof(DisplayParam));
        return 0;
    }
    return -1;
}

int RemoteDisplaySender::processFrame(uint8_t *data, size_t length) {
    if (mTransCli) {
        TransHeader transHeader(length, TYPE_DATA);
        mTransCli->sendData((char *)(&transHeader), sizeof(TransHeader));
        mTransCli->sendData((char *)data, length);
        return 0;
    }
    return -1;
}

RemoteDisplayReceiver::RemoteDisplayReceiver(TransmissonType type,
                                             shared_ptr<RemoteDisplay> &player) {
    switch (type) {
        case TYPE_SOCKET_NET:
            mTransServCbk = new TransmissionServCbk(this);
            mTransServ = new TransmissionServerNet(mTransServCbk);
            break;
        default:
            mTransServCbk = nullptr;
            mTransServ = nullptr;
            cerr << "failed!! Not currently supported type: " << type << endl;
            break;
    }
    mPlayer = player;
    mTransServ->start();
}

RemoteDisplayReceiver::~RemoteDisplayReceiver() {
    if (mTransServCbk) {
        delete mTransServCbk;
    }
    if (mTransServ) {
        mTransServ->stop();
        delete mTransServ;
    }
}

int RemoteDisplayReceiver::setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) {
    if (mPlayer) {
        mPlayer->setParam(widthPixels, heightPixels, fps);
        return 0;
    }
    return -1;
}

int RemoteDisplayReceiver::processFrame(uint8_t *data, size_t length) {
    if (mPlayer) {
        mPlayer->processFrame(data, length);
        return 0;
    }
    return -1;
}

}  // namespace remote_display
