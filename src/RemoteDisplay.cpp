/*
 * @Descripttion: Remote video playback interface
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-11 10:30:24
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-03-29 23:43:10
 */
#include "RemoteDisplay.h"
#include <iostream>
#include "TransmissionClientNet.h"
#include "TransmissionCommon.h"
#include "TransmissionServerNet.h"

using namespace std;
namespace remote_display {

enum DataType {
    TYPE_PARAM = 0,
    TYPE_DATA,
};

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
        return 0;
    }
    void devAttach(int fd) override {
        mRDReceiver->mDev->attach(fd);
    }
    void devDetach(int fd) override {
        mRDReceiver->mDev->detach(fd);
    }

  private:
    RemoteDisplayReceiver *mRDReceiver;
};

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
    int ret = 0;
    if (mTransCli) {
        TransHeader transHeader(sizeof(DisplayParam), TYPE_PARAM);
        DisplayParam displayParam(widthPixels, heightPixels, fps);
        ret = mTransCli->sendData((char *)(&transHeader), sizeof(TransHeader));
        if (ret < 0) {
            goto exit;
        }
        ret = mTransCli->sendData((char *)(&displayParam), sizeof(DisplayParam));
        if (ret < 0) {
            goto exit;
        }
    }
exit:
    return (ret < 0 ? -1 : 0);
}

int RemoteDisplaySender::processFrame(uint8_t *data, size_t length) {
    int ret = 0;
    if (mTransCli) {
        TransHeader transHeader(length, TYPE_DATA);
        ret = mTransCli->sendData((char *)(&transHeader), sizeof(TransHeader));
        if (ret < 0) {
            goto exit;
        }
        ret = mTransCli->sendData((char *)data, length);
        if (ret < 0) {
            goto exit;
        }
    }
exit:
    return (ret < 0 ? -1 : 0);
}

RemoteDisplayReceiver::RemoteDisplayReceiver(TransmissonType type,
                                             shared_ptr<RemoteDisplay> &player,
                                             shared_ptr<RemoteDisplayDev> &dev) {
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
    mDev = dev;
    if (mTransServ)
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
    int ret = 0;
    if (mPlayer) {
        ret = mPlayer->setParam(widthPixels, heightPixels, fps);
    }
    return ret;
}

int RemoteDisplayReceiver::processFrame(uint8_t *data, size_t length) {
    int ret = 0;
    if (mPlayer) {
        ret = mPlayer->processFrame(data, length);
    }
    return ret;
}

}  // namespace remote_display
