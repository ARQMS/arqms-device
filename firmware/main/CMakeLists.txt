idf_component_register(
     SRCS 
          "Events/AirQualityEvent.cpp"
          "Events/WifiStatusEvent.cpp"
          "Events/WifiSettingsEvent.cpp"
          "Events/DeviceSettingsEvent.cpp"
          "Events/SensorDataEvent.cpp"

          "HumiDevice.cpp"
          "Drivers/StartupController.cpp"
          "Drivers/SK6805Driver.cpp"

          "Control/ControlTask.cpp"
          "Control/Persistency/NvsStorageDriver.cpp"
          "Control/Persistency/NvsLayout.cpp"
          "Control/Persistency/NvsUtil.cpp"
          "Control/CoreSM/CoreSM.cpp"
          
          "GuiUpdater/GuiUpdaterTask.cpp"
          "GuiUpdater/AirIndicatorDriver.cpp"

          "CloudLink/CloudLinkTask.cpp"
          "CloudLink/Wifi.cpp"
          "CloudLink/WifiStateMachine.cpp"
          "CloudLink/ConfigurationService/ConfigurationService.cpp"
          "CloudLink/MqttService/MqttService.cpp"
     INCLUDE_DIRS "."
     REQUIRES HumiDevice.Rtos HumiDevice.Serialization HumiDevice.Platform esp_local_ctrl mdns mqtt
     EMBED_TXTFILES "../certs/cacert.pem" "../certs/prvtkey.pem"
)
