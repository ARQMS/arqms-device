idf_component_register(SRCS "HumiDevice.cpp" 
                            "WiFi/WiFiManager.cpp"
                            "DeviceState/DeviceStateMachine.cpp"
                            "DeviceStorage/DeviceStorage.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES log nvs_flash esp_wifi)
