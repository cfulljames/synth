/*!
 * \file
 *
 * Fixed point 16.16 type and utility macros.
 *
 * The values are stored with 16 bits for the integer portion and 16 bits for
 * the fractional portion.
 *
 * int              fraction
 * 0000000000000000.0000000000000000
 *
 * Using fixed point values:
 *     Adding two fixed point values: a + b
 *     Subtracting two fixed point values: a - b
 *     Multiply fixed point (a) by unsigned int (b): a * b
 *     Divide fixed point (a) by unsigned int (b): a / b
 *
 * The following operations are not supported:
 *     Adding a fixed point value with an unsigned int
 *     Subtracting a fixed point value from an unsigned int (or vice-versa)
 *     Multiplying two fixed point values
 *     Dividing by a fixed point value
 */

#ifndef _FIXED_POINT_H
#define _FIXED_POINT_H

#include <stdint.h>

typedef uint32_t FixedPoint_t;

//! The number of bits used to represent the fractional portion.
#define FIXED_POINT_SHIFT (16U)

//! Value 1 in fixed point.
#define FIXED_POINT_FACTOR ((FixedPoint_t)1 << FIXED_POINT_SHIFT)

//! Mask for the fractional portion
#define FIXED_POINT_MASK (FIXED_POINT_FACTOR - 1)

//! Convert an integer value to a fixed point value.
#define FIXED_POINT_FROM_INT(a) (((FixedPoint_t)(a) << FIXED_POINT_SHIFT))

//! Convert a fixed point value to an integer via truncation.
#define FIXED_POINT_TO_INT(x) ((uint16_t)(x >> FIXED_POINT_SHIFT))

//! Convert a floating point value to a fixed point value.
#define FIXED_POINT_FROM_FLOAT(a) ((FixedPoint_t)(a * FIXED_POINT_FACTOR))

//! Convert a fixed point value to a floating point value.
#define FIXED_POINT_TO_FLOAT(a) ((float)a / FIXED_POINT_FACTOR)

#endif // _FIXED_POINT_H
