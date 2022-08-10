#ifndef MATH_H_
#define MATH_H_

/**
 * A helper class for math operation
 */
class Math {
public:
    /**
     * Compares two floating point numbers.
     * @param left First number.
     * @param right Second number.
     * @return True if numbers are equal.
     */
    template<typename Type>
    static inline bool areEqualFloats(const Type left, const Type right) {
        return memcmp(&left, &right, sizeof(Type)) == 0;
    }

    /**
     * Force a value to be in the range given by min and max.
     * 
     * @param value The value to be forced.
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * @return Value forced to be in range of min and max.
     */
    template<typename Type>
    static inline Type truncateToRange(const Type value, const Type min, const Type max) {
        if (value < min) {
            return min;
        }
        else if (value > max) {
            return max;
        }

        return value;
    }

    
private:
    /**
     * Provide the private default constructor so the compiler does not generate the default one.
     */
    explicit Math(void);

    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    Math(const Math& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    Math& operator=(const Math& other);
};

#endif