#include "unity.h"
#include "oscillator.h"

static void SkipSamples(uint16_t samplesToSkip);
static void StartDefaultWave(void);

// Oscillator instance used by most tests
struct Oscillator_s m_osc;

void setUp(void)
{
    Oscillator_Init(&m_osc);
}

void tearDown(void)
{
}

void test_GetNextSampleReturnsZeroWhenStopped(void)
{
    StartDefaultWave();
    Oscillator_Stop(&m_osc);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(0, sample);
}

void test_GetNextSampleReturnsHighOnFirstSample(void)
{
    StartDefaultWave();

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

void test_GetNextSampleReturnsLowAfterFallingEdge(void)
{
    StartDefaultWave();
    SkipSamples(500);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(-127, sample);
}

void test_GetNextSampleReturnsHighBeforeFallingEdge(void)
{
    StartDefaultWave();
    SkipSamples(499);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

void test_GetNextSampleReturnsHighAfterRisingEdge(void)
{
    StartDefaultWave();
    SkipSamples(1000);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

void test_GetNextSampleReturnsLowBeforeRisingEdge(void)
{
    StartDefaultWave();
    SkipSamples(999);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(-127, sample);
}

void test_SetPeriodResetsPhase(void)
{
    StartDefaultWave();
    SkipSamples(500);
    // Next sample would be low.

    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_INT(1000));

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

void test_SetPeriodChangesPeriod(void)
{
    StartDefaultWave();
    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_INT(500));

    SkipSamples(250);

    // Sample 251 should be low; would have been high for default wave
    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(-127, sample);
}

void test_SetAmplitudeToZero(void)
{
    StartDefaultWave();
    Oscillator_SetAmplitude(&m_osc, 0);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(0, sample);
}

void test_SetAmplitudeToMax(void)
{
    StartDefaultWave();
    Oscillator_SetAmplitude(&m_osc, 127);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

void test_SetAmplitudeAboveMaxSetsToMax(void)
{
    StartDefaultWave();
    Oscillator_SetAmplitude(&m_osc, 128);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

void test_SetPulseWidthOffSilencesOutput(void)
{
    StartDefaultWave();
    Oscillator_SetPulseWidth(&m_osc, PULSE_WIDTH_OFF);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(0, sample);
}

void test_SetPulseWidthMax(void)
{
    StartDefaultWave();
    Oscillator_SetPulseWidth(&m_osc, PULSE_WIDTH_12);
    SkipSamples(125);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(-127, sample);
}

void test_SetPulseWidthAboveMaxSilencesOutput(void)
{
    StartDefaultWave();
    Oscillator_SetPulseWidth(&m_osc, PULSE_WIDTH_NUM_SETTINGS);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(0, sample);
}

void test_SetPeriodOneOrLessSilencesOscillator(void)
{
    StartDefaultWave();
    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_INT(1));

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(0, sample);
}

void test_DefaultPeriodAfterInit(void)
{
    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_INT(1000));

    Oscillator_Init(&m_osc);

    Oscillator_SetPulseWidth(&m_osc, PULSE_WIDTH_50);
    Oscillator_SetAmplitude(&m_osc, 127);
    Oscillator_Start(&m_osc);

    // Sample silenced since period is 0
    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(0, sample);
}

void test_DefaultPulseWidthAfterInit(void)
{
    Oscillator_SetPulseWidth(&m_osc, PULSE_WIDTH_50);

    Oscillator_Init(&m_osc);

    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_INT(1000));
    Oscillator_SetAmplitude(&m_osc, 127);
    Oscillator_Start(&m_osc);

    // Sample silenced since pulse width is off
    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(0, sample);
}

void test_DefaultAmplitudeAfterInit(void)
{
    Oscillator_SetAmplitude(&m_osc, 127);

    Oscillator_Init(&m_osc);

    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_INT(1000));
    Oscillator_SetPulseWidth(&m_osc, PULSE_WIDTH_50);
    Oscillator_Start(&m_osc);

    // Sample silenced since amplitude is 0
    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(0, sample);
}

void test_StoppedAfterInit(void)
{
    Oscillator_Start(&m_osc);

    Oscillator_Init(&m_osc);

    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_INT(1000));
    Oscillator_SetPulseWidth(&m_osc, PULSE_WIDTH_50);
    Oscillator_SetAmplitude(&m_osc, 127);

    // Sample silenced since oscillator is stopped
    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(0, sample);
}

void test_FirstCycleIsShortForNonIntegerPeriod(void)
{
    /*
     * For non-integer periods, the first cycle of the waveform will be the same
     * as the integer portion.  The leftover fractional portion is accumulated.
     * When this accumulation is one or greater, an extra sample is added to the
     * current waveform, and one is subtracted from the accumulation.  e.g. for
     * .75, the pattern will be short, long, long, long (an extra sample is
     * added 75% of the time).
     */

    StartDefaultWave();
    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_FLOAT(12.75));

    SkipSamples(12);

    // The next period is starting, so the sample will be high
    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

void test_ExtraSampleIsAddedForNonIntegerPeriod(void)
{
    // See above test for description

    StartDefaultWave();
    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_FLOAT(12.75));

    // First period is 12 samples; output would be high after this.
    SkipSamples(12);

    // Next three periods are 13 samples, so the output will still be low after
    // this.
    SkipSamples(12);

    // End of first long period
    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(-127, sample);
}

void test_ExtraSampleIsNotAddedForSubsequentShortCycles(void)
{
    // See above test for description

    StartDefaultWave();
    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_FLOAT(12.75));

    // For 12.75, the cycle will be 12, 13, 13, 13, 12, 13, 13, 13...
    // So The pattern repeats every 51 samples.
    SkipSamples(51);

    // The next cycle will be short, so the following two samples will show a
    // rising edge.
    SkipSamples(11);

    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(-127, sample);

    sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

void test_PhaseIsResetAfterStart(void)
{
    StartDefaultWave();

    // Next sample would normally be low
    SkipSamples(500);

    Oscillator_Stop(&m_osc);
    Oscillator_Start(&m_osc);

    // Sample is reset to high because of start.
    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

void test_AccumulatorIsResetAfterStart(void)
{
    StartDefaultWave();
    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_FLOAT(25.5));

    // Short cycle.
    SkipSamples(25);

    Oscillator_Stop(&m_osc);
    Oscillator_Start(&m_osc);

    // Another short cycle, since accumulator was reset.
    SkipSamples(25);

    // First sample of next cycle.
    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

void test_AccumulatorIsResetAfterPeriodChange(void)
{
    StartDefaultWave();
    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_FLOAT(25.5));

    // Short cycle.
    SkipSamples(25);

    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_FLOAT(25.5));

    // Another short cycle, since accumulator was reset.
    SkipSamples(25);

    // First sample of next cycle.
    int8_t sample = Oscillator_GetNextSample(&m_osc);
    TEST_ASSERT_EQUAL_INT(127, sample);
}

/******************************************************************************
 * Helper functions
 ******************************************************************************/

/*
 * Skip over the specified number of samples.
 */
static void SkipSamples(uint16_t samplesToSkip)
{
    for (uint16_t i = 0; i < samplesToSkip; i ++)
    {
        Oscillator_GetNextSample(&m_osc);
    }
}

/*
 * Configure the oscillator with some default settings and start it.
 */
static void StartDefaultWave(void)
{
    Oscillator_SetPeriod(&m_osc, FIXED_POINT_FROM_FLOAT(1000.0));
    Oscillator_SetAmplitude(&m_osc, 127);
    Oscillator_SetPulseWidth(&m_osc, PULSE_WIDTH_50);
    Oscillator_Start(&m_osc);
}
