idf_component_register(
     SRCS 
          "Rtos/TaskBase.cpp"

          "HumiDevice.cpp"
          "Drivers/StartupController.cpp"
          "Drivers/AirIndicatorDriver.cpp"

          "Control/ControlTask.cpp"
     INCLUDE_DIRS "."
)
