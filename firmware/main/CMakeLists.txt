idf_component_register(
     SRCS 
          "Events/AirQualityEvent.cpp"
          "Events/WifiStatusEvent.cpp"
          "Events/WifiSettingsEvent.cpp"

          "HumiDevice.cpp"
          "Drivers/StartupController.cpp"
          "Drivers/SK6805Driver.cpp"

          "Control/ControlTask.cpp"
          "Control/Persistency/NvsStorageDriver.cpp"
          
          "GuiUpdater/GuiUpdaterTask.cpp"
          "GuiUpdater/AirIndicatorDriver.cpp"

          "CloudLink/CloudLinkTask.cpp"
          "CloudLink/WifiStateMachine.cpp"
          "CloudLink/LocalCtrlHandler.cpp"
     INCLUDE_DIRS "."
     REQUIRES HumiDevice.Rtos HumiDevice.Serialization HumiDevice.Platform esp_local_ctrl mdns
     EMBED_TXTFILES "../certs/cacert.pem" "../certs/prvtkey.pem"
)
