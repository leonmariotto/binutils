# binutils

## Whatsup

There is some submodule in there, so run:
```
git submodule update --init
```
When cloning repository.

There is some dependency :
<ul>
    <li>- uv</li>
    <li>- cppcheck</li>
    <li>- graphviz</li>
    <li>- gcovr</li>
</ul>

This is mainly a reimplementation of nm in C, with some options readelf-like. </br>
Work in progress for binary injection. </br>
I use this repository to test some C developments tools and workflow. </br>

## Building

Assembly, preprocessing, object code.

### Callgraphs

The submodule gcc_graph_tool is used to produce a simple callgraph from gcc output.</br>
This could be extended with stack usage for each function</br>.
Screenshot of callgraph :
<img width="1105" height="685" alt="Screenshot from 2025-07-27 19-53-32" src="https://github.com/user-attachments/assets/42a33d9f-7bdc-4522-a5c2-e08d34eaa711" />


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
unit_test.mk is made to be controllable by caller script (TBD).
