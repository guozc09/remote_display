/*
 * @Descripttion: Transport server interface
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-02-25 00:15:36
 */
#ifndef __TRANSMISSION_SERVER_INTERFACE_H__
#define __TRANSMISSION_SERVER_INTERFACE_H__

#include <stddef.h>
#include <stdint.h>
#include "TransmissionCommon.h"

using namespace std;
namespace remote_display {

class TransmissionServCbkIf {
  public:
    virtual ~TransmissionServCbkIf() = default;
    virtual int dataIsAvailable(uint32_t type, char *data, size_t dataLen) = 0;
};

class TransmissionServerIf {
  public:
    virtual ~TransmissionServerIf() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};

}  // namespace remote_display

#endif  // __TRANSMISSION_SERVER_INTERFACE_H__
