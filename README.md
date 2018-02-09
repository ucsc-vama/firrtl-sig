firrtl-sig
===================
This repo provides C++ implementations of the UInt and SInt types described in the [firrtl](https://github.com/freechipsproject/firrtl) [spec](https://github.com/ucb-bar/firrtl/blob/master/spec/spec.pdf). The signals are templated by bit width, and the operators are designed to conform to the rules of the spec, and most properties will be checked at compile time.

To test the repo using short unit tests:

    $ make
