#ifndef SRC_UTIL_BITFIELD_H_
#define SRC_UTIL_BITFIELD_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>
#include "MinimumTypeHelper.h"

/**
 * A value based BitField defined by Index and Bits.
 */
template<size_t Index, size_t Bits = 1>
class BitField {
private:
    enum {
        Mask = (1u << Bits) - 1u
    };

    typedef typename MinimumTypeHelper<Index + Bits>::type T;

public:
	/**
	 * Equal operator to set BitField for all expected value types.
	 *
	 * Note: The value is set at given Index and only Masked bits are written to BitField!
	 */
    template <class T2>
    BitField &operator=(T2 value) {
    	m_value = (m_value & ~(static_cast<T>(Mask) << Index)) | ((static_cast<T>(value) & static_cast<T>(Mask)) << Index);
        return *this;
    }

    operator T() const {
        return (m_value >> Index) & static_cast<T>(Mask);
    }

    BitField& operator++() {
        return *this = *this + 1;
    }

    T operator++(int) {
        T r = *this;
        ++* this;
        return r;
    }

    BitField& operator--() {
        return *this = *this - 1;
    }

    T operator--(int) {
        T r = *this; --* this; return r;
    }

private:
    T m_value;
};

/**
 * A bool BitField which can be 0 or 1. In contrast to BitField<Index,Bits> this one
 * ahs only single bit and represents a True/False flag.
 */
template<size_t Index>
class BitField<Index, 1> {
private:
    enum {
        Bits = 1,
        Mask = 0x01
    };

    typedef typename MinimumTypeHelper<Index + Bits>::type T;

public:
	/**
	 * Equal operator to set BitField value.
	 */
    BitField &operator=(bool value) {
    	m_value = (m_value & ~(Mask << Index)) | (value << Index);
        return *this;
    }


    operator bool() const {
    	return m_value & (static_cast<T>(Mask) << Index);
    }

private:
    T m_value;
};

#endif /* SRC_UTIL_BITFIELD_H_ */
