idf_component_register(SRCS "HumiDevice.cpp" 
                            "DeviceState/DeviceStateMachine.cpp"
                            "DeviceState/DeviceSettings.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES log nvs_flash)
