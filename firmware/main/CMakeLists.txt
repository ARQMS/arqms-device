idf_component_register(
     SRCS 
          "Rtos/Serialization/Deserializer.cpp"
          "Rtos/Serialization/Serializer.cpp"
          "Rtos/EventRuntime.cpp"

          "Events/AirQualityEvent.cpp"

          "HumiDevice.cpp"
          "Drivers/StartupController.cpp"

          "Control/ControlTask.cpp"
          
          "GuiUpdater/GuiUpdaterTask.cpp"
          "GuiUpdater/AirIndicatorDriver.cpp"
     INCLUDE_DIRS "."
)
