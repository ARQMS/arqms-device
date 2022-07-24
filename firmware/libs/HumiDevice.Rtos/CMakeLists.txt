idf_component_register(
     SRCS 
          "EventRuntime.cpp"
     INCLUDE_DIRS ".." "."
     REQUIRES HumiDevice.Serialization HumiDevice.Platform
)
