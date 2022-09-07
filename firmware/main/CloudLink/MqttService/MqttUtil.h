#ifndef MQTT_UTIL_H_
#define MQTT_UTIL_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// idf includes
#include "mqtt_client.h"
#include <string>

// The number of supports placeholders
#define NUM_PLACEHOLDER 3
#define MAX_PLACEHOLDER_LENGTH 32

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
    static void registerPlaceholder(const char8_t name[MAX_PLACEHOLDER_LENGTH], const char8_t value[MAX_PLACEHOLDER_LENGTH]) {
        assert(s_placeholderIndex < NUM_PLACEHOLDER);

        strncpy(s_placeholderName[s_placeholderIndex], name, MAX_PLACEHOLDER_LENGTH);
        strncpy(s_placeholderValue[s_placeholderIndex], value, MAX_PLACEHOLDER_LENGTH);

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
    static int32_t publish(esp_mqtt_client_handle_t client, const char8_t* topic, const char8_t* data) {
        if (client == NULL) return -1;

        std::string topicStr(topic);
        replacePlaceholders(topicStr);

        std::string valueStr(data);

        ESP_LOGD("Humi publish", "Publish to %s with %s", topicStr.c_str(), valueStr.c_str());

        return esp_mqtt_client_publish(client, topicStr.c_str(), valueStr.c_str(), 0, AT_MOST_ONCE, false);
    }

    /**
     * Compares expected topic with placeholders and actual topic
     * 
     * @param expected the expected topic (with placeholder)
     * @param topic the actual topic
     * @return true whenever expected topic matches with actual topic
     */
    static bool isTopic(const char8_t* expected, const char8_t* topic) {
        std::string expectedTopicStr(expected);
        replacePlaceholders(expectedTopicStr);

        return expectedTopicStr.compare(topic) == 0;
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
    static char8_t s_placeholderName[NUM_PLACEHOLDER][MAX_PLACEHOLDER_LENGTH];
    static char8_t s_placeholderValue[NUM_PLACEHOLDER][MAX_PLACEHOLDER_LENGTH];
};

size_t MqttUtil::s_placeholderIndex = 0;
char8_t MqttUtil::s_placeholderName[NUM_PLACEHOLDER][MAX_PLACEHOLDER_LENGTH] = {0};
char8_t MqttUtil::s_placeholderValue[NUM_PLACEHOLDER][MAX_PLACEHOLDER_LENGTH] = {0};

#endif // MQTT_UTIL_H_