# binutils

## Whatsup

Also it may include other things : documentation generation,
test framework, ... <\br>
There is some submodule in there, so run:
```
git submodule update --init
```
When cloning repository.

There is some dependency :
    - uv
    - cppcheck
    - graphviz
    - gcovr

## Building

Assembly, preprocessing, object code.

### Callgraphs

The submodule gcc_graph_tool is used to produce a simple callgraph from gcc output.
This could be extended with stack usage for each function.

## Static Analysis

### MISRA checker

The MISRA checker is a plugin of cppcheck. Configurations files is located under misra/ folder. <\br>
The misra Makefile rules build a report with single line per warning (can be used to
easily check if there is more error in CI and reject PR)

## Testing

### Unit test

unit_test.mk is a simple Makefile that allow to build a single source file along with a test source file.
It build object ready for coverage, run binary and use gcovr to produce coverage output.
Test is run with CMocka, it output a JUnit xml.
```
make -f unit_test.mk
```
unit_test.mk is made to be controllable by caller script (TBD).
