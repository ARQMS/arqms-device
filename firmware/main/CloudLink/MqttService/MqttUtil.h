#ifndef MQTT_UTIL_H_
#define MQTT_UTIL_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// idf includes
#include "mqtt_client.h"
#include <string>

// The number of supports placeholders
#define NUM_PLACEHOLDER 2

/**
 * Qos state for events
 * @see https://www.hivemq.com/blog/mqtt-essentials-part-6-mqtt-quality-of-service-levels/
 */
enum MqttQoS {
    AT_MOST_ONCE = 0,
    AT_LEAST_ONCE = 1,
    EXACTLY_INCE = 2,
};

/**
 * MQTT Helper
 */
class MqttUtil {
public:
    /**
     * Registers a placeholder with value (max 32 character!)
     * @param name the placeholder name
     * @param value the placeholder value
     */
    static void registerPlaceholder(const char8_t name[32], const char8_t value[32]) {
        assert(s_placeholderIndex < NUM_PLACEHOLDER);

        strncpy(s_placeholderName[s_placeholderIndex], name, 32);
        strncpy(s_placeholderValue[s_placeholderIndex], value, 32);

        s_placeholderIndex++;
    }

    /**
     * Subscribes for given topic
     * 
     * @param client the mqtt client 
     * @param topic the destination topic. Suppots placeholders
     * @return message_id of the subscribe message on success
     *         -1 on failure
     */
    static int32_t subscribe(esp_mqtt_client_handle_t client, const char8_t* topic) {
        if (client == NULL) return -1;

        std::string topicStr(topic);
        replacePlaceholders(topicStr);

        ESP_LOGD("Humi publish", "Subscribed to %s", topicStr.c_str());

        return esp_mqtt_client_subscribe(client, topicStr.c_str(), AT_LEAST_ONCE);
    }

    /**
     * Sends the given data to topic
     * 
     * @tparam T type of data to convert to str
     * @param client the mqtt client 
     * @param topic the destination topic. Suppots placeholders
     * @param data the data to send (must be convertable to string)
     * @return message_id of the publish message (for QoS 0 message_id will always be zero) on success.
     *         -1 on failure.
      */
    template<typename T>
    static int32_t publish(esp_mqtt_client_handle_t client, const char8_t* topic, const T& data) {
        std::string valueStr(std::to_string(data));
        return publish(client, topic, valueStr.c_str());
    }
    static int32_t publish(esp_mqtt_client_handle_t client, const char8_t* topic, const char8_t* data) {
        if (client == NULL) return -1;

        std::string topicStr(topic);
        replacePlaceholders(topicStr);

        std::string valueStr(data);

        ESP_LOGD("Humi publish", "Publish to %s with %s", topicStr.c_str(), valueStr.c_str());

        return esp_mqtt_client_publish(client, topicStr.c_str(), valueStr.c_str(), 0, AT_MOST_ONCE, false);
    }

private:
    // helper methods
    static void replacePlaceholders(std::string& str) {
        for (uint32_t i = 0; i < NUM_PLACEHOLDER; ++i) {
            replacePlaceholder(str, s_placeholderValue[i], s_placeholderName[i], strlen(s_placeholderName[i]));
        }
    }

    static void replacePlaceholder(std::string& str, const char8_t* value, const char8_t* placeholder, const size_t placeholderLength) {
        const int32_t pos = str.find(placeholder, placeholderLength);
        if (pos < 0) return;
        str.replace(pos, placeholderLength, value);
    }

    /**
     * Private constructor
     */
    MqttUtil();

    /**
     * Private destructor
     */
    ~MqttUtil();

    static size_t s_placeholderIndex;
    static char8_t s_placeholderName[NUM_PLACEHOLDER][32];
    static char8_t s_placeholderValue[NUM_PLACEHOLDER][32];
};

size_t MqttUtil::s_placeholderIndex = 0;
char8_t MqttUtil::s_placeholderName[NUM_PLACEHOLDER][32] = {0};
char8_t MqttUtil::s_placeholderValue[NUM_PLACEHOLDER][32] = {0};

#endif // MQTT_UTIL_H_