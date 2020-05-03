/*
 * Master Core Linker Script
 */

/* Memory map is shared with the bootloader. */
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
    } > application_ivt

    /* Code */
    .text :
    {
        *(.init);
        *(.dinit);
        KEEP (*(.handle));
        KEEP (*(.isr*));
        *(.libc) *(.libm) *(.libdsp);  /* keep together in this order */
        *(.lib*);
        *(.text*);
    } > application

    /* Data Constants */
    .const :
    {
        *(.const);
    } > application

    /* Slave core application to be loaded at runtime. */
    .slave_image :
    {
        *(synth_slave_image);
    } > application

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
    .ivt :
    {
        LONG( DEFINED(__AltOscillatorFail)   ? ABSOLUTE(__AltOscillatorFail)   :
             (DEFINED(__OscillatorFail)      ? ABSOLUTE(__OscillatorFail)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltAddressError)     ? ABSOLUTE(__AltAddressError)     :
             (DEFINED(__AddressError)        ? ABSOLUTE(__AddressError)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltHardTrapError)    ? ABSOLUTE(__AltHardTrapError)    :
             (DEFINED(__HardTrapError)       ? ABSOLUTE(__HardTrapError)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltStackError)       ? ABSOLUTE(__AltStackError)       :
             (DEFINED(__StackError)          ? ABSOLUTE(__StackError)          :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMathError)        ? ABSOLUTE(__AltMathError)        :
             (DEFINED(__MathError)           ? ABSOLUTE(__MathError)           :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltReservedTrap5)    ? ABSOLUTE(__AltReservedTrap5)    :
             (DEFINED(__ReservedTrap5)       ? ABSOLUTE(__ReservedTrap5)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSoftTrapError)    ? ABSOLUTE(__AltSoftTrapError)    :
             (DEFINED(__SoftTrapError)       ? ABSOLUTE(__SoftTrapError)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltReservedTrap7)    ? ABSOLUTE(__AltReservedTrap7)    :
             (DEFINED(__ReservedTrap7)       ? ABSOLUTE(__ReservedTrap7)       :
             ABSOLUTE(__DefaultInterrupt)));

        LONG( DEFINED(__AltINT0Interrupt)    ? ABSOLUTE(__AltINT0Interrupt)    :
             (DEFINED(__INT0Interrupt)       ? ABSOLUTE(__INT0Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltT1Interrupt)      ? ABSOLUTE(__AltT1Interrupt)      :
             (DEFINED(__T1Interrupt)         ? ABSOLUTE(__T1Interrupt)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCNAInterrupt)     ? ABSOLUTE(__AltCNAInterrupt)     :
             (DEFINED(__CNAInterrupt)        ? ABSOLUTE(__CNAInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCNBInterrupt)     ? ABSOLUTE(__AltCNBInterrupt)     :
             (DEFINED(__CNBInterrupt)        ? ABSOLUTE(__CNBInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltDMA0Interrupt)    ? ABSOLUTE(__AltDMA0Interrupt)    :
             (DEFINED(__DMA0Interrupt)       ? ABSOLUTE(__DMA0Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt5)       ? ABSOLUTE(__AltInterrupt5)       :
             (DEFINED(__Interrupt5)          ? ABSOLUTE(__Interrupt5)          :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCP1Interrupt)    ? ABSOLUTE(__AltCCP1Interrupt)    :
             (DEFINED(__CCP1Interrupt)       ? ABSOLUTE(__CCP1Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCT1Interrupt)    ? ABSOLUTE(__AltCCT1Interrupt)    :
             (DEFINED(__CCT1Interrupt)       ? ABSOLUTE(__CCT1Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltDMA1Interrupt)    ? ABSOLUTE(__AltDMA1Interrupt)    :
             (DEFINED(__DMA1Interrupt)       ? ABSOLUTE(__DMA1Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSPI1RXInterrupt)  ? ABSOLUTE(__AltSPI1RXInterrupt)  :
             (DEFINED(__SPI1RXInterrupt)     ? ABSOLUTE(__SPI1RXInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSPI1TXInterrupt)  ? ABSOLUTE(__AltSPI1TXInterrupt)  :
             (DEFINED(__SPI1TXInterrupt)     ? ABSOLUTE(__SPI1TXInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltU1RXInterrupt)    ? ABSOLUTE(__AltU1RXInterrupt)    :
             (DEFINED(__U1RXInterrupt)       ? ABSOLUTE(__U1RXInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltU1TXInterrupt)    ? ABSOLUTE(__AltU1TXInterrupt)    :
             (DEFINED(__U1TXInterrupt)       ? ABSOLUTE(__U1TXInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltECCSBEInterrupt)  ? ABSOLUTE(__AltECCSBEInterrupt)  :
             (DEFINED(__ECCSBEInterrupt)     ? ABSOLUTE(__ECCSBEInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltNVMInterrupt)     ? ABSOLUTE(__AltNVMInterrupt)     :
             (DEFINED(__NVMInterrupt)        ? ABSOLUTE(__NVMInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltINT1Interrupt)    ? ABSOLUTE(__AltINT1Interrupt)    :
             (DEFINED(__INT1Interrupt)       ? ABSOLUTE(__INT1Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSI2C1Interrupt)   ? ABSOLUTE(__AltSI2C1Interrupt)   :
             (DEFINED(__SI2C1Interrupt)      ? ABSOLUTE(__SI2C1Interrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMI2C1Interrupt)   ? ABSOLUTE(__AltMI2C1Interrupt)   :
             (DEFINED(__MI2C1Interrupt)      ? ABSOLUTE(__MI2C1Interrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltDMA2Interrupt)    ? ABSOLUTE(__AltDMA2Interrupt)    :
             (DEFINED(__DMA2Interrupt)       ? ABSOLUTE(__DMA2Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCNCInterrupt)     ? ABSOLUTE(__AltCNCInterrupt)     :
             (DEFINED(__CNCInterrupt)        ? ABSOLUTE(__CNCInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltINT2Interrupt)    ? ABSOLUTE(__AltINT2Interrupt)    :
             (DEFINED(__INT2Interrupt)       ? ABSOLUTE(__INT2Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltDMA3Interrupt)    ? ABSOLUTE(__AltDMA3Interrupt)    :
             (DEFINED(__DMA3Interrupt)       ? ABSOLUTE(__DMA3Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltDMA4Interrupt)    ? ABSOLUTE(__AltDMA4Interrupt)    :
             (DEFINED(__DMA4Interrupt)       ? ABSOLUTE(__DMA4Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCP2Interrupt)    ? ABSOLUTE(__AltCCP2Interrupt)    :
             (DEFINED(__CCP2Interrupt)       ? ABSOLUTE(__CCP2Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCT2Interrupt)    ? ABSOLUTE(__AltCCT2Interrupt)    :
             (DEFINED(__CCT2Interrupt)       ? ABSOLUTE(__CCT2Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltC1Interrupt)      ? ABSOLUTE(__AltC1Interrupt)      :
             (DEFINED(__C1Interrupt)         ? ABSOLUTE(__C1Interrupt)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltINT3Interrupt)    ? ABSOLUTE(__AltINT3Interrupt)    :
             (DEFINED(__INT3Interrupt)       ? ABSOLUTE(__INT3Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltU2RXInterrupt)    ? ABSOLUTE(__AltU2RXInterrupt)    :
             (DEFINED(__U2RXInterrupt)       ? ABSOLUTE(__U2RXInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltU2TXInterrupt)    ? ABSOLUTE(__AltU2TXInterrupt)    :
             (DEFINED(__U2TXInterrupt)       ? ABSOLUTE(__U2TXInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSPI2RXInterrupt)  ? ABSOLUTE(__AltSPI2RXInterrupt)  :
             (DEFINED(__SPI2RXInterrupt)     ? ABSOLUTE(__SPI2RXInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSPI2TXInterrupt)  ? ABSOLUTE(__AltSPI2TXInterrupt)  :
             (DEFINED(__SPI2TXInterrupt)     ? ABSOLUTE(__SPI2TXInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltC1RXInterrupt)    ? ABSOLUTE(__AltC1RXInterrupt)    :
             (DEFINED(__C1RXInterrupt)       ? ABSOLUTE(__C1RXInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt32)      ? ABSOLUTE(__AltInterrupt32)      :
             (DEFINED(__Interrupt32)         ? ABSOLUTE(__Interrupt32)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt33)      ? ABSOLUTE(__AltInterrupt33)      :
             (DEFINED(__Interrupt33)         ? ABSOLUTE(__Interrupt33)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltDMA5Interrupt)    ? ABSOLUTE(__AltDMA5Interrupt)    :
             (DEFINED(__DMA5Interrupt)       ? ABSOLUTE(__DMA5Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCP3Interrupt)    ? ABSOLUTE(__AltCCP3Interrupt)    :
             (DEFINED(__CCP3Interrupt)       ? ABSOLUTE(__CCP3Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCT3Interrupt)    ? ABSOLUTE(__AltCCT3Interrupt)    :
             (DEFINED(__CCT3Interrupt)       ? ABSOLUTE(__CCT3Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSI2C2Interrupt)   ? ABSOLUTE(__AltSI2C2Interrupt)   :
             (DEFINED(__SI2C2Interrupt)      ? ABSOLUTE(__SI2C2Interrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMI2C2Interrupt)   ? ABSOLUTE(__AltMI2C2Interrupt)   :
             (DEFINED(__MI2C2Interrupt)      ? ABSOLUTE(__MI2C2Interrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt39)      ? ABSOLUTE(__AltInterrupt39)      :
             (DEFINED(__Interrupt39)         ? ABSOLUTE(__Interrupt39)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCP4Interrupt)    ? ABSOLUTE(__AltCCP4Interrupt)    :
             (DEFINED(__CCP4Interrupt)       ? ABSOLUTE(__CCP4Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCT4Interrupt)    ? ABSOLUTE(__AltCCT4Interrupt)    :
             (DEFINED(__CCT4Interrupt)       ? ABSOLUTE(__CCT4Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt42)      ? ABSOLUTE(__AltInterrupt42)      :
             (DEFINED(__Interrupt42)         ? ABSOLUTE(__Interrupt42)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCP5Interrupt)    ? ABSOLUTE(__AltCCP5Interrupt)    :
             (DEFINED(__CCP5Interrupt)       ? ABSOLUTE(__CCP5Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCT5Interrupt)    ? ABSOLUTE(__AltCCT5Interrupt)    :
             (DEFINED(__CCT5Interrupt)       ? ABSOLUTE(__CCT5Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltDMTInterrupt)     ? ABSOLUTE(__AltDMTInterrupt)     :
             (DEFINED(__DMTInterrupt)        ? ABSOLUTE(__DMTInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCP6Interrupt)    ? ABSOLUTE(__AltCCP6Interrupt)    :
             (DEFINED(__CCP6Interrupt)       ? ABSOLUTE(__CCP6Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCT6Interrupt)    ? ABSOLUTE(__AltCCT6Interrupt)    :
             (DEFINED(__CCT6Interrupt)       ? ABSOLUTE(__CCT6Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltQEI1Interrupt)    ? ABSOLUTE(__AltQEI1Interrupt)    :
             (DEFINED(__QEI1Interrupt)       ? ABSOLUTE(__QEI1Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltU1EInterrupt)     ? ABSOLUTE(__AltU1EInterrupt)     :
             (DEFINED(__U1EInterrupt)        ? ABSOLUTE(__U1EInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltU2EInterrupt)     ? ABSOLUTE(__AltU2EInterrupt)     :
             (DEFINED(__U2EInterrupt)        ? ABSOLUTE(__U2EInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCRCInterrupt)     ? ABSOLUTE(__AltCRCInterrupt)     :
             (DEFINED(__CRCInterrupt)        ? ABSOLUTE(__CRCInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltC1TXInterrupt)    ? ABSOLUTE(__AltC1TXInterrupt)    :
             (DEFINED(__C1TXInterrupt)       ? ABSOLUTE(__C1TXInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt53)      ? ABSOLUTE(__AltInterrupt53)      :
             (DEFINED(__Interrupt53)         ? ABSOLUTE(__Interrupt53)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt54)      ? ABSOLUTE(__AltInterrupt54)      :
             (DEFINED(__Interrupt54)         ? ABSOLUTE(__Interrupt54)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt55)      ? ABSOLUTE(__AltInterrupt55)      :
             (DEFINED(__Interrupt55)         ? ABSOLUTE(__Interrupt55)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt56)      ? ABSOLUTE(__AltInterrupt56)      :
             (DEFINED(__Interrupt56)         ? ABSOLUTE(__Interrupt56)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt57)      ? ABSOLUTE(__AltInterrupt57)      :
             (DEFINED(__Interrupt57)         ? ABSOLUTE(__Interrupt57)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt58)      ? ABSOLUTE(__AltInterrupt58)      :
             (DEFINED(__Interrupt58)         ? ABSOLUTE(__Interrupt58)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt59)      ? ABSOLUTE(__AltInterrupt59)      :
             (DEFINED(__Interrupt59)         ? ABSOLUTE(__Interrupt59)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt60)      ? ABSOLUTE(__AltInterrupt60)      :
             (DEFINED(__Interrupt60)         ? ABSOLUTE(__Interrupt60)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltICDInterrupt)     ? ABSOLUTE(__AltICDInterrupt)     :
             (DEFINED(__ICDInterrupt)        ? ABSOLUTE(__ICDInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltJTAGInterrupt)    ? ABSOLUTE(__AltJTAGInterrupt)    :
             (DEFINED(__JTAGInterrupt)       ? ABSOLUTE(__JTAGInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPTGSTEPInterrupt) ? ABSOLUTE(__AltPTGSTEPInterrupt) :
             (DEFINED(__PTGSTEPInterrupt)    ? ABSOLUTE(__PTGSTEPInterrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltI2C1BCInterrupt)  ? ABSOLUTE(__AltI2C1BCInterrupt)  :
             (DEFINED(__I2C1BCInterrupt)     ? ABSOLUTE(__I2C1BCInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltI2C2BCInterrupt)  ? ABSOLUTE(__AltI2C2BCInterrupt)  :
             (DEFINED(__I2C2BCInterrupt)     ? ABSOLUTE(__I2C2BCInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt66)      ? ABSOLUTE(__AltInterrupt66)      :
             (DEFINED(__Interrupt66)         ? ABSOLUTE(__Interrupt66)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPWM1Interrupt)    ? ABSOLUTE(__AltPWM1Interrupt)    :
             (DEFINED(__PWM1Interrupt)       ? ABSOLUTE(__PWM1Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPWM2Interrupt)    ? ABSOLUTE(__AltPWM2Interrupt)    :
             (DEFINED(__PWM2Interrupt)       ? ABSOLUTE(__PWM2Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPWM3Interrupt)    ? ABSOLUTE(__AltPWM3Interrupt)    :
             (DEFINED(__PWM3Interrupt)       ? ABSOLUTE(__PWM3Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPWM4Interrupt)    ? ABSOLUTE(__AltPWM4Interrupt)    :
             (DEFINED(__PWM4Interrupt)       ? ABSOLUTE(__PWM4Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt71)      ? ABSOLUTE(__AltInterrupt71)      :
             (DEFINED(__Interrupt71)         ? ABSOLUTE(__Interrupt71)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt72)      ? ABSOLUTE(__AltInterrupt72)      :
             (DEFINED(__Interrupt72)         ? ABSOLUTE(__Interrupt72)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt73)      ? ABSOLUTE(__AltInterrupt73)      :
             (DEFINED(__Interrupt73)         ? ABSOLUTE(__Interrupt73)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt74)      ? ABSOLUTE(__AltInterrupt74)      :
             (DEFINED(__Interrupt74)         ? ABSOLUTE(__Interrupt74)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCNDInterrupt)     ? ABSOLUTE(__AltCNDInterrupt)     :
             (DEFINED(__CNDInterrupt)        ? ABSOLUTE(__CNDInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCNEInterrupt)     ? ABSOLUTE(__AltCNEInterrupt)     :
             (DEFINED(__CNEInterrupt)        ? ABSOLUTE(__CNEInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCMP1Interrupt)    ? ABSOLUTE(__AltCMP1Interrupt)    :
             (DEFINED(__CMP1Interrupt)       ? ABSOLUTE(__CMP1Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt78)      ? ABSOLUTE(__AltInterrupt78)      :
             (DEFINED(__Interrupt78)         ? ABSOLUTE(__Interrupt78)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt79)      ? ABSOLUTE(__AltInterrupt79)      :
             (DEFINED(__Interrupt79)         ? ABSOLUTE(__Interrupt79)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt80)      ? ABSOLUTE(__AltInterrupt80)      :
             (DEFINED(__Interrupt80)         ? ABSOLUTE(__Interrupt80)         :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPTGWDTInterrupt)  ? ABSOLUTE(__AltPTGWDTInterrupt)  :
             (DEFINED(__PTGWDTInterrupt)     ? ABSOLUTE(__PTGWDTInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPTG0Interrupt)    ? ABSOLUTE(__AltPTG0Interrupt)    :
             (DEFINED(__PTG0Interrupt)       ? ABSOLUTE(__PTG0Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPTG1Interrupt)    ? ABSOLUTE(__AltPTG1Interrupt)    :
             (DEFINED(__PTG1Interrupt)       ? ABSOLUTE(__PTG1Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPTG2Interrupt)    ? ABSOLUTE(__AltPTG2Interrupt)    :
             (DEFINED(__PTG2Interrupt)       ? ABSOLUTE(__PTG2Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPTG3Interrupt)    ? ABSOLUTE(__AltPTG3Interrupt)    :
             (DEFINED(__PTG3Interrupt)       ? ABSOLUTE(__PTG3Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSENT1Interrupt)   ? ABSOLUTE(__AltSENT1Interrupt)   :
             (DEFINED(__SENT1Interrupt)      ? ABSOLUTE(__SENT1Interrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSENT1EInterrupt)  ? ABSOLUTE(__AltSENT1EInterrupt)  :
             (DEFINED(__SENT1EInterrupt)     ? ABSOLUTE(__SENT1EInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSENT2Interrupt)   ? ABSOLUTE(__AltSENT2Interrupt)   :
             (DEFINED(__SENT2Interrupt)      ? ABSOLUTE(__SENT2Interrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSENT2EInterrupt)  ? ABSOLUTE(__AltSENT2EInterrupt)  :
             (DEFINED(__SENT2EInterrupt)     ? ABSOLUTE(__SENT2EInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCInterrupt)     ? ABSOLUTE(__AltADCInterrupt)     :
             (DEFINED(__ADCInterrupt)        ? ABSOLUTE(__ADCInterrupt)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN0Interrupt)  ? ABSOLUTE(__AltADCAN0Interrupt)  :
             (DEFINED(__ADCAN0Interrupt)     ? ABSOLUTE(__ADCAN0Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN1Interrupt)  ? ABSOLUTE(__AltADCAN1Interrupt)  :
             (DEFINED(__ADCAN1Interrupt)     ? ABSOLUTE(__ADCAN1Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN2Interrupt)  ? ABSOLUTE(__AltADCAN2Interrupt)  :
             (DEFINED(__ADCAN2Interrupt)     ? ABSOLUTE(__ADCAN2Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN3Interrupt)  ? ABSOLUTE(__AltADCAN3Interrupt)  :
             (DEFINED(__ADCAN3Interrupt)     ? ABSOLUTE(__ADCAN3Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN4Interrupt)  ? ABSOLUTE(__AltADCAN4Interrupt)  :
             (DEFINED(__ADCAN4Interrupt)     ? ABSOLUTE(__ADCAN4Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN5Interrupt)  ? ABSOLUTE(__AltADCAN5Interrupt)  :
             (DEFINED(__ADCAN5Interrupt)     ? ABSOLUTE(__ADCAN5Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN6Interrupt)  ? ABSOLUTE(__AltADCAN6Interrupt)  :
             (DEFINED(__ADCAN6Interrupt)     ? ABSOLUTE(__ADCAN6Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN7Interrupt)  ? ABSOLUTE(__AltADCAN7Interrupt)  :
             (DEFINED(__ADCAN7Interrupt)     ? ABSOLUTE(__ADCAN7Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN8Interrupt)  ? ABSOLUTE(__AltADCAN8Interrupt)  :
             (DEFINED(__ADCAN8Interrupt)     ? ABSOLUTE(__ADCAN8Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN9Interrupt)  ? ABSOLUTE(__AltADCAN9Interrupt)  :
             (DEFINED(__ADCAN9Interrupt)     ? ABSOLUTE(__ADCAN9Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN10Interrupt) ? ABSOLUTE(__AltADCAN10Interrupt) :
             (DEFINED(__ADCAN10Interrupt)    ? ABSOLUTE(__ADCAN10Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN11Interrupt) ? ABSOLUTE(__AltADCAN11Interrupt) :
             (DEFINED(__ADCAN11Interrupt)    ? ABSOLUTE(__ADCAN11Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN12Interrupt) ? ABSOLUTE(__AltADCAN12Interrupt) :
             (DEFINED(__ADCAN12Interrupt)    ? ABSOLUTE(__ADCAN12Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN13Interrupt) ? ABSOLUTE(__AltADCAN13Interrupt) :
             (DEFINED(__ADCAN13Interrupt)    ? ABSOLUTE(__ADCAN13Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN14Interrupt) ? ABSOLUTE(__AltADCAN14Interrupt) :
             (DEFINED(__ADCAN14Interrupt)    ? ABSOLUTE(__ADCAN14Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN15Interrupt) ? ABSOLUTE(__AltADCAN15Interrupt) :
             (DEFINED(__ADCAN15Interrupt)    ? ABSOLUTE(__ADCAN15Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN16Interrupt) ? ABSOLUTE(__AltADCAN16Interrupt) :
             (DEFINED(__ADCAN16Interrupt)    ? ABSOLUTE(__ADCAN16Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN17Interrupt) ? ABSOLUTE(__AltADCAN17Interrupt) :
             (DEFINED(__ADCAN17Interrupt)    ? ABSOLUTE(__ADCAN17Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN18Interrupt) ? ABSOLUTE(__AltADCAN18Interrupt) :
             (DEFINED(__ADCAN18Interrupt)    ? ABSOLUTE(__ADCAN18Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN19Interrupt) ? ABSOLUTE(__AltADCAN19Interrupt) :
             (DEFINED(__ADCAN19Interrupt)    ? ABSOLUTE(__ADCAN19Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCAN20Interrupt) ? ABSOLUTE(__AltADCAN20Interrupt) :
             (DEFINED(__ADCAN20Interrupt)    ? ABSOLUTE(__ADCAN20Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt112)     ? ABSOLUTE(__AltInterrupt112)     :
             (DEFINED(__Interrupt112)        ? ABSOLUTE(__Interrupt112)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt113)     ? ABSOLUTE(__AltInterrupt113)     :
             (DEFINED(__Interrupt113)        ? ABSOLUTE(__Interrupt113)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt114)     ? ABSOLUTE(__AltInterrupt114)     :
             (DEFINED(__Interrupt114)        ? ABSOLUTE(__Interrupt114)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCFLTInterrupt)  ? ABSOLUTE(__AltADCFLTInterrupt)  :
             (DEFINED(__ADCFLTInterrupt)     ? ABSOLUTE(__ADCFLTInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCMP0Interrupt)  ? ABSOLUTE(__AltADCMP0Interrupt)  :
             (DEFINED(__ADCMP0Interrupt)     ? ABSOLUTE(__ADCMP0Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCMP1Interrupt)  ? ABSOLUTE(__AltADCMP1Interrupt)  :
             (DEFINED(__ADCMP1Interrupt)     ? ABSOLUTE(__ADCMP1Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCMP2Interrupt)  ? ABSOLUTE(__AltADCMP2Interrupt)  :
             (DEFINED(__ADCMP2Interrupt)     ? ABSOLUTE(__ADCMP2Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADCMP3Interrupt)  ? ABSOLUTE(__AltADCMP3Interrupt)  :
             (DEFINED(__ADCMP3Interrupt)     ? ABSOLUTE(__ADCMP3Interrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADFLTR0Interrupt) ? ABSOLUTE(__AltADFLTR0Interrupt) :
             (DEFINED(__ADFLTR0Interrupt)    ? ABSOLUTE(__ADFLTR0Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADFLTR1Interrupt) ? ABSOLUTE(__AltADFLTR1Interrupt) :
             (DEFINED(__ADFLTR1Interrupt)    ? ABSOLUTE(__ADFLTR1Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADFLTR2Interrupt) ? ABSOLUTE(__AltADFLTR2Interrupt) :
             (DEFINED(__ADFLTR2Interrupt)    ? ABSOLUTE(__ADFLTR2Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADFLTR3Interrupt) ? ABSOLUTE(__AltADFLTR3Interrupt) :
             (DEFINED(__ADFLTR3Interrupt)    ? ABSOLUTE(__ADFLTR3Interrupt)    :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCLC1PInterrupt)   ? ABSOLUTE(__AltCLC1PInterrupt)   :
             (DEFINED(__CLC1PInterrupt)      ? ABSOLUTE(__CLC1PInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCLC2PInterrupt)   ? ABSOLUTE(__AltCLC2PInterrupt)   :
             (DEFINED(__CLC2PInterrupt)      ? ABSOLUTE(__CLC2PInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSPI1Interrupt)    ? ABSOLUTE(__AltSPI1Interrupt)    :
             (DEFINED(__SPI1Interrupt)       ? ABSOLUTE(__SPI1Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltSPI2Interrupt)    ? ABSOLUTE(__AltSPI2Interrupt)    :
             (DEFINED(__SPI2Interrupt)       ? ABSOLUTE(__SPI2Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt128)     ? ABSOLUTE(__AltInterrupt128)     :
             (DEFINED(__Interrupt128)        ? ABSOLUTE(__Interrupt128)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIS1Interrupt)   ? ABSOLUTE(__AltMSIS1Interrupt)   :
             (DEFINED(__MSIS1Interrupt)      ? ABSOLUTE(__MSIS1Interrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIAInterrupt)    ? ABSOLUTE(__AltMSIAInterrupt)    :
             (DEFINED(__MSIAInterrupt)       ? ABSOLUTE(__MSIAInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIBInterrupt)    ? ABSOLUTE(__AltMSIBInterrupt)    :
             (DEFINED(__MSIBInterrupt)       ? ABSOLUTE(__MSIBInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSICInterrupt)    ? ABSOLUTE(__AltMSICInterrupt)    :
             (DEFINED(__MSICInterrupt)       ? ABSOLUTE(__MSICInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIDInterrupt)    ? ABSOLUTE(__AltMSIDInterrupt)    :
             (DEFINED(__MSIDInterrupt)       ? ABSOLUTE(__MSIDInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIEInterrupt)    ? ABSOLUTE(__AltMSIEInterrupt)    :
             (DEFINED(__MSIEInterrupt)       ? ABSOLUTE(__MSIEInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIFInterrupt)    ? ABSOLUTE(__AltMSIFInterrupt)    :
             (DEFINED(__MSIFInterrupt)       ? ABSOLUTE(__MSIFInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIGInterrupt)    ? ABSOLUTE(__AltMSIGInterrupt)    :
             (DEFINED(__MSIGInterrupt)       ? ABSOLUTE(__MSIGInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIHInterrupt)    ? ABSOLUTE(__AltMSIHInterrupt)    :
             (DEFINED(__MSIHInterrupt)       ? ABSOLUTE(__MSIHInterrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIDTInterrupt)   ? ABSOLUTE(__AltMSIDTInterrupt)   :
             (DEFINED(__MSIDTInterrupt)      ? ABSOLUTE(__MSIDTInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIWFEInterrupt)  ? ABSOLUTE(__AltMSIWFEInterrupt)  :
             (DEFINED(__MSIWFEInterrupt)     ? ABSOLUTE(__MSIWFEInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltMSIFLTInterrupt)  ? ABSOLUTE(__AltMSIFLTInterrupt)  :
             (DEFINED(__MSIFLTInterrupt)     ? ABSOLUTE(__MSIFLTInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltS1RSTInterrupt)   ? ABSOLUTE(__AltS1RSTInterrupt)   :
             (DEFINED(__S1RSTInterrupt)      ? ABSOLUTE(__S1RSTInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt142)     ? ABSOLUTE(__AltInterrupt142)     :
             (DEFINED(__Interrupt142)        ? ABSOLUTE(__Interrupt142)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt143)     ? ABSOLUTE(__AltInterrupt143)     :
             (DEFINED(__Interrupt143)        ? ABSOLUTE(__Interrupt143)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt144)     ? ABSOLUTE(__AltInterrupt144)     :
             (DEFINED(__Interrupt144)        ? ABSOLUTE(__Interrupt144)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt145)     ? ABSOLUTE(__AltInterrupt145)     :
             (DEFINED(__Interrupt145)        ? ABSOLUTE(__Interrupt145)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltS1BRKInterrupt)   ? ABSOLUTE(__AltS1BRKInterrupt)   :
             (DEFINED(__S1BRKInterrupt)      ? ABSOLUTE(__S1BRKInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt147)     ? ABSOLUTE(__AltInterrupt147)     :
             (DEFINED(__Interrupt147)        ? ABSOLUTE(__Interrupt147)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt148)     ? ABSOLUTE(__AltInterrupt148)     :
             (DEFINED(__Interrupt148)        ? ABSOLUTE(__Interrupt148)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCP7Interrupt)    ? ABSOLUTE(__AltCCP7Interrupt)    :
             (DEFINED(__CCP7Interrupt)       ? ABSOLUTE(__CCP7Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCT7Interrupt)    ? ABSOLUTE(__AltCCT7Interrupt)    :
             (DEFINED(__CCT7Interrupt)       ? ABSOLUTE(__CCT7Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt151)     ? ABSOLUTE(__AltInterrupt151)     :
             (DEFINED(__Interrupt151)        ? ABSOLUTE(__Interrupt151)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCP8Interrupt)    ? ABSOLUTE(__AltCCP8Interrupt)    :
             (DEFINED(__CCP8Interrupt)       ? ABSOLUTE(__CCP8Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCCT8Interrupt)    ? ABSOLUTE(__AltCCT8Interrupt)    :
             (DEFINED(__CCT8Interrupt)       ? ABSOLUTE(__CCT8Interrupt)       :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt154)     ? ABSOLUTE(__AltInterrupt154)     :
             (DEFINED(__Interrupt154)        ? ABSOLUTE(__Interrupt154)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt155)     ? ABSOLUTE(__AltInterrupt155)     :
             (DEFINED(__Interrupt155)        ? ABSOLUTE(__Interrupt155)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt156)     ? ABSOLUTE(__AltInterrupt156)     :
             (DEFINED(__Interrupt156)        ? ABSOLUTE(__Interrupt156)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltS1CLKFInterrupt)  ? ABSOLUTE(__AltS1CLKFInterrupt)  :
             (DEFINED(__S1CLKFInterrupt)     ? ABSOLUTE(__S1CLKFInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt158)     ? ABSOLUTE(__AltInterrupt158)     :
             (DEFINED(__Interrupt158)        ? ABSOLUTE(__Interrupt158)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt159)     ? ABSOLUTE(__AltInterrupt159)     :
             (DEFINED(__Interrupt159)        ? ABSOLUTE(__Interrupt159)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt160)     ? ABSOLUTE(__AltInterrupt160)     :
             (DEFINED(__Interrupt160)        ? ABSOLUTE(__Interrupt160)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt161)     ? ABSOLUTE(__AltInterrupt161)     :
             (DEFINED(__Interrupt161)        ? ABSOLUTE(__Interrupt161)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt162)     ? ABSOLUTE(__AltInterrupt162)     :
             (DEFINED(__Interrupt162)        ? ABSOLUTE(__Interrupt162)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt163)     ? ABSOLUTE(__AltInterrupt163)     :
             (DEFINED(__Interrupt163)        ? ABSOLUTE(__Interrupt163)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt164)     ? ABSOLUTE(__AltInterrupt164)     :
             (DEFINED(__Interrupt164)        ? ABSOLUTE(__Interrupt164)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt165)     ? ABSOLUTE(__AltInterrupt165)     :
             (DEFINED(__Interrupt165)        ? ABSOLUTE(__Interrupt165)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt166)     ? ABSOLUTE(__AltInterrupt166)     :
             (DEFINED(__Interrupt166)        ? ABSOLUTE(__Interrupt166)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt167)     ? ABSOLUTE(__AltInterrupt167)     :
             (DEFINED(__Interrupt167)        ? ABSOLUTE(__Interrupt167)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltADFIFOInterrupt)  ? ABSOLUTE(__AltADFIFOInterrupt)  :
             (DEFINED(__ADFIFOInterrupt)     ? ABSOLUTE(__ADFIFOInterrupt)     :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPEVTAInterrupt)   ? ABSOLUTE(__AltPEVTAInterrupt)   :
             (DEFINED(__PEVTAInterrupt)      ? ABSOLUTE(__PEVTAInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPEVTBInterrupt)   ? ABSOLUTE(__AltPEVTBInterrupt)   :
             (DEFINED(__PEVTBInterrupt)      ? ABSOLUTE(__PEVTBInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPEVTCInterrupt)   ? ABSOLUTE(__AltPEVTCInterrupt)   :
             (DEFINED(__PEVTCInterrupt)      ? ABSOLUTE(__PEVTCInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPEVTDInterrupt)   ? ABSOLUTE(__AltPEVTDInterrupt)   :
             (DEFINED(__PEVTDInterrupt)      ? ABSOLUTE(__PEVTDInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPEVTEInterrupt)   ? ABSOLUTE(__AltPEVTEInterrupt)   :
             (DEFINED(__PEVTEInterrupt)      ? ABSOLUTE(__PEVTEInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltPEVTFInterrupt)   ? ABSOLUTE(__AltPEVTFInterrupt)   :
             (DEFINED(__PEVTFInterrupt)      ? ABSOLUTE(__PEVTFInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCLC3PInterrupt)   ? ABSOLUTE(__AltCLC3PInterrupt)   :
             (DEFINED(__CLC3PInterrupt)      ? ABSOLUTE(__CLC3PInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCLC4PInterrupt)   ? ABSOLUTE(__AltCLC4PInterrupt)   :
             (DEFINED(__CLC4PInterrupt)      ? ABSOLUTE(__CLC4PInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCLC1NInterrupt)   ? ABSOLUTE(__AltCLC1NInterrupt)   :
             (DEFINED(__CLC1NInterrupt)      ? ABSOLUTE(__CLC1NInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCLC2NInterrupt)   ? ABSOLUTE(__AltCLC2NInterrupt)   :
             (DEFINED(__CLC2NInterrupt)      ? ABSOLUTE(__CLC2NInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCLC3NInterrupt)   ? ABSOLUTE(__AltCLC3NInterrupt)   :
             (DEFINED(__CLC3NInterrupt)      ? ABSOLUTE(__CLC3NInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltCLC4NInterrupt)   ? ABSOLUTE(__AltCLC4NInterrupt)   :
             (DEFINED(__CLC4NInterrupt)      ? ABSOLUTE(__CLC4NInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt181)     ? ABSOLUTE(__AltInterrupt181)     :
             (DEFINED(__Interrupt181)        ? ABSOLUTE(__Interrupt181)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt182)     ? ABSOLUTE(__AltInterrupt182)     :
             (DEFINED(__Interrupt182)        ? ABSOLUTE(__Interrupt182)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt183)     ? ABSOLUTE(__AltInterrupt183)     :
             (DEFINED(__Interrupt183)        ? ABSOLUTE(__Interrupt183)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt184)     ? ABSOLUTE(__AltInterrupt184)     :
             (DEFINED(__Interrupt184)        ? ABSOLUTE(__Interrupt184)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt185)     ? ABSOLUTE(__AltInterrupt185)     :
             (DEFINED(__Interrupt185)        ? ABSOLUTE(__Interrupt185)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt186)     ? ABSOLUTE(__AltInterrupt186)     :
             (DEFINED(__Interrupt186)        ? ABSOLUTE(__Interrupt186)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt187)     ? ABSOLUTE(__AltInterrupt187)     :
             (DEFINED(__Interrupt187)        ? ABSOLUTE(__Interrupt187)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltInterrupt188)     ? ABSOLUTE(__AltInterrupt188)     :
             (DEFINED(__Interrupt188)        ? ABSOLUTE(__Interrupt188)        :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltU1EVTInterrupt)   ? ABSOLUTE(__AltU1EVTInterrupt)   :
             (DEFINED(__U1EVTInterrupt)      ? ABSOLUTE(__U1EVTInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
        LONG( DEFINED(__AltU2EVTInterrupt)   ? ABSOLUTE(__AltU2EVTInterrupt)   :
             (DEFINED(__U2EVTInterrupt)      ? ABSOLUTE(__U2EVTInterrupt)      :
             ABSOLUTE(__DefaultInterrupt)));
    } > application_ivt

} /* SECTIONS */

