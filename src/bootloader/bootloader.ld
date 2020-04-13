/*
 * Bootloader Linker Script
 */

/* Memory map is shared with the master core application. */
INCLUDE "memory_map.ld"

SECTIONS
{
    /* Reset Instruction */
    .reset :
    {
          SHORT(ABSOLUTE(__reset));
          SHORT(0x04);
          SHORT((ABSOLUTE(__reset) >> 16) & 0x7F);
          SHORT(0);
    } > reset

    /* Code */
    .text :
    {
        *(.init);
        *(.user_init);
        KEEP (*(.handle));
        KEEP (*(.isr*));
        *(.libc) *(.libm) *(.libdsp);  /* keep together in this order */
        *(.lib*);
        *(.text);
    } > bootloader

    /* Configuration Words */
    __FSEC :
    { KEEP (*(__FSEC.sec*))    } >FSEC
    __FBSLIM :
    { KEEP (*(__FBSLIM.sec*))    } >FBSLIM
    __FSIGN :
    { KEEP (*(__FSIGN.sec*))    } >FSIGN
    __FOSCSEL :
    { KEEP (*(__FOSCSEL.sec*))    } >FOSCSEL
    __FOSC :
    { KEEP (*(__FOSC.sec*))    } >FOSC
    __FWDT :
    { KEEP (*(__FWDT.sec*))    } >FWDT
    __FICD :
    { KEEP (*(__FICD.sec*))    } >FICD
    __FDMTIVTL :
    { KEEP (*(__FDMTIVTL.sec*))    } >FDMTIVTL
    __FDMTIVTH :
    { KEEP (*(__FDMTIVTH.sec*))    } >FDMTIVTH
    __FDMTCNTL :
    { KEEP (*(__FDMTCNTL.sec*))    } >FDMTCNTL
    __FDMTCNTH :
    { KEEP (*(__FDMTCNTH.sec*))    } >FDMTCNTH
    __FDMT :
    { KEEP (*(__FDMT.sec*))    } >FDMT
    __FDEVOPT :
    { KEEP (*(__FDEVOPT.sec*))    } >FDEVOPT
    __FALTREG :
    { KEEP (*(__FALTREG.sec*))    } >FALTREG
    __FMBXM :
    { KEEP (*(__FMBXM.sec*))    } >FMBXM
    __FMBXHS1 :
    { KEEP (*(__FMBXHS1.sec*))    } >FMBXHS1
    __FMBXHS2 :
    { KEEP (*(__FMBXHS2.sec*))    } >FMBXHS2
    __FMBXHSEN :
    { KEEP (*(__FMBXHSEN.sec*))    } >FMBXHSEN
    __FCFGPRA0 :
    { KEEP (*(__FCFGPRA0.sec*))    } >FCFGPRA0
    __FCFGPRB0 :
    { KEEP (*(__FCFGPRB0.sec*))    } >FCFGPRB0
    __FCFGPRC0 :
    { KEEP (*(__FCFGPRC0.sec*))    } >FCFGPRC0
    __FCFGPRD0 :
    { KEEP (*(__FCFGPRD0.sec*))    } >FCFGPRD0
    __FCFGPRE0 :
    { KEEP (*(__FCFGPRE0.sec*))    } >FCFGPRE0
    __FS1OSCSEL :
    { KEEP (*(__FS1OSCSEL.sec*))    } >FS1OSCSEL
    __FS1OSC :
    { KEEP (*(__FS1OSC.sec*))    } >FS1OSC
    __FS1WDT :
    { KEEP (*(__FS1WDT.sec*))    } >FS1WDT
    __FS1ICD :
    { KEEP (*(__FS1ICD.sec*))    } >FS1ICD
    __FS1DEVOPT :
    { KEEP (*(__FS1DEVOPT.sec*))    } >FS1DEVOPT
    __FS1ALTREG :
    { KEEP (*(__FS1ALTREG.sec*))    } >FS1ALTREG

    /*
     * ICD Debug Exec
     *
     * This section provides optional storage for the ICD2 debugger. Define a
     * global symbol named __ICD2RAM to enable ICD2. This section must be loaded
     * at data address 0x1000.
     */
    .icd __DATA_BASE (NOLOAD):
    {
      . += (DEFINED (__ICD2RAM) ? 0x50 : 0 );
    } > data

    /* Debug Info */
    .comment        0 : { *(.comment) }
    .debug_info     0 : { *(.debug_info) *(.gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    .debug_ranges   0 : { *(.debug_ranges) }
    .debug_aranges  0 : { *(.debug_aranges) }

    /* Interrupt Vector Table */
    .ivt __BOOTLOADER_IVT_BASE :
    {
        LONG( DEFINED(__OscillatorFail)    ? ABSOLUTE(__OscillatorFail)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__AddressError)    ? ABSOLUTE(__AddressError)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__HardTrapError)    ? ABSOLUTE(__HardTrapError)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__StackError)    ? ABSOLUTE(__StackError)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MathError)    ? ABSOLUTE(__MathError)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ReservedTrap5)    ? ABSOLUTE(__ReservedTrap5)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SoftTrapError)    ? ABSOLUTE(__SoftTrapError)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ReservedTrap7)    ? ABSOLUTE(__ReservedTrap7)    :
             ABSOLUTE(__DefaultInterrupt));

        LONG( DEFINED(__INT0Interrupt)    ? ABSOLUTE(__INT0Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__T1Interrupt)    ? ABSOLUTE(__T1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CNAInterrupt)    ? ABSOLUTE(__CNAInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CNBInterrupt)    ? ABSOLUTE(__CNBInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__DMA0Interrupt)    ? ABSOLUTE(__DMA0Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt5)    ? ABSOLUTE(__Interrupt5)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCP1Interrupt)    ? ABSOLUTE(__CCP1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCT1Interrupt)    ? ABSOLUTE(__CCT1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__DMA1Interrupt)    ? ABSOLUTE(__DMA1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SPI1RXInterrupt)    ? ABSOLUTE(__SPI1RXInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SPI1TXInterrupt)    ? ABSOLUTE(__SPI1TXInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__U1RXInterrupt)    ? ABSOLUTE(__U1RXInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__U1TXInterrupt)    ? ABSOLUTE(__U1TXInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ECCSBEInterrupt)    ? ABSOLUTE(__ECCSBEInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__NVMInterrupt)    ? ABSOLUTE(__NVMInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__INT1Interrupt)    ? ABSOLUTE(__INT1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SI2C1Interrupt)    ? ABSOLUTE(__SI2C1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MI2C1Interrupt)    ? ABSOLUTE(__MI2C1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__DMA2Interrupt)    ? ABSOLUTE(__DMA2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CNCInterrupt)    ? ABSOLUTE(__CNCInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__INT2Interrupt)    ? ABSOLUTE(__INT2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__DMA3Interrupt)    ? ABSOLUTE(__DMA3Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__DMA4Interrupt)    ? ABSOLUTE(__DMA4Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCP2Interrupt)    ? ABSOLUTE(__CCP2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCT2Interrupt)    ? ABSOLUTE(__CCT2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__C1Interrupt)    ? ABSOLUTE(__C1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__INT3Interrupt)    ? ABSOLUTE(__INT3Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__U2RXInterrupt)    ? ABSOLUTE(__U2RXInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__U2TXInterrupt)    ? ABSOLUTE(__U2TXInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SPI2RXInterrupt)    ? ABSOLUTE(__SPI2RXInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SPI2TXInterrupt)    ? ABSOLUTE(__SPI2TXInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__C1RXInterrupt)    ? ABSOLUTE(__C1RXInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt32)    ? ABSOLUTE(__Interrupt32)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt33)    ? ABSOLUTE(__Interrupt33)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__DMA5Interrupt)    ? ABSOLUTE(__DMA5Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCP3Interrupt)    ? ABSOLUTE(__CCP3Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCT3Interrupt)    ? ABSOLUTE(__CCT3Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SI2C2Interrupt)    ? ABSOLUTE(__SI2C2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MI2C2Interrupt)    ? ABSOLUTE(__MI2C2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt39)    ? ABSOLUTE(__Interrupt39)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCP4Interrupt)    ? ABSOLUTE(__CCP4Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCT4Interrupt)    ? ABSOLUTE(__CCT4Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt42)    ? ABSOLUTE(__Interrupt42)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCP5Interrupt)    ? ABSOLUTE(__CCP5Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCT5Interrupt)    ? ABSOLUTE(__CCT5Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__DMTInterrupt)    ? ABSOLUTE(__DMTInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCP6Interrupt)    ? ABSOLUTE(__CCP6Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCT6Interrupt)    ? ABSOLUTE(__CCT6Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__QEI1Interrupt)    ? ABSOLUTE(__QEI1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__U1EInterrupt)    ? ABSOLUTE(__U1EInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__U2EInterrupt)    ? ABSOLUTE(__U2EInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CRCInterrupt)    ? ABSOLUTE(__CRCInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__C1TXInterrupt)    ? ABSOLUTE(__C1TXInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt53)    ? ABSOLUTE(__Interrupt53)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt54)    ? ABSOLUTE(__Interrupt54)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt55)    ? ABSOLUTE(__Interrupt55)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt56)    ? ABSOLUTE(__Interrupt56)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt57)    ? ABSOLUTE(__Interrupt57)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt58)    ? ABSOLUTE(__Interrupt58)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt59)    ? ABSOLUTE(__Interrupt59)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt60)    ? ABSOLUTE(__Interrupt60)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ICDInterrupt)    ? ABSOLUTE(__ICDInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__JTAGInterrupt)    ? ABSOLUTE(__JTAGInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PTGSTEPInterrupt)    ? ABSOLUTE(__PTGSTEPInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__I2C1BCInterrupt)    ? ABSOLUTE(__I2C1BCInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__I2C2BCInterrupt)    ? ABSOLUTE(__I2C2BCInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt66)    ? ABSOLUTE(__Interrupt66)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PWM1Interrupt)    ? ABSOLUTE(__PWM1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PWM2Interrupt)    ? ABSOLUTE(__PWM2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PWM3Interrupt)    ? ABSOLUTE(__PWM3Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PWM4Interrupt)    ? ABSOLUTE(__PWM4Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt71)    ? ABSOLUTE(__Interrupt71)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt72)    ? ABSOLUTE(__Interrupt72)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt73)    ? ABSOLUTE(__Interrupt73)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt74)    ? ABSOLUTE(__Interrupt74)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CNDInterrupt)    ? ABSOLUTE(__CNDInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CNEInterrupt)    ? ABSOLUTE(__CNEInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CMP1Interrupt)    ? ABSOLUTE(__CMP1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt78)    ? ABSOLUTE(__Interrupt78)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt79)    ? ABSOLUTE(__Interrupt79)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt80)    ? ABSOLUTE(__Interrupt80)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PTGWDTInterrupt)    ? ABSOLUTE(__PTGWDTInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PTG0Interrupt)    ? ABSOLUTE(__PTG0Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PTG1Interrupt)    ? ABSOLUTE(__PTG1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PTG2Interrupt)    ? ABSOLUTE(__PTG2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PTG3Interrupt)    ? ABSOLUTE(__PTG3Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SENT1Interrupt)    ? ABSOLUTE(__SENT1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SENT1EInterrupt)    ? ABSOLUTE(__SENT1EInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SENT2Interrupt)    ? ABSOLUTE(__SENT2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SENT2EInterrupt)    ? ABSOLUTE(__SENT2EInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCInterrupt)    ? ABSOLUTE(__ADCInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN0Interrupt)    ? ABSOLUTE(__ADCAN0Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN1Interrupt)    ? ABSOLUTE(__ADCAN1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN2Interrupt)    ? ABSOLUTE(__ADCAN2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN3Interrupt)    ? ABSOLUTE(__ADCAN3Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN4Interrupt)    ? ABSOLUTE(__ADCAN4Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN5Interrupt)    ? ABSOLUTE(__ADCAN5Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN6Interrupt)    ? ABSOLUTE(__ADCAN6Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN7Interrupt)    ? ABSOLUTE(__ADCAN7Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN8Interrupt)    ? ABSOLUTE(__ADCAN8Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN9Interrupt)    ? ABSOLUTE(__ADCAN9Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN10Interrupt)    ? ABSOLUTE(__ADCAN10Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN11Interrupt)    ? ABSOLUTE(__ADCAN11Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN12Interrupt)    ? ABSOLUTE(__ADCAN12Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN13Interrupt)    ? ABSOLUTE(__ADCAN13Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN14Interrupt)    ? ABSOLUTE(__ADCAN14Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN15Interrupt)    ? ABSOLUTE(__ADCAN15Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN16Interrupt)    ? ABSOLUTE(__ADCAN16Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN17Interrupt)    ? ABSOLUTE(__ADCAN17Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN18Interrupt)    ? ABSOLUTE(__ADCAN18Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN19Interrupt)    ? ABSOLUTE(__ADCAN19Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCAN20Interrupt)    ? ABSOLUTE(__ADCAN20Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt112)    ? ABSOLUTE(__Interrupt112)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt113)    ? ABSOLUTE(__Interrupt113)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt114)    ? ABSOLUTE(__Interrupt114)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCFLTInterrupt)    ? ABSOLUTE(__ADCFLTInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCMP0Interrupt)    ? ABSOLUTE(__ADCMP0Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCMP1Interrupt)    ? ABSOLUTE(__ADCMP1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCMP2Interrupt)    ? ABSOLUTE(__ADCMP2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADCMP3Interrupt)    ? ABSOLUTE(__ADCMP3Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADFLTR0Interrupt)    ? ABSOLUTE(__ADFLTR0Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADFLTR1Interrupt)    ? ABSOLUTE(__ADFLTR1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADFLTR2Interrupt)    ? ABSOLUTE(__ADFLTR2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADFLTR3Interrupt)    ? ABSOLUTE(__ADFLTR3Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CLC1PInterrupt)    ? ABSOLUTE(__CLC1PInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CLC2PInterrupt)    ? ABSOLUTE(__CLC2PInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SPI1Interrupt)    ? ABSOLUTE(__SPI1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__SPI2Interrupt)    ? ABSOLUTE(__SPI2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt128)    ? ABSOLUTE(__Interrupt128)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIS1Interrupt)    ? ABSOLUTE(__MSIS1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIAInterrupt)    ? ABSOLUTE(__MSIAInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIBInterrupt)    ? ABSOLUTE(__MSIBInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSICInterrupt)    ? ABSOLUTE(__MSICInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIDInterrupt)    ? ABSOLUTE(__MSIDInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIEInterrupt)    ? ABSOLUTE(__MSIEInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIFInterrupt)    ? ABSOLUTE(__MSIFInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIGInterrupt)    ? ABSOLUTE(__MSIGInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIHInterrupt)    ? ABSOLUTE(__MSIHInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIDTInterrupt)    ? ABSOLUTE(__MSIDTInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIWFEInterrupt)    ? ABSOLUTE(__MSIWFEInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__MSIFLTInterrupt)    ? ABSOLUTE(__MSIFLTInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__S1RSTInterrupt)    ? ABSOLUTE(__S1RSTInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt142)    ? ABSOLUTE(__Interrupt142)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt143)    ? ABSOLUTE(__Interrupt143)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt144)    ? ABSOLUTE(__Interrupt144)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt145)    ? ABSOLUTE(__Interrupt145)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__S1BRKInterrupt)    ? ABSOLUTE(__S1BRKInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt147)    ? ABSOLUTE(__Interrupt147)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt148)    ? ABSOLUTE(__Interrupt148)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCP7Interrupt)    ? ABSOLUTE(__CCP7Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCT7Interrupt)    ? ABSOLUTE(__CCT7Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt151)    ? ABSOLUTE(__Interrupt151)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCP8Interrupt)    ? ABSOLUTE(__CCP8Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CCT8Interrupt)    ? ABSOLUTE(__CCT8Interrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt154)    ? ABSOLUTE(__Interrupt154)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt155)    ? ABSOLUTE(__Interrupt155)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt156)    ? ABSOLUTE(__Interrupt156)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__S1CLKFInterrupt)    ? ABSOLUTE(__S1CLKFInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt158)    ? ABSOLUTE(__Interrupt158)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt159)    ? ABSOLUTE(__Interrupt159)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt160)    ? ABSOLUTE(__Interrupt160)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt161)    ? ABSOLUTE(__Interrupt161)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt162)    ? ABSOLUTE(__Interrupt162)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt163)    ? ABSOLUTE(__Interrupt163)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt164)    ? ABSOLUTE(__Interrupt164)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt165)    ? ABSOLUTE(__Interrupt165)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt166)    ? ABSOLUTE(__Interrupt166)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt167)    ? ABSOLUTE(__Interrupt167)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__ADFIFOInterrupt)    ? ABSOLUTE(__ADFIFOInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PEVTAInterrupt)    ? ABSOLUTE(__PEVTAInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PEVTBInterrupt)    ? ABSOLUTE(__PEVTBInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PEVTCInterrupt)    ? ABSOLUTE(__PEVTCInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PEVTDInterrupt)    ? ABSOLUTE(__PEVTDInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PEVTEInterrupt)    ? ABSOLUTE(__PEVTEInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__PEVTFInterrupt)    ? ABSOLUTE(__PEVTFInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CLC3PInterrupt)    ? ABSOLUTE(__CLC3PInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CLC4PInterrupt)    ? ABSOLUTE(__CLC4PInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CLC1NInterrupt)    ? ABSOLUTE(__CLC1NInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CLC2NInterrupt)    ? ABSOLUTE(__CLC2NInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CLC3NInterrupt)    ? ABSOLUTE(__CLC3NInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__CLC4NInterrupt)    ? ABSOLUTE(__CLC4NInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt181)    ? ABSOLUTE(__Interrupt181)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt182)    ? ABSOLUTE(__Interrupt182)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt183)    ? ABSOLUTE(__Interrupt183)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt184)    ? ABSOLUTE(__Interrupt184)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt185)    ? ABSOLUTE(__Interrupt185)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt186)    ? ABSOLUTE(__Interrupt186)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt187)    ? ABSOLUTE(__Interrupt187)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__Interrupt188)    ? ABSOLUTE(__Interrupt188)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__U1EVTInterrupt)    ? ABSOLUTE(__U1EVTInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
        LONG( DEFINED(__U2EVTInterrupt)    ? ABSOLUTE(__U2EVTInterrupt)    :
             ABSOLUTE(__DefaultInterrupt));
    } > bootloader_ivt
} /* SECTIONS */

