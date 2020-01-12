    .include "p33CH128MP508S1.inc"

; This file is generated at build time and contains the _sine_table declaration.
    .include "sine.inc"

; ******************************************************************************
; Variables
; ******************************************************************************
    .data


; DAC Ready flag (signals that the DAC has written the current sample)
    .global _synth_dac_ready
    .align 2
_synth_dac_ready:
    .byte 0x00

; Current sample to be written
    .global _synth_sample
    .align 2
_synth_sample:
    .word 0x00

; Pitch of the oscillator
_osc_freq_l:
    .word 0x6ECA

_osc_freq_h:
    .word 0x0014

; Oscillator accumulator
_osc_acc_l:
    .word 0x0000

_osc_acc_h:
    .word 0x0000

;*******************************************************************************
; Functions
;*******************************************************************************

    .text

    .global _synth_init
    .global _synth_run

_synth_init:

    ; Initialize E1 as an output
    bclr TRISE, #1

    return


_synth_run:

    ; Turn LED off while waiting
    bclr PORTE, #1

    ; Wait for DAC to set the ready flag.
    cp0.b _synth_dac_ready
    bra z, _synth_run

    ; Clear the flag
    clr _synth_dac_ready

    ; Turn LED on while running calculations
    bset PORTE, #1

    ; CALCULATE INDEX

    ; Get lowest 11 bits of upper word to use as index in table
    mov #0x07FF, w0
    and _osc_acc_h, wreg

    ; Shift left 1 bit since each entry is two bytes
    sl w0, #1, w0


    ; GET VALUE

    ; Calculate address of sample in table
    mov #edspage(_sine_table), w1
    mov w1, _DSRPAG

    mov #edsoffset(_sine_table), w1
    add w0, w1, w0

    ; Read value from table
    mov [w0], w0

    ; Convert from signed to unsigned
    mov #0x8000, w1
    add w0, w1, w0

    ; Shift right to convert from 16 bit to 12 bit
    lsr w0, #4, w0


    ; STORE VALUE
    mov w0, _synth_sample


    ; UPDATE ACCUMULATOR

    ; Load oscillator frequency lower word into w0
    mov _osc_freq_l, w0

    ; Add lower word of frequency to accumulator
    add _osc_acc_l

    ; Load oscillator frequency upper word into w0
    mov _osc_freq_h, w0

    ; Add upper word of frequency to accumulator with carry from lower word
    addc _osc_acc_h


    ; LOOP FOREVER
    bra _synth_run
