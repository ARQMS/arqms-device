#include "ControlTask.h"

#include "esp_sleep.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiSettingsEvent.h"
#include "Events/DeviceInfoEvent.h"
#include "Events/DeviceStatusEvent.h"
    
StorageDriverIfc* ControlTask::s_pNvsStorageDriver = NULL;

ControlTask::ControlTask() :
    Gui(),
    CloudLink(),
    m_coreSm(),
    m_lastWifiStatus(),
    m_lastBatteryStatus(),
    m_wifiReceived(false),
    m_batteryReceived(false),
    m_deviceStatusSent(false),
    m_sensorStatusSent(false),
    m_displayOn(false) {
}

ControlTask::~ControlTask() {
}

void ControlTask::onInitialize()  {
    assert(s_pNvsStorageDriver != NULL);
}

void ControlTask::onStart() {
    WifiSettingsEvent wifiSettings;
    s_pNvsStorageDriver->readWifiConfig(&wifiSettings);

    DeviceInfoEvent deviceSettings;
    s_pNvsStorageDriver->readDeviceConfig(&deviceSettings);

    if (wifiSettings.getMode() == WifiMode::AP) {
        m_coreSm.onServiceMode();
    }

    CloudLink.send(EventIdentifiers::DEVICE_INFO_EVENT, &deviceSettings);
    CloudLink.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &wifiSettings);

    // device started with GPIO, ISR already handle so we must send event manualy
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_GPIO) {
        ButtonEvent event(ButtonId::USER, ButtonStatus::SHORT_PRESS);
        Gui.send(EventIdentifiers::DEVICE_BTN_EVENT, &event);
    }
}

void ControlTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::WIFI_STATUS_EVENT: 
            onHandleWifiStatus(WifiStatusEvent(*pEvent));
            break;
        
        case EventIdentifiers::BATTERY_LEVEL_EVENT:
            onHandleBatteryStatus(BatteryStatusEvent(*pEvent));
            break;

        case EventIdentifiers::DEVICE_BTN_EVENT: 
            onHandleButton(ButtonEvent(*pEvent));
            break;

        case EventIdentifiers::DEVICE_CONFIG_EVENT:
            onHandleDeviceConfig(DeviceConfigEvent(*pEvent));
            break;

        case EventIdentifiers::DISPLAY_INFO_EVENT:
            onHandleDisplayInfo(DisplayInfoEvent(*pEvent));
            break;

    default:
        break;
    }
}

void ControlTask::onHandleTimer(const TimerId timerId) {
    // nothing to do
}

void ControlTask::onHandleWifiStatus(const WifiStatusEvent& status) {
    m_lastWifiStatus = status;
    m_wifiReceived = true;
    sendDeviceStatus();

    if (m_coreSm.isCurrentState(CoreSM::State::Service)) {
        if (status.getStatus() == WifiStatus::TIMEOUT
         || status.getStatus() == WifiStatus::CLIENT_DISCONNECTED) {
            shutdown();
            return;
        }
    }
    else if (m_coreSm.isCurrentState(CoreSM::State::BootUp)) {
        if (status.getStatus() == WifiStatus::TIMEOUT
         || status.getStatus() == WifiStatus::DISCONNECTED) {
            m_coreSm.onServiceMode();

            WifiSettingsEvent event;
            event.setMode(WifiMode::AP);
            CloudLink.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &event);
        }
        else if (status.getStatus() == WifiStatus::MQTT_CONNECTED) {
            m_coreSm.onRunning();

            MeasSensor.send(EventIdentifiers::SENSOR_SNAPSHOT);
        }
    }
    else if (m_coreSm.isCurrentState(CoreSM::State::Running)) {
        if (status.getStatus() == WifiStatus::TIMEOUT
         || status.getStatus() == WifiStatus::DISCONNECTED) {
            m_coreSm.onServiceMode();

            WifiSettingsEvent event;
            event.setMode(WifiMode::AP);
            CloudLink.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &event);
        }

        m_deviceStatusSent |= status.getLastSentMsg() == EventIdentifiers::DEVICE_STATUS_EVENT;
        m_sensorStatusSent |= status.getLastSentMsg() == EventIdentifiers::SENSOR_DATA_EVENT;
        sleepConditional();
    }
}

void ControlTask::onHandleBatteryStatus(const BatteryStatusEvent& status) {
    m_lastBatteryStatus = status;
    m_batteryReceived = true;
    sendDeviceStatus();
}

void ControlTask::onHandleButton(const ButtonEvent& button) {
    if (button.getButtonId() == ButtonId::SW_RESET) {
        if (button.getStatus() == ButtonStatus::LONG_PRESS) {
            s_pNvsStorageDriver->reset();
        }
        
        restart();
    }
}

void ControlTask::onHandleDisplayInfo(const DisplayInfoEvent& info) {
    m_displayOn = info.getStatus();

    sleepConditional();
}

void ControlTask::onHandleDeviceConfig(const DeviceConfigEvent& config) {
    char8_t channel[DeviceConfigEvent::MAX_CHANNEL_LENGTH];
    config.getChannel(channel);
    uint32_t interval = config.getInterval();

    if (strlen(channel) > 0) {
        s_pNvsStorageDriver->put(ESP_CTRL_PROP_DEVICE_CHANNEL, channel, sizeof(channel));
    }
    if (interval > 0) {
        s_pNvsStorageDriver->put(ESP_CTRL_PROP_DEVICE_INTERVAL, &interval, sizeof(interval));
    }
    s_pNvsStorageDriver->commit();

    // NOTE: configuration is only applied to application on bootUp process, so restart device manualy
}

void ControlTask::sendDeviceStatus() {
    // pre condition for sending device status
    if (!m_wifiReceived || !m_batteryReceived || !m_coreSm.isCurrentState(CoreSM::State::Running)) return;

    m_wifiReceived = false;
    m_batteryReceived = false;

    uint32_t uptime = pdTICKS_TO_MS(xTaskGetTickCount());
    DeviceStatusEvent deviceInfo(m_lastBatteryStatus, m_lastWifiStatus, uptime);

    CloudLink.send(EventIdentifiers::DEVICE_STATUS_EVENT, &deviceInfo);
}

void ControlTask::shutdown() {
    // TODO perform shutdown, seems IDF does not support shutdown, 
    // so we must connect GPIO 1 (PCU_STATE2) as output and connect to Latch Power or 
    // battery charger
    esp_restart();
}

void ControlTask::restart() {
    esp_restart();
}

void ControlTask::sleepConditional() {
    if (!m_deviceStatusSent || !m_sensorStatusSent || m_displayOn) return;
    
    m_deviceStatusSent = false;
    m_sensorStatusSent = false;
    m_displayOn = false;

    sleep();
}

void ControlTask::sleep() {
    // According to IDF Doc all communication peripherals
    // must be stopped before entering sleep mode 
    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/system/sleep_modes.html#wi-fi-bluetooth-and-sleep-modes
    WifiSettingsEvent settings;
    settings.setMode(WifiMode::NONE);
    CloudLink.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &settings);

    // wait at least 1 second to ensure phy are stopped
    vTaskDelay(pdMS_TO_TICKS(1000));

    DeviceInfoEvent deviceSettings;
    s_pNvsStorageDriver->readDeviceConfig(&deviceSettings);

    esp_sleep_enable_timer_wakeup(deviceSettings.getInterval() * 1000000);
    esp_deep_sleep_enable_gpio_wakeup(BIT(USR_BTN), ESP_GPIO_WAKEUP_GPIO_LOW);

    ESP_LOGI("Control", "sleep starting");
    esp_deep_sleep_start();
}

