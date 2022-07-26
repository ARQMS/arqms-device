idf_component_register(
     SRCS 
          "EventRuntime.cpp"
          "TimerEvent.cpp"
     INCLUDE_DIRS ".." "."
     REQUIRES HumiDevice.Serialization HumiDevice.Platform
)
