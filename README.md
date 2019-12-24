/Name TBD/: A 16-bit hardware FM synthesizer
============================================
The goal of this project is to create an 16-bit hardware FM synth.  This is
still in the very early stages, and there are a great many design decisions that
have not yet been made.  It also needs a name.

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
Software build instructions coming soon.

Testing
-------
Unit tests are run with Ceedling.  To run the tests, install Ceedling 0.28.3 or
later.  Then, simply `cd` into this directory and run `ceedling`.  More
information about Ceedling and the underlying utilities and frameworks can be
found at [throwtheswitch.org](https://www.throwtheswitch.org/ceedling).

