/*
 * @Descripttion: TransmissionServer test module
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-13 12:34:38
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-13 19:22:21
 */
#include "stdio.h"
#include "TransmissionServer.h"

using namespace remote_display;

int main() {
    TransmissionServer server;
    char dataBuf[1024];
    int dataLen = 0;
    if (server.transmissionConnect(6666) < 0) {
        return 0;
    }

    while (1) {
        dataLen = server.receiveData(dataBuf, sizeof(dataBuf));
        if (dataLen > 0) {
            printf("data length[%d], data: %s\n", dataLen, dataBuf);
        }
    }

    server.transmissionDisconnect();
    return 0;
}
