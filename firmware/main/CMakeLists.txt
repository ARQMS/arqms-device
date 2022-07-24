idf_component_register(
     SRCS 
          "Events/AirQualityEvent.cpp"
          "Events/WifiStatusEvent.cpp"
          "Events/WifiSettingsEvent.cpp"

          "HumiDevice.cpp"
          "Drivers/StartupController.cpp"

          "Control/ControlTask.cpp"
          
          "GuiUpdater/GuiUpdaterTask.cpp"
          "GuiUpdater/AirIndicatorDriver.cpp"

          "CloudLink/CloudLinkTask.cpp"
     INCLUDE_DIRS "."
     REQUIRES HumiDevice.Rtos HumiDevice.Serialization HumiDevice.Platform
)
