/*
 * @Descripttion: Transport client interface
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-02-24 22:05:17
 */
#ifndef __TRANSMISSION_CLIENT_INTERFACE_H__
#define __TRANSMISSION_CLIENT_INTERFACE_H__

#include <stdint.h>
#include <stddef.h>
#include "TransmissionCommon.h"

namespace remote_display {

class TransmissionClientIf {
  public:
    virtual ~TransmissionClientIf() = default;
    virtual int transmissionConnect() = 0;
    virtual void transmissionDisconnect() = 0;
    virtual int sendData(char* data, size_t length) = 0;
};

}  // namespace remote_display

#endif // __TRANSMISSION_CLIENT_INTERFACE_H__
