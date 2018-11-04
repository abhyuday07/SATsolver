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
1. Unit Propagation: Unit propagation is a procedure of automated theorem proving that can simplify a set of (usually propositional) clauses.
2. Pure Literals: They are the literals that for which their negation is not present in any of the clauses.
3. Maximum Occurrence: We proceed in solving by removing a literal with maximum occurrence. It will be useful since will reduce the size of maximum number of clauses.
4. Removal of Tautologies: They are the clauses which contain literals of the form 'a' and '-a'.
5. Elimination of singular clauses: They are the clauses with only one literal. Say, the literal present in the clause is 'a'. Then, 'a' would be asserted 'TRUE' and all the clauses containing 'a' would be eliminated. Now the literals '-a' are 'FALSE'. Therefore, they are eliminated from each and every clause.



Compile and Run
---------------

1. g++ -std=c++17 satdpll.cpp
2. ./a.out
