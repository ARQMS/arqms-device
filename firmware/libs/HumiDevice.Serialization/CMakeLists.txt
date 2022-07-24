idf_component_register(
     SRCS 
          "Deserializer.cpp"
          "Serializer.cpp"
     INCLUDE_DIRS ".." "."
     REQUIRES HumiDevice.Serialization HumiDevice.Platform
     
)
