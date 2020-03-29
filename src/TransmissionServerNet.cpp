/*
 * @Descripttion: Transport server net
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-03-30 00:36:09
 */
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <thread>

#include "TransmissionServerNet.h"

using namespace std;

namespace remote_display {
#define MAX_EVENTS 10

TransmissionServerNet::TransmissionServerNet(TransmissionServCbkIf *transServCbk) : mTransServCbk(transServCbk) {
    mEpollfd = -1;
}

TransmissionServerNet::~TransmissionServerNet() {
    if (mDataBuf)
        free(mDataBuf);
    if (mEpollfd != -1)
        close(mEpollfd);
}

void TransmissionServerNet::start() {
    if (mTransThread == nullptr)
        mTransThread = new thread(&TransmissionServerNet::transServThread, this);
    else
        cout << "TransmissionServerNet has been started, do nothing." << endl;
}

void TransmissionServerNet::stop() {
    if (mTransThread) {
        delete mTransThread;
        mTransThread = nullptr;
    }
}

bool TransmissionServerNet::recvAll(int sock, char *buffer, size_t size) {
    while (size > 0) {
        int recvSize = recv(sock, buffer, size, 0);
        if (recvSize <= 0) {
            return false;
        }
        size = size - recvSize;
        buffer += recvSize;
    }
    return true;
}

int TransmissionServerNet::handleData(int sock) {
    if (sock == -1) {
        cerr << "falied!! Invalid sock is " << sock << endl;
        return -1;
    }
    TransHeader transHeader;
    bool isComplete = false;
    isComplete = recvAll(sock, (char *)&transHeader, sizeof(transHeader));
    if (!isComplete) {
        mTransServCbk->devDetach(sock);
        close(sock);
        cerr << "receive error!!" << endl;
        return -1;
    }
    cout << "display header length = " << transHeader.mLength << " display header type = " << transHeader.mType << endl;

    if (mDataLen < transHeader.mLength) {
        mDataBuf = (char *)realloc(mDataBuf, transHeader.mLength);
    }
    if (mDataBuf == nullptr) {
        cerr << "mDataLen is null\n";
        close(sock);
        return -1;
    }

    isComplete = recvAll(sock, mDataBuf, transHeader.mLength);
    if (isComplete) {
        cout << "received data length: " << transHeader.mLength << endl;
        cout << "received data: " << mDataBuf << endl;
        if (mTransServCbk)
            mTransServCbk->dataIsAvailable(transHeader.mType, mDataBuf, transHeader.mLength);
        else
            cerr << "mTransServCbk is null";

    } else {
        mTransServCbk->devDetach(sock);
        close(sock);
        cerr << "receive error!!" << endl;
        return -1;
    }

    return 0;
}

void TransmissionServerNet::transServThread() {
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&option, (socklen_t)sizeof(option)) == -1) {
        printf("setsockopt error!! strerror:%s errno:%d\n", strerror(errno), errno);
        close(listen_sock);
        return;
    }
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_sock, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1) {
        perror("bind");
        return;
    }

    if (listen(listen_sock, 5) == -1) {
        perror("listen");
        return;
    }

    struct epoll_event ev, events[MAX_EVENTS];
    int conn_sock, nfds;

    mEpollfd = epoll_create(10);
    if (mEpollfd == -1) {
        perror("epoll_create");
        return;
    }

    ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLHUP;
    ev.data.fd = listen_sock;
    if (epoll_ctl(mEpollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        return;
    }
    for (;;) {
        nfds = epoll_wait(mEpollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            return;
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].events & EPOLLRDHUP || events[n].events & EPOLLERR || events[n].events & EPOLLHUP) {
                printf("%s @%d epoll_wait error events:0x%08x\n", __FUNCTION__, __LINE__, events[n].events);
                return;
            }
            if (events[n].data.fd == listen_sock) {
                struct sockaddr_in client_addr;
                socklen_t addrlen = sizeof(client_addr);
                conn_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &addrlen);
                if (conn_sock == -1) {
                    perror("accept");
                    return;
                }
                ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLHUP;
                ev.data.fd = conn_sock;
                if (epoll_ctl(mEpollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    return;
                }
                printf("accept successful!\n");
                mTransServCbk->devAttach(conn_sock);
            } else {
                if (handleData(events[n].data.fd) == -1) {
                    if (epoll_ctl(mEpollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL) == -1)
                        return;
                }
            }
        }
    }

    return;
}

}  // namespace remote_display
