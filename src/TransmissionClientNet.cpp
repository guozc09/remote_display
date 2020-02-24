/*
 * @Descripttion: Transport client net
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-02-24 22:46:02
 */
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "TransmissionClientNet.h"

namespace remote_display {

int TransmissionClientNet::transmissionConnect() {
    struct sockaddr_in servaddr;

    if ((mSockCli = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0) {
        return -1;
    }

    for (int connCnt = 1; connCnt <= 5; connCnt++) {
        if (connect(mSockCli, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
            printf("connect error: %s(errno: %d), connect times:%d\n", strerror(errno), errno,
                   connCnt);
        } else {
            printf("connect successful!\n");
            return 0;
        }
    }
    return -1;
}

void TransmissionClientNet::transmissionDisconnect() {
    if (mSockCli == -1) {
        close(mSockCli);
        shutdown(mSockCli, SHUT_RDWR);
        mSockCli = -1;
    }
}

int TransmissionClientNet::sendData(char* data, size_t length) {
    int sendSize = 0;
    if (!data) {
        printf("failed!! data is null\n");
        return -1;
    }

    if (mSockCli == -1) {
        printf("failed!! mSockCli is -1\n");
        return -1;
    }

    while (length > 0) {
        sendSize = send(mSockCli, data, length, 0);
        if (sendSize < 0) {
            printf("send data error: %s(errno: %d)\n", strerror(errno), errno);
            return -1;
        }
        length = length - sendSize;
        data += sendSize;
    }

    return sendSize;
}

}  // namespace remote_display
