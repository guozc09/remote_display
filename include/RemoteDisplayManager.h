/*
 * @Descripttion: RemoteDisplay manager module
 * @version: 0.0.1
 * @Author: Zhc Guo
 * @Date: 2020-01-14 13:29:38
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-02-25 21:35:35
 */
#ifndef __REMOTE_DISPLAY_MANAGER_H__
#define __REMOTE_DISPLAY_MANAGER_H__

#include <map>
#include <string>
#include "RemoteDisplay.h"
#include "TransmissionCommon.h"

using namespace std;
namespace remote_display {

class RemoteDisplayManager {
  public:
    RemoteDisplayManager(const RemoteDisplayManager&) = delete;
    RemoteDisplayManager& operator=(const RemoteDisplayManager&) = delete;
    ~RemoteDisplayManager() = default;
    static RemoteDisplayManager& getInstance() {
        static RemoteDisplayManager instance;
        return instance;
    }
    RemoteDisplay* getRemoteDisplay(string name, RemoteDisplayRole role, TransmissonType type,
                                    shared_ptr<RemoteDisplay> player = nullptr,
                                    shared_ptr<RemoteDisplayDev> dev = nullptr);
    void releaseRemoteDisplay(string name);

  private:
    RemoteDisplayManager() = default;
    map<string, RemoteDisplay*> mRemoteDisplayMap;
};

}  // namespace remote_display

#endif // __REMOTE_DISPLAY_MANAGER_H__
