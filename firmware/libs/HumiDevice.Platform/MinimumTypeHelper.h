#ifndef UTIL_MINIMUMTYPEHELPER_H_
#define UTIL_MINIMUMTYPEHELPER_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

template<bool B, class T, class F>
struct conditional { typedef T type; };

template<class T, class F>
struct conditional<false, T, F> { typedef F type; };


/**
 * A helper struct to define expected value type based on bits at compile time.
 *
 * see als following link:
 * 	 https://peter.bloomfield.online/using-cpp-templates-for-size-based-type-selection/
 */
template <size_t LastBit>
struct MinimumTypeHelper {
    typedef
        typename conditional<LastBit == 0 , void,
        typename conditional<LastBit <= 8 , uint8_t,
        typename conditional<LastBit <= 16, uint16_t,
        typename conditional<LastBit <= 32, uint32_t,
        typename conditional<LastBit <= 64, uint64_t,
        void>::type>::type>::type>::type>::type type;
};



#endif /* UTIL_MINIMUMTYPEHELPER_H_ */
