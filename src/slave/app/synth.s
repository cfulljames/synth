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

    ; Wait for next sample interrupt
    bclr PORTE, #1              ; Turn LED off while waiting
    cp0.b _synth_dac_ready      ; Wait for DAC to set the ready flag
    bra z, _synth_run
    clr _synth_dac_ready        ; Clear the flag
    bset PORTE, #1              ; Turn LED on while running calculations

    ; Set accumulator, frequency and output pointers
    mov #_osc_freqs, w2
    mov #_osc_accs, w3
    mov #_osc_outputs, w4
    mov #_mod_matrix, w10

    ; Run the following calculations for each operator in the voice
    do #2, 1f

    ; Calculate modulation phase offset for this operator
    mov #_osc_outputs, w8       ; Load pointer to first oscillator output
    ; Clear accumulator B and prefetch first mod and sample
    clr A, [w8]+=2, w5, [w10]+=2, w6
    ; Multiply each output sample by the corresponding modulation amount
    mac w5*w6, A, [w8]+=2, w5, [w10]+=2, w6
    mac w5*w6, A, [w8]+=2, w5, [w10]+=2, w6
    mac w5*w6, A
    sac A, w5                   ; Store vector multiplication result

    ; Calculate sine table index from phase accumulator and modulation
    mov [w3 + 2], w0            ; Load accumulator upper word
    add w0, w5, w0              ; Add modulation phase offset
    mov #0x07FF, w1             ; Mask out unused upper bits to get valid index
    and w1, w0, w0
    sl w0, #1, w0               ; Shift left 1 bit since each entry is two bytes

    ; Calculate address of sample in sine table
    mov #edspage(_sine_table), w1
    mov w1, _DSRPAG
    mov #edsoffset(_sine_table), w1
    add w0, w1, w0
    mov [w0], [w4++]            ; Read value from table

    ; TODO: Get envelope value and multiply before storing

    ; Update oscillator phase accumulator
    mov [w2++], w1              ; Load oscillator frequency lower word
    mov [w3], w0                ; Load accumulator lower word
    add w1, w0, w0              ; Add lower word of frequency to accumulator
    mov w0, [w3++]              ; Store accumulator lower word back
    mov [w2++], w1              ; Load oscillator frequency upper word
    mov [w3], w0                ; Load accumulator upper word
    addc w1, w0, w0             ; Add upper word of frequency with carry
1:  mov w0, [w3++]              ; Store accumulator upper word

    ; Apply gain and set oscillator outputs
    ; Gains from the mod matrix are already loaded in w10
    ; Outputs are in X memory, must be in w8 or w9
    mov #_osc_outputs, w8
    ; Clear accumulator and prefetch first output and gain
    clr A, [w8]+=2, w5, [w10]+=2, w6
    ; Multiply output and gain and accumulate for each oscillator
    mac w5*w6, A, [w8]+=2, w5, [w10]+=2, w6
    mac w5*w6, A, [w8]+=2, w5, [w10]+=2, w6
    mac w5*w6, A
    sac A, #0, w4 ; Store accumulator result back with no shift

    ; Store the new sample in the buffer
    mov #0x8000, w1             ; Convert from signed to unsigned
    add w4, w1, w0
    lsr w0, #4, w0              ; Shift right to convert from 16 bit to 12 bit
    mov w0, _synth_sample       ; Copy to buffer

    ; Return to the top of the loop
    bra _synth_run
