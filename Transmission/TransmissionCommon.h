/*
 * @Descripttion: Transmisson common header
 * @version:
 * @Author: Zhc Guo
 * @Date: 2020-01-14 15:37:06
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-15 15:07:02
 */
#pragma once
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

struct DisplayHeader {
    DisplayHeader() = default;
    DisplayHeader(uint32_t length, uint32_t type) : mLength(length), mType(type) {
    }
    uint32_t mLength;
    uint32_t mType;
    uint32_t reserved[6];
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

}  // namespace remote_display
