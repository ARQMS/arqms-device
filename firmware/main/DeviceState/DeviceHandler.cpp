#include "DeviceHandler.h"

#include "esp_system.h"
#include "esp_sleep.h"

DeviceHandler::DeviceHandler(WiFiManager& wifiManager, SensorManager& sensor, DeviceStorage& storage) :
    wifiManager(wifiManager),
    sensor(sensor),
    storage(storage),
    updateInstallationFailed(false),
    deviceStatePublished(false),
    sensorDataPublished(false) {
}

bool DeviceHandler::isWifiCfgValid() const {
    return wifiManager.isWifiCfgValid();
}

bool DeviceHandler::isAllTaskDone() const {
    bool allTaskDone = true;

    // no firmware update to install or installation failed
    allTaskDone &= !isNewVersionAvailable() || updateInstallationFailed;
    allTaskDone &= isDataPublished();

    return allTaskDone;
}

bool DeviceHandler::isDataPublished() const {
    bool allTaskDone = true;

    allTaskDone &= deviceStatePublished;
    allTaskDone &= sensorDataPublished;

    return allTaskDone;
}

bool DeviceHandler::isNewVersionAvailable() const {
    return false; // TODO
}

bool DeviceHandler::isFirmwareUpdateFailed() const {
    return updateInstallationFailed;
}

bool DeviceHandler::anyClientConnected() const {
    return wifiManager.countClients() > 0;
}

WiFiState DeviceHandler::getWifiState() const {
    return wifiManager.getWifiState();
}

void DeviceHandler::connectToWiFi() {
    // TODO

    // WifiParameters config = storage.getWifiParameters();
    // wifiManager.start(STA);
    // wifiManager.connect(config);
}

void DeviceHandler::enableAPMode() {
    // TODO

    // wifiManager.start(AP);
}

void DeviceHandler::startDataAcquisition() {
    sensor.startDataAcquisition();
}

void DeviceHandler::startFirmwareUpdate() {
    if (!isNewVersionAvailable()) return;

    // TODO Create UpdateManager (similar to WifiManager) and call updateManager.install(url);
    // url is given by MQTT packete (security fail?!? when someone can send a MQTT packet, a bad firmware
    // can be installed?)

    //  https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/system/ota.html

    updateInstallationFailed = true; 
    // updateInstallationFailed = !updateManager.install(FIRMWARE_URL);

    if (!updateInstallationFailed) {
        esp_restart(); // does not return!
    }
}

void DeviceHandler::enterSleep() {
    // TODO
    
    // // configure sleep mode
    // UserSettings settings = storage.getUserSettings();
    // esp_deep_sleep_enable_timer_wakeup(settings.sleepInterval * 1000);
    // esp_deep_sleep_enable_gpio_wakeup(0x3, ESP_GPIO_WAKEUP_GPIO_LOW);
    // 
    // // start sleep mode, does not return!
    // esp_deep_sleep_start();
}