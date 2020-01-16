/*
 * @Descripttion: Transport server
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-12 12:37:35
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-17 00:46:14
 */
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
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

#include "TransmissionServer.h"

using namespace std;

namespace remote_display {

void TransmissionServerNet::receiveThread() {
    if (mSockConn == -1) {
        cerr << "falied!! Invalid mSockConn is " << mSockConn << endl;
        return;
    }
    DisplayHeader displayHeader;

    while (mIsRun) {
        size_t dataLen = 0;
        dataLen = recv(mSockConn, (void *)&displayHeader, sizeof(displayHeader), 0);
        cout << "display header length = " << displayHeader.mLength
             << " display header type = " << displayHeader.mType << endl;
        switch (displayHeader.mType) {
            case TYPE_PARAM: {
                DisplayParam *param = new DisplayParam();
                dataLen = recv(mSockConn, (void *)param, displayHeader.mLength, 0);
                cout << " mWidthPixels: " << param->mWidthPixels
                     << " mHeightPixels: " << param->mHeightPixels << " mFps: " << param->mFps
                     << endl;
                if (mTransmissionHandler)
                    mTransmissionHandler->setParam(param->mWidthPixels, param->mHeightPixels,
                                                   param->mFps);
                delete param;
                break;
            }
            case TYPE_DATA: {
                char *data = new char[displayHeader.mLength];
                dataLen = recv(mSockConn, (void *)data, displayHeader.mLength, 0);
                cout << "received data length: " << dataLen << endl;
                cout << "received data: " << data << endl;
                if (mTransmissionHandler)
                    mTransmissionHandler->processFrame((uint8_t *)data, dataLen);
                delete[] data;
                break;
            }
            default:
                cout << "Types not currently supported!! type[" << displayHeader.mType << "]"
                     << endl;
                break;
        }
    }
    return;
}

TransmissionServerNet::TransmissionServerNet(TransmissionHandler *transmissionHandler)
    : mTransmissionHandler(transmissionHandler) {
}

void TransmissionServerNet::start() {
    mIsRun = true;
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

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

    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    mSockConn = accept(server_sockfd, (struct sockaddr *)&client_addr, &length);
    if (mSockConn < 0) {
        perror("connect error!!");
        close(server_sockfd);
        return;
    } else {
        printf("connect successful!\n");
        close(server_sockfd);
    }

    mRecvThread = new thread(&TransmissionServerNet::receiveThread, this);
}

void TransmissionServerNet::stop() {
    mIsRun = false;
    if (mRecvThread) {
        mRecvThread->join();
        delete mRecvThread;
        mRecvThread = nullptr;
    }
    if (mSockConn != -1) {
        close(mSockConn);
    }
}

}  // namespace remote_display
