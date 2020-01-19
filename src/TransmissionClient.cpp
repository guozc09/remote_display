/*
 * @Descripttion: Transport client
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-19 20:00:06
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

#include "TransmissionClient.h"

namespace remote_display {

int TransmissionClientNet::transmissionConnect() {
    struct sockaddr_in servaddr;

    if ((mSockClient = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0) {
        return -1;
    }

    if (connect(mSockClient, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
        return -1;
    } else {
        printf("connect successful!\n");
        return 0;
    }
}

void TransmissionClientNet::transmissionDisconnect() {
    close(mSockClient);
}

int TransmissionClientNet::sendData(char* data, size_t length) {
    int sendSize = 0;
    if (!data) {
        printf("failed!! data is null\n");
        return -1;
    }

    while (length > 0)
    {
        sendSize = send(mSockClient, data, length, 0);
        if(sendSize < 0) {
            printf("send image error: %s(errno: %d)\n", strerror(errno), errno);
            return -1;
        }
        length = length - sendSize;
        data += sendSize;
    }

    return sendSize;
}

}  // namespace remote_display
