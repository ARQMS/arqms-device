#include "DeviceHandler.h"

DeviceHandler::DeviceHandler(WiFiManager& wifiManager, DeviceStorage& storage) :
    wifiManager(wifiManager),
    storage(storage) {
}

bool DeviceHandler::isWifiCfgValid() {
    return wifiManager.isSTACfgValid();
}