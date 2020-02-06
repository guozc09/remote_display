/*
 * @Descripttion: RemoteDisplay manager module
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-14 13:29:54
 * @LastEditors  : Zhc Guo
 * @LastEditTime : 2020-01-15 17:06:14
 */
#include "RemoteDisplayManager.h"

namespace remote_display {

RemoteDisplay* RemoteDisplayManager::getRemoteDisplay(string name, RemoteDisplayRole role,
                                                      TransmissonType type,
                                                      shared_ptr<RemoteDisplayPlayer> player) {
    map<string, RemoteDisplay*>::iterator iter = mRemoteDisplayMap.find(name);
    if (iter != mRemoteDisplayMap.end()) {
        return iter->second;
    } else {
        RemoteDisplay* remoteDisplay = nullptr;
        if (role == ROLE_SENDER) {
            remoteDisplay = new RemoteDisplaySender(type);
        } else {
            remoteDisplay = new RemoteDisplayReceiver(type, player);
        }
        mRemoteDisplayMap.insert(pair<string, RemoteDisplay*>(name, remoteDisplay));
        return remoteDisplay;
    }
}

void RemoteDisplayManager::releaseRemoteDisplay(string name) {
    map<string, RemoteDisplay*>::iterator iter = mRemoteDisplayMap.find(name);
    if (iter != mRemoteDisplayMap.end()) {
        delete iter->second;
        mRemoteDisplayMap.erase(iter);
    }
}

}  // namespace remote_display
