# Summary

This repository intend to collect tools for binary exploitation.
There is only an elf parser (readelf-like) [elftool](elftool.md) for now.

Compilation is done through a Makefile which have some special rules :

- misra: a static analysis pass is done a source code and a misra report is generated in build/misra_report.txt.
- [gcc_graph_tool](gcc_graph_tool.md): generate a callgraph of the binary.

Also a [unitary test](unit_test.md) system is present and some unit tests are implemented.
