Unit Tests for Assembly Code
============================
Some of the critical DSP functions are written in assembly, and therefore cannot
be run on the host system.  These tests still use Unity, but rather than relying
on Ceedling's version, they use the one in `lib/unity`.  These tests are built
using CMake and run on the target.
