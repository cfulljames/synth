#include "unity.h"
#include "fixed_point.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_ConvertIntZeroToFixed(void)
{
    uint16_t a = 0;
    FixedPoint_t x = FIXED_POINT_FROM_INT(a);
    TEST_ASSERT_EQUAL_HEX(0x00000000, x);
}

void test_ConvertIntOneToFixed(void)
{
    uint16_t a = 1;
    FixedPoint_t x = FIXED_POINT_FROM_INT(a);
    TEST_ASSERT_EQUAL_HEX(0x00010000, x);
}

void test_ConvertIntMaxToFixed(void)
{
    uint16_t a = 0xFFFF;
    FixedPoint_t x = FIXED_POINT_FROM_INT(a);
    TEST_ASSERT_EQUAL_HEX(0xFFFF0000, x);
}

void test_ConvertFixedZeroToInt(void)
{
    FixedPoint_t x = 0x00000000;
    uint16_t a = FIXED_POINT_TO_INT(x);
    TEST_ASSERT_EQUAL_UINT(0, a);
}

void test_ConvertFixedOneToInt(void)
{
    FixedPoint_t x = 0x00010000;
    uint16_t a = FIXED_POINT_TO_INT(x);
    TEST_ASSERT_EQUAL_UINT(1, a);
}

void test_ConvertFixedMaxToInt(void)
{
    FixedPoint_t x = 0xFFFF0000;
    uint16_t a = FIXED_POINT_TO_INT(x);
    TEST_ASSERT_EQUAL_UINT(0xFFFF, a);
}

void test_ConvertFloatZeroToFixed(void)
{
    float a = 0.0;
    FixedPoint_t x = FIXED_POINT_FROM_FLOAT(a);
    TEST_ASSERT_EQUAL_HEX(0x00000000, x);
}

void test_ConvertFloatSmallToFixed(void)
{
    float a = 1.25;
    FixedPoint_t x = FIXED_POINT_FROM_FLOAT(a);
    TEST_ASSERT_EQUAL_HEX(0x00014000, x);
}

void test_ConvertFloatLargeToFixed(void)
{
    float a = 65535.0625;
    FixedPoint_t x = FIXED_POINT_FROM_FLOAT(a);
    TEST_ASSERT_EQUAL_HEX(0xFFFF1000, x);
}

void test_ConvertFixedPointZeroToFloat(void)
{
    FixedPoint_t x = 0x00000000;
    float a = FIXED_POINT_TO_FLOAT(x);
    TEST_ASSERT_EQUAL_FLOAT(0.0, a);
}

void test_ConvertFixedPointSmallToFloat(void)
{
    FixedPoint_t x = 0x00014000;
    float a = FIXED_POINT_TO_FLOAT(x);
    TEST_ASSERT_EQUAL_FLOAT(1.25, a);
}

void test_ConvertFixedPointLargeToFloat(void)
{
    FixedPoint_t x = 0xFFFF1000;
    float a = FIXED_POINT_TO_FLOAT(x);
    TEST_ASSERT_EQUAL_FLOAT(65535.0625, a);
}

/*
 * The following tests are only to ensure that the fixed point implementation
 * works as expected for math operations.  They are not testing actual
 * implemented functionality; they're just showing how the values can be used.
 */

void test_FixedPointAddition(void)
{
    FixedPoint_t x = FIXED_POINT_FROM_INT(53);
    FixedPoint_t y = FIXED_POINT_FROM_INT(47);
    uint16_t result = FIXED_POINT_TO_INT(x + y);
    TEST_ASSERT_EQUAL_UINT(100, result);
}

void test_FixedPointSubtraction(void)
{
    FixedPoint_t x = FIXED_POINT_FROM_INT(53);
    FixedPoint_t y = FIXED_POINT_FROM_INT(47);
    uint16_t result = FIXED_POINT_TO_INT(x - y);
    TEST_ASSERT_EQUAL_UINT(6, result);
}

void test_FixedPointTimesInt(void)
{
    FixedPoint_t x = FIXED_POINT_FROM_INT(12);
    uint16_t a = 5;
    uint16_t result = FIXED_POINT_TO_INT(x * a);
    TEST_ASSERT_EQUAL_UINT(60, result);
}

void test_FixedPointDividedByInt(void)
{
    FixedPoint_t x = FIXED_POINT_FROM_INT(25);
    uint16_t a = 5;
    uint16_t result = FIXED_POINT_TO_INT(x / a);
    TEST_ASSERT_EQUAL_UINT(5, result);
}
