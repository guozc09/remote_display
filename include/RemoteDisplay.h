/*
 * @Descripttion: Remote video playback interface
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-11 10:30:09
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-02-25 21:33:30
 */
#ifndef __REMOTE_DISPLAY_H__
#define __REMOTE_DISPLAY_H__

#include <stdint.h>
#include <memory>
#include "TransmissionClientInterface.h"
#include "TransmissionServerInterface.h"

using std::shared_ptr;

namespace remote_display {

enum RemoteDisplayRole {
    ROLE_SENDER = 0,
    ROLE_RECEVIER,
};

class RemoteDisplay {
  public:
    virtual ~RemoteDisplay() = default;
    virtual int setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) = 0;
    virtual int processFrame(uint8_t *data, size_t length) = 0;
};

class RemoteDisplayDev {
  public:
    virtual ~RemoteDisplayDev() = default;
    virtual void attach(int fd) = 0;
    virtual void detach(int fd) = 0;
};

class RemoteDisplaySender : virtual public RemoteDisplay {
  public:
    RemoteDisplaySender(TransmissonType type);
    ~RemoteDisplaySender();
    int setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) override;
    int processFrame(uint8_t *data, size_t length) override;

  private:
    TransmissionClientIf *mTransCli{nullptr};
};

class RemoteDisplayReceiver : virtual public RemoteDisplay {
  public:
    RemoteDisplayReceiver(TransmissonType type, shared_ptr<RemoteDisplay> &player, shared_ptr<RemoteDisplayDev> &dev);
    ~RemoteDisplayReceiver();
    int setParam(uint32_t widthPixels, uint32_t heightPixels, uint32_t fps) override;
    int processFrame(uint8_t *data, size_t length) override;

    shared_ptr<RemoteDisplayDev> mDev;
  private:
    TransmissionServCbkIf *mTransServCbk;
    TransmissionServerIf *mTransServ;
    shared_ptr<RemoteDisplay> mPlayer;
};

}  // namespace remote_display

#endif // __REMOTE_DISPLAY_H__
