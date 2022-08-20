#ifndef TMC5160_REGISTER_H_
#define TMC5160_REGISTER_H_

// Platform include
#include "MinimumTypeHelper.h"

/**
 * Template to define register at compile-time
 *
 * address - the register address.
 * expectedSize - the expected size based on register width, in bits.
 * type - the internal data structure. see also BitField and union for or use integer values.
 */
template<size_t address, size_t expectedSize, typename type>
struct Register {
	static_assert(sizeof(type) <= expectedSize, "Expected size must be greater than type");
    static_assert(expectedSize > 0, "invalid field of zero width");
    static_assert(expectedSize <= 32, "register width overflow");

    // gets the minimum type based on expected size
	typedef typename MinimumTypeHelper<expectedSize>::type T;

	constexpr size_t getAddress() const {
		return address;
	}

    /**
     * Default Constructor.
     */
    Register() : m_meta() {}
    Register(const T value) : m_meta() {
        m_meta.value = value;
	}

	/**
	 * Extract register for easy access to internal data structure, very useful for BitField union structures.
	 */
    type& operator ()() {
        return m_meta.structure;
    }

    /**
     * Used for set register values directly. Useful if no internal data structure is attached.
     */
    type& operator =(const T value) {
    	m_meta.value = value;

        return m_meta.structure;
    }

    /**
     * Gets the internal data value.
     */
    template<typename TOut>
    explicit operator TOut() {
    	return m_meta.value;
    }

private:

	// A union for expected register values.
	//	T: ensure size is correct
	//	type: is used for read and write expected BitField with a named index
	union InternStruct {
		volatile T value;
	    type structure;
	};

	// An internal type which reserved expected register size in memory
	InternStruct m_meta;
};


#endif /* TMC5160_REGISTER_H_ */
