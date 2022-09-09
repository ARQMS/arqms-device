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

    /**
     * Re-maps a number from on range to another (y = m*value + b). That is, a value of xStart would get mapped to yStart,
     * a value of xEnd to yEnd, values in-between to values in-between, etc.
     * 
     * @note Does not constrain values to within the range, because out-of-range values are sometimes intended and useful.
     * 
     * @tparam Type expected input and output type
     * @param value the number to map
     * @param xStart the lower bound of the value’s current range.
     * @param xEnd the upper bound of the value’s current range.
     * @param yStart the lower bound of the value’s target range.
     * @param yEnd the upper bound of the value’s target range.
     * @return the mapped vlaue
     */
    template<typename Type>
    static inline Type linearMap(const Type value, const Type xStart, const Type xEnd, const Type yStart, const Type yEnd) {
        const float32_t m = (yEnd - static_cast<float32_t>(yStart)) / (xEnd - static_cast<float32_t>(xStart));
        const float32_t b = yStart - m * xStart;

        return static_cast<Type>(m * value + b);
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