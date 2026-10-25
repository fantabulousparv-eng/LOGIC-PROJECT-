
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include "common.h"

using namespace std;

// Create a node. The caller owns the returned tree.
static Node* makeNode(const string& value,
                      Node* left = nullptr,
                      Node* right = nullptr) {
    Node* node = new Node;
    node->value = value;
    node->left = left;
    node->right = right;
    return node;
}

// Convert a DIMACS clause into an OR tree.
// Positive 12 becomes x12; negative 12 becomes ~x12.
static Node* makeClause(const vector<int>& literals) {
    if (literals.empty()) {
        // An empty clause is always false.
        return makeNode("FALSE");
    }

    Node* clause = nullptr;

    for (int literal : literals) {
        string variable = "x" + to_string(abs(literal));
        Node* term = makeNode(variable);

        if (literal < 0) {
            term = makeNode("~", nullptr, term);
        }

        if (clause == nullptr) {
            clause = term;
        } else {
            clause = makeNode("+", clause, term);
        }
    }

    return clause;
}

// Read a DIMACS CNF file and build its parse tree.
Node* readDimacsCNF(const string& filename) {
    ifstream file(filename);

    if (!file) {
        throw runtime_error("Cannot open file: " + filename);
    }

    int variableCount = -1;
    int clauseCount = -1;
    int clausesRead = 0;

    Node* formula = nullptr;
    vector<int> currentClause;
    string line;

    try {
        while (getline(file, line)) {
            // Ignore leading whitespace.
            size_t start = line.find_first_not_of(" \t\r");

            if (start == string::npos) continue;
            line = line.substr(start);

            // Comment line.
            if (line[0] == 'c') continue;

            // Problem header: p cnf <variables> <clauses>
            if (line[0] == 'p') {
                string p, format;
                stringstream header(line);

                if (!(header >> p >> format
                             >> variableCount >> clauseCount) ||
                    p != "p" || format != "cnf" ||
                    variableCount < 0 || clauseCount < 0) {
                    throw runtime_error("Invalid DIMACS header");
                }

                continue;
            }

            // A clause may continue across lines.
            stringstream input(line);
            int literal;

            while (input >> literal) {
                if (literal == 0) {
                    Node* clause = makeClause(currentClause);
                    currentClause.clear();
                    ++clausesRead;

                    if (formula == nullptr) {
                        formula = clause;
                    } else {
                        formula = makeNode("*", formula, clause);
                    }
                } else {
                    if (variableCount < 0) {
                        throw runtime_error(
                            "Found clause data before DIMACS header");
                    }

                    if (literal == -2147483647 - 1 ||
                        abs(literal) > variableCount) {
                        throw runtime_error(
                            "Literal exceeds declared variable count");
                    }

                    currentClause.push_back(literal);
                }
            }
        }

        if (variableCount < 0 || clauseCount < 0) {
            throw runtime_error("DIMACS header not found");
        }

        if (!currentClause.empty()) {
            throw runtime_error(
                "A clause is missing its terminating zero");
        }

        if (clausesRead != clauseCount) {
            throw runtime_error(
                "Read clause count differs from header");
        }

        // A CNF with zero clauses is logically true.
        if (formula == nullptr) {
            formula = makeNode("TRUE");
        }

        cout << "\nDIMACS file loaded successfully.\n";
        cout << "Variables declared: " << variableCount << '\n';
        cout << "Clauses read: " << clausesRead << '\n';

        return formula;
    } catch (...) {
        deleteTree(formula);
        throw;
    }
}
