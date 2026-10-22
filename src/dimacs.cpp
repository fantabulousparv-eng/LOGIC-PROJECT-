/** @file dimacs.cpp @brief Implementation of the DIMACS reader / CNF-to-infix converter. */
#include "dimacs.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

CnfInfo readDimacs(const string &path)
{
    ifstream in(path);
    if (!in)
        throw runtime_error("cannot open file: " + path);

    CnfInfo cnf;
    vector<int> current;
    string line;
    bool stop = false;
    while (!stop && getline(in, line))
    {
        size_t p = line.find_first_not_of(" \t\r");
        if (p == string::npos)
            continue;
        char c = line[p];
        if (c == 'c')
            continue; // comment
        if (c == '%')
            break; // end marker used by some benchmark sets
        if (c == 'p')
        { // header: p cnf V C
            istringstream hs(line.substr(p + 1));
            string fmt;
            size_t v = 0;
            hs >> fmt >> v;
            cnf.numVars = v;
            continue;
        }
        istringstream ls(line);
        long lit;
        while (ls >> lit)
        {
            if (lit == 0)
            {
                if (!current.empty())
                    cnf.clauses.push_back(current);
                current.clear();
            }
            else
            {
                current.push_back(static_cast<int>(lit));
            }
        }
    }
    if (!current.empty())
        cnf.clauses.push_back(current); // last clause without trailing 0
    cnf.numClauses = cnf.clauses.size();
    if (cnf.clauses.empty())
        throw runtime_error("no clauses found in " + path);
    return cnf;
}

/** @brief Appends items[lo,hi) joined by @p op as a balanced, fully parenthesized expression. */
static void joinBalanced(const vector<string> &items, size_t lo, size_t hi, char op, string &out)
{
    if (hi - lo == 1)
    {
        out += items[lo];
        return;
    }
    size_t mid = lo + (hi - lo) / 2;
    out += '(';
    joinBalanced(items, lo, mid, op, out);
    out += op;
    joinBalanced(items, mid, hi, op, out);
    out += ')';
}

string cnfToInfix(const CnfInfo &cnf)
{
    vector<string> clauseStrs;
    clauseStrs.reserve(cnf.clauses.size());
    for (const auto &clause : cnf.clauses)
    {
        vector<string> lits;
        lits.reserve(clause.size());
        for (int l : clause)
        {
            string name = "x" + to_string(abs(l));
            lits.push_back(l < 0 ? "(~" + name + ")" : name);
        }
        string s;
        joinBalanced(lits, 0, lits.size(), '+', s);
        clauseStrs.push_back(move(s));
    }
    string formula;
    joinBalanced(clauseStrs, 0, clauseStrs.size(), '*', formula);
    return formula;
}
