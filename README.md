firrtl-sig ![Build Status](https://github.com/ucsc-vama/firrtl-sig/actions/workflows/c-cpp.yml/badge.svg)
===================
This repo provides C++ implementations of the UInt and SInt types described in the [firrtl](https://github.com/freechipsproject/firrtl) [spec](https://github.com/ucb-bar/firrtl/blob/master/spec/spec.pdf). The signals are templated by bit width, and the operators are designed to conform to the rules of the spec. Most of the assertions to check safety of operations will be performed at compile time.

To test the repo using short unit tests (after pulling in the submodules):

    $ make test


Legal
-----
Essential Signal Simulation Enabled by Netlist Transforms (ESSENT) Copyright (c) 2019, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy). All rights reserved.

If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Intellectual Property Office at IPO@lbl.gov.

NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so.
