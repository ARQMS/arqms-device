idf_component_register(
     SRCS 
          "Events/AirQualityEvent.cpp"
          "Events/WifiStatusEvent.cpp"
          "Events/WifiSettingsEvent.cpp"
          "Events/DeviceSettingsEvent.cpp"
          "Events/SensorDataEvent.cpp"
          "Events/SensorStatusEvent.cpp"
          "Events/ButtonEvent.cpp"

          "HumiDevice.cpp"
          "Drivers/ApplicationHardwareConfig.cpp"
          "Drivers/StartupController.cpp"
          "Drivers/BME680Driver.cpp"
          "Drivers/SK6805Driver.cpp"

          "Control/ControlTask.cpp"
          "Control/Persistency/NvsStorageDriver.cpp"
          "Control/Persistency/NvsLayout.cpp"
          "Control/Persistency/NvsUtil.cpp"
          "Control/CoreSM/CoreSM.cpp"
          "Control/PushBtn/PushBtnCtrlTask.cpp"
          "Control/PushBtn/GpioIrqHandler.cpp"
          "Control/PushBtn/ButtonSM.cpp"
          
          "GuiUpdater/GuiUpdaterTask.cpp"
          "GuiUpdater/AirIndicatorDriver.cpp"

          "CloudLink/CloudLinkTask.cpp"
          "CloudLink/Wifi.cpp"
          "CloudLink/WifiStateMachine.cpp"
          "CloudLink/ConfigurationService/ConfigurationService.cpp"
          "CloudLink/MqttService/MqttService.cpp"

          "MeasSensor/MeasSensorTask.cpp"

          "MeasFilter/MeasFilterTask.cpp"
          "MeasFilter/MovingAvgFilter.cpp"
     INCLUDE_DIRS "."
     REQUIRES HumiDevice.Rtos HumiDevice.Serialization HumiDevice.Platform Bosch68x.Api esp_local_ctrl mdns mqtt json
     EMBED_TXTFILES "../certs/cacert.pem" "../certs/prvtkey.pem"
)
