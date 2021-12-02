# Solver prototype

Minimal functionality. So far, input in json form is parsed and tokens and orders are stored in appropriate data structures. The main function simply prints the input. 

AMMs are ignored for the time being, and I am currently implementing them; a PR tonight will provide a basic implementation of constant-product AMMs.

A sample json file can be found in ```./sample.json```

The Boost Library is needed. It can be downloaded from [here](https://boostorg.jfrog.io/artifactory/main/release/1.77.0/source/boost_1_77_0.7z). The extracted ```/boost/``` subfolder must be moved to the root directory of the project.

To compile, simply run ```make``` in the terminal, and to execute, just run ```./main```.
