#ifndef HDP_H_
#define HDP_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

///
/// The following structs represents the HDP (HumiDevice Protocol) which is sent and 
/// received from MQTT packages. It is a JSON based representation. See wiki articel for 
/// more detailed information.
///
/// @see https://github.com/ARQMS/arqms-device/wiki/HumiDevice-Protocol
///
#include "HDPDeviceStatus.h"
#include "HDPRoomStatus.h"
#include "HDPDeviceConfig.h"
#include "HDPUpdateInfo.h"

#endif // HDP_H_