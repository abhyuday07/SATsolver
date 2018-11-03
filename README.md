# SATsolver
Implementation of SAT solver using improvised semantic tableaux.

Source Code Dependencies
------------------------
The code has a strict dependency on C++ 2017 standard. No other dependencies as such.

Input
------
The input file "sat.txt" should be placed in the same folder as the source code. The input file must be in DIMACS format without any comments. There shan't be any conflicting number of variables.

Output
-------
The output will consist of four parts:
1. SATISFIABLE or UNSAT
2. A model if the formulae is SATISFIABLE
3. Number of calls to DP.
4. Execution time in seconds

Heuristics
-------
We have applied the following heuristics:
1. Pure Literals: They are the literals that for which their negation is not present in any of the clauses. 
2. Maximum Occurrence: We proceed in solving by removing a literal with maximum occurence. It will be useful since will reduce the size of maximum number of clauses.

Some Other Steps
----------------
1. We remove the singular clauses(the clauses with only one literal).
2. The literals present in the singular clauses are assigned true and they are then removed.
3. If the literal is 'a' the all the clauses containing 'a' are removed and '-a' is removed from each clause.

Compile and Run
---------------
1. g++ -std=c++17 satdpll.cpp
2. ./a.out
