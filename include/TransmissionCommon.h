/*
 * @Descripttion: Transmisson common header
 * @version:
 * @Author: Zhc Guo
 * @Date: 2020-01-14 15:37:06
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-02-25 00:16:18
 */
#ifndef __TRANSMISSION_COMMON_H__
#define __TRANSMISSION_COMMON_H__

#include <stdint.h>
namespace remote_display {

#define IP "127.0.0.1"
#define PORT 6666

enum TransmissonType {
    TYPE_SOCKET_LOOPBACK = 0,
    TYPE_SOCKET_NET = 1,
};

enum DataType {
    TYPE_PARAM = 0,
    TYPE_DATA,
};

struct TransHeader {
    TransHeader() = default;
    TransHeader(uint32_t length, uint32_t type) : mLength(length), mType(type) {
    }
    uint32_t mLength;
    uint32_t mType;
    uint32_t reserved[6];
};

}  // namespace remote_display

#endif // __TRANSMISSION_COMMON_H__
