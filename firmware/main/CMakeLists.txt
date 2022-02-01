idf_component_register(SRCS "HumiDevice.cpp" 
                            "WiFi/WiFiManager.cpp"
                            "WiFi/MqttHandler.cpp"
                            "WiFi/LocalCtrlHandler.cpp"
                            "DeviceState/DeviceStateMachine.cpp"
                            "DeviceState/DeviceHandler.cpp"
                            "DeviceStorage/DeviceStorage.cpp"
                            "Sensor/SensorManager.cpp"
                            "Hmi/HmiManager.cpp"
                            "Events/EventLoop.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES log nvs_flash esp_wifi esp_local_ctrl mdns mqtt
                       EMBED_TXTFILES "../certs/cacert.pem" "../certs/prvtkey.pem")
