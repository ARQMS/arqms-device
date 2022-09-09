#ifndef HDP_UPDATE_INFO_H_
#define HDP_UPDATE_INFO_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "HDPMessage.h"

/**
 * Immutable update info object for HDP.
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/HumiDevice-Protocol
 */
class HDPUpdateInfo : public HDPMessage {
public:
    // constants
    const static uint32_t MAX_DOWNLOAD_URI = 255;

    /**
     * Constructor
     */
    HDPUpdateInfo() {
    };

    /**
     * Destructor
     */
    virtual ~HDPUpdateInfo() {
    };

    /**
     * Setter for download uri
     * 
     * @param downloadUri to set, NULL will clear downloadUri
     */
    void setDownloadUri(const char8_t downloadUri[MAX_DOWNLOAD_URI]) {
        if (downloadUri != NULL) {
            memcpy(m_downloadUri, downloadUri, MAX_DOWNLOAD_URI);
        }
        else {
            memset(m_downloadUri, 0, MAX_DOWNLOAD_URI);
        }
    }
    
    /**
     * Getter for download uri
     * 
     * @param uri to get
     */
    void getDownloadUri(char8_t uri[MAX_DOWNLOAD_URI]) const {
        if (uri != NULL) {
            memcpy(uri, m_downloadUri, MAX_DOWNLOAD_URI);
        }
    }

    /**
     * Setter for version 
     * 
     * @param version to set
     */
    void setVersion(const uint32_t version) {
        m_version = version;
    }

    /**
     * Getter for version
     * 
     * @return uint32_t the version
     */
    uint32_t getVersion() const {
        return m_version;
    }

    /**
     * @see HDPMessage::getJson
     */
    virtual void getJson(cJSON* obj) const override {
        cJSON_AddStringToObject(obj, "DownloadUri", m_downloadUri);
        cJSON_AddNumberToObject(obj, "Version", m_version);
    };

private:
    char8_t m_downloadUri[MAX_DOWNLOAD_URI];
    uint32_t m_version;
};

#endif // HDP_UPDATE_INFO_H_