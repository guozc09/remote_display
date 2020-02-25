/*
 * @Descripttion: Transport server net
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-02-25 21:25:29
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
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "TransmissionServerNet.h"

#define INFTIM 1024

using namespace std;

namespace remote_display {

TransmissionServerNet::TransmissionServerNet(TransmissionServCbkIf *transServCbk) : mTransServCbk(transServCbk) {
    for (int i = 0; i < OPEN_MAX; i++) {
        mClient[i].fd = -1;
    }
}

TransmissionServerNet::~TransmissionServerNet() {
    if (mDataBuf)
        free(mDataBuf);
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
    for (int i = 0; i < OPEN_MAX; i++) {
        if (mClient[i].fd != -1) {
            close(mClient[i].fd);
            mClient[i].fd = -1;
        }
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

int TransmissionServerNet::handleData(int &sock) {
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
        sock = -1;
        cerr << "receive error!!" << endl;
        return -1;
    }
    cout << "display header length = " << transHeader.mLength
         << " display header type = " << transHeader.mType << endl;

    if (mDataLen < transHeader.mLength) {
        mDataBuf = (char *)realloc(mDataBuf, transHeader.mLength);
    }
    if (mDataBuf == nullptr) {
        cerr << "mDataLen is null\n";
        close(sock);
        sock = -1;
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
        sock = -1;
        cerr << "receive error!!" << endl;
        return -1;
    }

    return 0;
}

void TransmissionServerNet::transServThread() {
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&option,
                   (socklen_t)sizeof(option)) == -1) {
        printf("setsockopt error!! strerror:%s errno:%d\n", strerror(errno), errno);
        close(server_sockfd);
        return;
    }
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sockfd, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1) {
        perror("bind");
        return;
    }

    if (listen(server_sockfd, 5) == -1) {
        perror("listen");
        return;
    }

    mClient[0].fd = server_sockfd;
    mClient[0].events = POLLRDNORM;  // poll read normal

    int maxi = 0;
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    while (true) {
        int nready = poll(mClient, maxi + 1, INFTIM);
        if (mClient[0].revents & POLLRDNORM) {
            // this means new client connection request come
            int connfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &length);
            if (connfd < 0) {
                perror("accept error!!");
                continue;
            } else {
                printf("accept successful!\n");
                mTransServCbk->devAttach(connfd);
            }
            // updates maxi, and check if i out of limition of OPEN_MAX
            int i;
            for (i = 1; i < OPEN_MAX; i++) {
                if (mClient[i].fd < 0) {
                    mClient[i].fd = connfd;
                    mClient[i].events = (POLLRDNORM | POLLERR);
                    break;
                }
            }
            if (i == OPEN_MAX) {
                perror("too many client requests ");
                return;
            }
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;
        }
        for (int i = 1; i <= maxi; i++) {
            if (mClient[i].fd < 0)
                continue;  // continue the sub for cycle
            if (mClient[i].revents & (POLLRDNORM | POLLERR)) {
                if (handleData(mClient[i].fd) == -1)
                    return;
            }
        }
    }
    return;
}

}  // namespace remote_display
