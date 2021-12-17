# Solver template

UNDER CONSTRUCTION.

This is a minimal solver template for the CowSwap Solver competition. Documentation (that is still incomplete) about how to build a Solver can be found [here](https://docs.google.com/document/d/1PUJfJ47eVbCSx5At1eZCHWW5NByV40JZsQWgtw1VDoI/edit?usp=sharing).

The functionality is minimal. So far, input in json form (provided by the Driver) is parsed and tokens, orders and AMMs are stored in appropriate data structures. The main function simply prints the input. Regarding AMMs, only Constant-Product AMMs have been implemented.

A sample json input file can be found in ```./sample.json```

The Boost Library is needed. It can be downloaded from [here](https://boostorg.jfrog.io/artifactory/main/release/1.77.0/source/boost_1_77_0.7z). The extracted ```/boost/``` subfolder must be moved to the root directory of the project.

To compile, simply run ```make``` in the terminal, and to execute, just run ```./main```.
