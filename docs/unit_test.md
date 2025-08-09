# Unitary test

There is a unit_test.mk that is reponsible to build one single unit test, with variable controllable by the caller.

The caller is run_unit_test.py which have a variable unit_list at the top of file.

For each unit in unit list it will build, run, generate json and html report. Each json report is then merged into a
super report that is output in html and xml (cobertura) form.

Unit test sources use cmocka to output result in Junit format.
