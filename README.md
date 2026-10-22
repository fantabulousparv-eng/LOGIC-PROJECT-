# CS F214 Logic in CS - Assignment 1, Part A

Propositional-logic formulas stored as binary parse trees (C++17).

## Build and run
    make                                   # builds ./logic_tool
    ./logic_tool demo                      # built-in formulas, Tasks 1-5 printed
    ./logic_tool formula "((p+q)*(~r))"    # your own fully parenthesized formula
    ./logic_tool cnf a.cnf b.cnf --report report.html
    ./logic_tool dir path/to/sat2002 --report report.html
    make docs                              # Doxygen -> docs/html/index.html (needs doxygen)

Options: --report <file.html>, --max-vars N (truth-table limit, default 20), --no-recursive.

## Layout
- src/common.*                 shared Node struct and helpers
- src/task1_infix_to_prefix.*  Task 1
- src/task2_prefix_to_tree.*   Task 2
- src/task3_traversals.*       Task 3
- src/task4_height.*           Task 4
- src/task5_evaluate.*         Task 5
- src/dimacs.*                 SAT .cnf reader and CNF-to-infix converter
- src/task6_analysis.*         Task 6: timing, memory, HTML report
- src/main.cpp                 driver connecting all tasks
- docs/html/                   generated Doxygen documentation
- data/sample_uf12.cnf         tiny sample CNF for a quick test (NOT from SAT 2002)

Place the SAT 2002 .cnf files in a folder (decompress .gz/.bz2 first) and run the `dir` command.
