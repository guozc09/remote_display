/*
 * @Description: Transport client net
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-06-04 22:04:49
 */
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "TransmissionClientNet.h"

namespace remote_display {
#define MAX_EVENTS 10

int TransmissionClientNet::transmissionConnect() {
    int ret = 0;
    struct sockaddr_in servaddr;

    if ((mSockCli = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        ret = -1;
        goto exit;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0) {
        ret = -1;
        goto exit;
    }

    for (int connCnt = 1; connCnt <= 5; connCnt++) {
        int err = connect(mSockCli, (struct sockaddr*)&servaddr, sizeof(servaddr));
        if (err && err != EINPROGRESS) {
            printf("connect error: %s(errno: %d), connect times:%d\n", strerror(errno), errno, connCnt);
            ret = -1;
        } else {
            printf("connect successful!\n");
            setnonblocking(mSockCli);
            mEpollFd = epoll_create(10);
            if (mEpollFd == -1) {
                perror("epoll_create");
                ret = -1;
                goto exit;
            }
            struct epoll_event ev;
            ev.events = EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLIN;
            ev.data.fd = mSockCli;
            if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mSockCli, &ev) == -1) {
                perror("epoll_ctl: mSockCli");
                close(mEpollFd);
                mEpollFd = -1;
                ret = -1;
                goto exit;
            }
            break;
        }
    }
exit:
    if (ret == -1) {
        close(mSockCli);
        mSockCli = -1;
    }
    return ret;
}

void TransmissionClientNet::transmissionDisconnect() {
    if (mSockCli != -1) {
        shutdown(mSockCli, SHUT_RDWR);
        close(mSockCli);
        mSockCli = -1;
    }
    if (mEpollFd != -1) {
        close(mEpollFd);
        mEpollFd = -1;
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
    int timeout = 1000;  // ms
    struct epoll_event events[MAX_EVENTS];
    while (length > 0) {
        int nfds = epoll_wait(mEpollFd, events, MAX_EVENTS, timeout);
        if (nfds == -1) {
            perror("epoll_wait");
            return -1;
        } else if (nfds == 0) {  // timeout
            printf("epoll_wait timeout:%d ms\n", timeout);
            return -1;
        }
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == mSockCli) {
                if (events[n].events & EPOLLHUP || events[n].events & EPOLLERR || events[n].events & EPOLLIN) { // EPOLLIN is to handle peer socket close.
                    printf("%s @%d epoll_wait error events:0x%08x\n", __FUNCTION__, __LINE__, events[n].events);
                    return -1;
                }
                sendSize = send(mSockCli, data, length, 0);
                if (sendSize < 0) {
                    perror("send data");
                    return -1;
                }
                length = length - sendSize;
                data += sendSize;
            }
        }
    }

    return sendSize;
}

void TransmissionClientNet::setnonblocking(int sock) {
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        perror("fcntl(sock,GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(sock,SETFL,opts)");
        exit(EXIT_FAILURE);
    }
}

}  // namespace remote_display
