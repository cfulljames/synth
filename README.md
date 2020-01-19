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
This project uses a Microchip dsPIC33CH128MP508 as the target platform.  This
"digital signal controller" provides a good balance of DSP and MCU functions in
a highly affordable package.  It's not exactly meant for audio, but it has the
necessary features to be used for this purpose.

Building
--------

### Requirements
To build the software you'll need the following installed on your system:

- [CMake](https://cmake.org)
- [Python 3.7 or later](https://python.org)
- Microchip's [MPLAB IDE](https://www.microchip.com/mplab/mplab-x-ide)
- Microchip's [XC16 Toolchain](https://www.microchip.com/mplab/compilers)

#### A Note on the MPLAB Build System
MPLAB's build system is a bit limited, especially for dual core devices like the
dsPIC33C series.  Many of the features (like user Makefile projects) don't work
quite as expected.  To work around this, a normal "Standalone" project has been
created for each core, but the Makefiles for each project have been modified to
skip the MPLAB build and instead run a custom CMake build.  This allows for much
greater flexibility in generating files and tracking dependencies.

### Setup
After cloning the repo, copy the file `mplab/conf.mk.defaults` to
`mplab/conf.mk` and modify the values with the appropriate paths for your
system.  The defaults supplied are for macOS.

### MPLAB IDE

#### Who is this for?
This method is especially useful for debugging, or if you like working in MPLAB
IDE.  If you prefer a command line build process, see below.

#### Procedure
After cloning the repo, launch MPLAB IDE and open the two projects contained in
the `mplab` directory.  From there, the build and debug commands should work as
expected.  If CMake reports any errors, check that your `conf.mk` file contains
the correct paths.

### Command Line

#### Who is this for?
This method is useful if you prefer a command line interface for building,
expecially if you're familiar with GNU Make and CMake.  A word of caution: The
command line debugging support (via MDB) is pretty bad.  If you prefer a
graphical experience or if you want advanced debugging capabilities, see the
MPLAB procedure above.

#### Procedure
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

Debugging
---------
Debugging this project is probably a bit more work than you're used to.  The
dsPIC33C family contains two (mostly) independent dsPIC cores in a single
package, and they can be debugged in three ways:

- Master-only: The slave core runs freely while the master core is debugged.
- Slave-only: The master core runs freely while the slave core is debugged.
- Dual simultaneous debug: Both cores are debugged simultaneously.

This requires some additional configuration and/or code modification depending
on the setup.  See
[Microchip's Dual Core Documentation](http://ww1.microchip.com/downloads/en/AppNotes/AN2721,-Getting-Started-with-Dual-Core-DS00002721A.pdf)
for details.

Testing
-------
Unit tests are run with Ceedling.  To run the tests, install Ceedling 0.28.3 or
later.  Then, simply `cd` into this directory and run `ceedling`.  More
information about Ceedling and the underlying utilities and frameworks can be
found at [throwtheswitch.org](https://www.throwtheswitch.org/ceedling).

