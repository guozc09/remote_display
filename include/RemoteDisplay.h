/*
 * @Descripttion: Remote video playback interface
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-11 10:30:09
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-15 17:00:00
 */
#ifndef __REMOTE_DISPLAY_H__
#define __REMOTE_DISPLAY_H__

#include <stdint.h>
#include <memory>
#include "TransmissionClient.h"
#include "TransmissionServer.h"

using std::shared_ptr;

namespace remote_display {

enum RemoteDisplayRole {
    ROLE_SENDER = 0,
    ROLE_RECEVIER,
};

class RemoteDisplayPlayer {
  public:
    virtual int setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) = 0;
    virtual int processFrame(uint8_t *data, size_t length) = 0;
};

class RemoteDisplay {
  public:
    virtual int setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) = 0;
    virtual int processFrame(uint8_t *data, size_t length) = 0;
};

class RemoteDisplaySender : virtual public RemoteDisplay {
  public:
    RemoteDisplaySender(TransmissonType type);
    ~RemoteDisplaySender();
    int setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) override;
    int processFrame(uint8_t *data, size_t length) override;

  private:
    TransmissionClient *mTransmissionClient{nullptr};
};

class RemoteDisplayReceiver : virtual public RemoteDisplay, virtual public TransmissionHandler {
  public:
    RemoteDisplayReceiver(TransmissonType type, shared_ptr<RemoteDisplayPlayer> &player);
    ~RemoteDisplayReceiver();
    int setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) override;
    int processFrame(uint8_t *data, size_t length) override;

  private:
    TransmissionServer *mTransmissionServer{nullptr};
    shared_ptr<RemoteDisplayPlayer> mPlayer{nullptr};
};

}  // namespace remote_display

#endif // __REMOTE_DISPLAY_H__
