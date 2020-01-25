    .include "p33CH128MP508S1.inc"

; This file is generated at build time and contains the _sine_table declaration.
    .include "sine.inc"

; ******************************************************************************
; Variables in X Memory
; ******************************************************************************
    .section xvars, xmemory


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

; Pitch of the oscillators
_osc_freqs:
    .long 0x00146ECA
    .long 0x0028DD94
    .long 0x003D4C5E

; Oscillator accumulators
_osc_accs:
    .long 0, 0, 0

; Oscillator outputs
_osc_outputs:
    .word 0x0000, 0x0000, 0x0000

; ******************************************************************************
; Variables in Y Memory
; ******************************************************************************

    .section yvars, ymemory

; Modulation matrix
_mod_matrix:
    .word 0x0000, 0x00FF, 0x0000
    .word 0x0000, 0x0000, 0x00FF
    .word 0x0000, 0x0000, 0x0000
    .word 0x7FFF, 0x0000, 0x0000 ; Outputs

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

    ; WAIT FOR NEXT SAMPLE INTERRUPT
    ; Turn LED off while waiting
    bclr PORTE, #1
    ; Wait for DAC to set the ready flag.
    cp0.b _synth_dac_ready
    bra z, _synth_run
    ; Clear the flag
    clr _synth_dac_ready
    ; Turn LED on while running calculations
    bset PORTE, #1

    ; Set accumulator, frequency and output pointers
    mov #_osc_freqs, w2
    mov #_osc_accs, w3
    mov #_osc_outputs, w4
    mov #_mod_matrix, w10

    ; START LOOP
    do #2, 1f

    ; CALCULATE MODULATION PHASE OFFSET
    ; Load pointer to first oscillator output
    mov #_osc_outputs, w8
    ; Clear accumulator B and prefetch first mod and sample
    clr A, [w8]+=2, w5, [w10]+=2, w6
    ; Multiply each output sample by the corresponding modulation amount
    mac w5*w6, A, [w8]+=2, w5, [w10]+=2, w6
    mac w5*w6, A, [w8]+=2, w5, [w10]+=2, w6
    mac w5*w6, A
    ; Store vector multiplication result
    sac A, w5

    ; CALCULATE RAW SAMPLE INDEX
    ; Load accumulator upper word
    mov [w3 + 2], w0
    ; Add modulation phase offset
    add w0, w5, w0
    ; Mask out unused upper bits to get a valid index
    mov #0x07FF, w1
    and w1, w0, w0
    ; Shift left 1 bit since each entry is two bytes
    sl w0, #1, w0

    ; GET RAW SAMPLE VALUE
    ; Calculate address of sample in table
    mov #edspage(_sine_table), w1
    mov w1, _DSRPAG
    mov #edsoffset(_sine_table), w1
    add w0, w1, w0
    ; Read value from table
    mov [w0], [w4++]

    ; TODO: Get envelope value and multiply before storing

    ; UPDATE OSCILLATOR PHASE ACCUMULATOR
    ; Load oscillator frequency lower word
    mov [w2++], w1
    ; Load accumulator lower word
    mov [w3], w0
    ; Add lower word of frequency to accumulator
    add w1, w0, w0
    ; Store accumulator lower word back
    mov w0, [w3++]
    ; Load oscillator frequency upper word
    mov [w2++], w1
    ; Load accumulator upper word
    mov [w3], w0
    ; Add upper word of frequency to accumulator with carry from lower word
    addc w1, w0, w0
    ; Store accumulator upper word
1:  mov w0, [w3++]

    ; SUM OUTPUTS
    ; Setup output and gain pointers for use with DSP prefetch
    ; Gains from the mod matrix are already loaded in w10
    ; Outputs are in X memory, must be in w8 or w9
    mov #_osc_outputs, w8
    ; Clear accumulator and prefetch first output and gain
    clr A, [w8]+=2, w5, [w10]+=2, w6
    ; Multiply output and gain and accumulate for each oscillator
    mac w5*w6, A, [w8]+=2, w5, [w10]+=2, w6
    mac w5*w6, A, [w8]+=2, w5, [w10]+=2, w6
    mac w5*w6, A
    ; Store accumulator result back with no shift
    sac A, #0, w4

    ; STORE VALUE
    ; Convert from signed to unsigned
    mov #0x8000, w1
    add w4, w1, w0
    ; Shift right to convert from 16 bit to 12 bit
    lsr w0, #4, w0
    ; Copy to buffer
    mov w0, _synth_sample

    ; LOOP FOREVER
    bra _synth_run
