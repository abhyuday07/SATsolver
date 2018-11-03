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
2. Removing Tautologies: They are the clauses in which 'a' and '-a' both are present.
3. Maximum Occurence: We proceed in solving by removing a literal with maximum occurence. It will be useful since will reduce the size of maximum number of clauses.
4.
