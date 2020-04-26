#include "unity.h"

#include "cobs.h"
#include "cobs_data.h"

static void test_encode(
        const uint8_t *input_data, uint32_t input_length,
        const uint8_t *expected_data, uint32_t expected_length);
static void test_decode(
        const uint8_t *expected_data, uint32_t expected_length,
        const uint8_t *input_data, uint32_t input_length);

/******************************************************************************
 * Setup / Teardown
 ******************************************************************************/

void setUp(void)
{
}

void tearDown(void)
{
}

/******************************************************************************
 * Size Macros
 ******************************************************************************/

void test_encode_length_macro(void)
{
    TEST_ASSERT_EQUAL_INT(2, COBS_MAX_DATA_LENGTH_AFTER_ENCODE(1));
    TEST_ASSERT_EQUAL_INT(255, COBS_MAX_DATA_LENGTH_AFTER_ENCODE(254));
    TEST_ASSERT_EQUAL_INT(257, COBS_MAX_DATA_LENGTH_AFTER_ENCODE(255));
    TEST_ASSERT_EQUAL_INT(510, COBS_MAX_DATA_LENGTH_AFTER_ENCODE(508));
    TEST_ASSERT_EQUAL_INT(512, COBS_MAX_DATA_LENGTH_AFTER_ENCODE(509));
}

void test_decode_length_macro(void)
{
    TEST_ASSERT_EQUAL_INT(1, COBS_MAX_DATA_LENGTH_AFTER_DECODE(2));
    TEST_ASSERT_EQUAL_INT(255, COBS_MAX_DATA_LENGTH_AFTER_DECODE(256));
}

/******************************************************************************
 * COBS Encode
 ******************************************************************************/

void test_encode_short_data_no_zeros(void)
{
    test_encode(
            SHORT_DATA_NO_ZEROS,
            sizeof(SHORT_DATA_NO_ZEROS),
            SHORT_DATA_NO_ZEROS_ENCODED,
            sizeof(SHORT_DATA_NO_ZEROS_ENCODED));
}

void test_encode_short_data_one_zero(void)
{
    test_encode(
            SHORT_DATA_ONE_ZERO,
            sizeof(SHORT_DATA_ONE_ZERO),
            SHORT_DATA_ONE_ZERO_ENCODED,
            sizeof(SHORT_DATA_ONE_ZERO_ENCODED));
}

void test_encode_medium_data_several_zeros(void)
{
    test_encode(
            MEDIUM_DATA_SEVERAL_ZEROS,
            sizeof(MEDIUM_DATA_SEVERAL_ZEROS),
            MEDIUM_DATA_SEVERAL_ZEROS_ENCODED,
            sizeof(MEDIUM_DATA_SEVERAL_ZEROS_ENCODED));
}

void test_encode_long_data_no_zeros(void)
{
    test_encode(
            LONG_DATA_NO_ZEROS,
            sizeof(LONG_DATA_NO_ZEROS),
            LONG_DATA_NO_ZEROS_ENCODED,
            sizeof(LONG_DATA_NO_ZEROS_ENCODED));
}

void test_encode_extra_long_data_no_zeros(void)
{
    test_encode(
            EXTRA_LONG_DATA_NO_ZEROS,
            sizeof(EXTRA_LONG_DATA_NO_ZEROS),
            EXTRA_LONG_DATA_NO_ZEROS_ENCODED,
            sizeof(EXTRA_LONG_DATA_NO_ZEROS_ENCODED));
}


/******************************************************************************
 * COBS Decode
 ******************************************************************************/

void test_decode_short_data_no_zeros(void)
{
    test_decode(
            SHORT_DATA_NO_ZEROS,
            sizeof(SHORT_DATA_NO_ZEROS),
            SHORT_DATA_NO_ZEROS_ENCODED,
            sizeof(SHORT_DATA_NO_ZEROS_ENCODED));
}

void test_decode_short_data_one_zero(void)
{
    test_decode(
            SHORT_DATA_ONE_ZERO,
            sizeof(SHORT_DATA_ONE_ZERO),
            SHORT_DATA_ONE_ZERO_ENCODED,
            sizeof(SHORT_DATA_ONE_ZERO_ENCODED));
}

void test_decode_medium_data_several_zeros(void)
{
    test_decode(
            MEDIUM_DATA_SEVERAL_ZEROS,
            sizeof(MEDIUM_DATA_SEVERAL_ZEROS),
            MEDIUM_DATA_SEVERAL_ZEROS_ENCODED,
            sizeof(MEDIUM_DATA_SEVERAL_ZEROS_ENCODED));
}

void test_decode_long_data_no_zeros(void)
{
    test_decode(
            LONG_DATA_NO_ZEROS,
            sizeof(LONG_DATA_NO_ZEROS),
            LONG_DATA_NO_ZEROS_ENCODED,
            sizeof(LONG_DATA_NO_ZEROS_ENCODED));
}

void test_decode_extra_long_data_no_zeros(void)
{
    test_decode(
            EXTRA_LONG_DATA_NO_ZEROS,
            sizeof(EXTRA_LONG_DATA_NO_ZEROS),
            EXTRA_LONG_DATA_NO_ZEROS_ENCODED,
            sizeof(EXTRA_LONG_DATA_NO_ZEROS_ENCODED));
}

void test_decode_invalid_encoding(void)
{
    uint8_t result[sizeof(INVALID_ENCODING)];

    // Encode data.
    uint32_t length = cobs_decode(
            INVALID_ENCODING, sizeof(INVALID_ENCODING), result);

    // Check failure status received.
    TEST_ASSERT_EQUAL_INT32(COBS_DECODE_FAIL, length);
}

/******************************************************************************
 * Static Helper Functions
 ******************************************************************************/

static void test_encode(
        const uint8_t *input_data, uint32_t input_length,
        const uint8_t *expected_data, uint32_t expected_length)
{
    uint8_t result[expected_length];

    // Encode data.
    uint32_t length = cobs_encode(input_data, input_length, result);

    // Check correct length received.
    TEST_ASSERT_EQUAL_INT32(expected_length, length);

    // Check correct data received.
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_data, result, expected_length);
}

static void test_decode(
        const uint8_t *expected_data, uint32_t expected_length,
        const uint8_t *input_data, uint32_t input_length)
{
    uint8_t result[expected_length];

    // Encode data.
    uint32_t length = cobs_decode(input_data, input_length, result);

    // Check correct length received.
    TEST_ASSERT_EQUAL_INT32(expected_length, length);

    // Check correct data received.
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_data, result, expected_length);
}
