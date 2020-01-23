#include "unity.h"

#include "system.h"
#include "uart.h"
#include "dac.h"
#include "audio.h"
#include "synth.h"

#include <xc.h>
#include <stdio.h>

void setUp(void)
{
    puts("setUp");
}

void tearDown(void)
{
    puts("tearDown");
}

void test_me(void)
{
    TEST_FAIL_MESSAGE("test_me");
}

int main(void)
{
    system_init();
    uart_init();
    dac_init();
    audio_init();
    synth_init();

    UNITY_BEGIN();

    RUN_TEST(test_me);

    UNITY_END();
    
    while(1);

    return 0;
}
