# binutils

This repository intend to collect tools for binary analysis.</br>
Currently it contain a library (libelftool) that can be used to do elf64 parsing injection.</br>
It also contain a command line tool similar to readelf.</br>
See https://leonmariotto.github.io/binutils/

## Submodules

There is some submodule in there, so run:
```
git submodule update --init
```
When cloning repository.

## Building

There is some dependency:
- uv
- cppcheck
- graphviz
- gcovr

### Callgraphs

The submodule gcc_graph_tool is used to produce a simple callgraph from gcc output.</br>
This could be extended with stack usage for each function.</br>

## Static Analysis

### MISRA checker

The MISRA checker is a plugin of cppcheck. Configurations files is located under misra/ folder. </br>
The misra Makefile rules build a report with single line per warning (can be used to
easily check if there is more error in CI and reject PR)

## Testing

### Unit test

unit_test.mk is a simple Makefile that allow to build a single source file along with a test source file.</br>
It build object ready for coverage, run binary and use gcovr to produce coverage output.</br>
Test is run with CMocka, it output a JUnit xml.</br>
```
make -f unit_test.mk
```
unit_test.mk is made is controllable by caller script and generate a global coverture report.</br>
This is enabled in CI.</br>
The report is analyzed by codecov :
[![codecov](https://codecov.io/gh/leonmariotto/binutils/branch/main/graph/badge.svg)](https://codecov.io/gh/leonmariotto/binutils)
