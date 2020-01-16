/*
 * @Descripttion: TransmissionClient test module
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-13 12:34:54
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-13 18:51:18
 */
#include <unistd.h>
#include "TransmissionClient.h"

using namespace remote_display;

int main()
{
    TransmissionClient client;
    int sendDataTimes = 100;
    char sendBuf[] = "Hello world!!";
    if (client.transmissionConnect("192.168.1.208", 6666) < 0)
    {
        return 0;
    }

    while (sendDataTimes--)
    {
        client.sendData(sendBuf, sizeof(sendBuf));
        sleep(1);
    }

    client.transmissionDisconnect();
    return 0;
}
