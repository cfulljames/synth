/Name TBD/: A 16-bit hardware FM synthesizer
============================================
The goal of this project is to create a 16-bit hardware FM synth.  This is still
in the very early stages, and there are a great many design decisions that have
not yet been made.  It also needs a name.

Many of my inspirations for this project come from the synth chips and video
games of the late '80s and early '90s.  The sounds of the Game Boy and the Sega
Genesis are certainly big inspirations, but the aim of this project is not to
reproduce these sounds, but rather to pay homage to this time period in the
context of a fully modern synthesizer.

Platform
--------
Currently, the plan is to use a Microchip dsPIC33CH as the basis for this
project.  This "digital signal controller" provides a good balance of DSP and
MCU functions in a highly affordable package.  It's not exactly meant for audio,
but it has the necessary features to be used for this purpose.

Building
--------

### Requirements
To build the software you'll need the following installed on your system:

- [CMake](https://cmake.org)
- Microchip's [MPLAB IDE](https://www.microchip.com/mplab/mplab-x-ide)
- Microchip's [XC16 Toolchain](https://www.microchip.com/mplab/compilers)

The IDE isn't used for building, but it will be needed for flashing the code.
It's also the most convenient way to install the XC16 toolchain.

Ensure that `cmake` and the XC16 toolchain (`xc16-gcc`, etc.) are in your system
`PATH`.

### Procedure
After cloning the repo, `cd` into this directory and run the following commands:

```
mkdir build
cd build
cmake ..
make
```

If successful, this will create the file `synth_master.hex` in
`build/src/master/`.  This can be loaded onto the target using MPLAB IPE
(installed with the IDE).

If you're on a macOS or Linux based system, you can also flash using the
following method:

- Add `mdb.sh` to your system `PATH` from the MPLAB IDE files (in my
  installation, this showed up in
  `/Applications/microchip/mplabx/v5.25/mplab_platform/bin`).
- Run `make flash`

This will launch MDB on the command line to load the program.

Testing
-------
Unit tests are run with Ceedling.  To run the tests, install Ceedling 0.28.3 or
later.  Then, simply `cd` into this directory and run `ceedling`.  More
information about Ceedling and the underlying utilities and frameworks can be
found at [throwtheswitch.org](https://www.throwtheswitch.org/ceedling).

