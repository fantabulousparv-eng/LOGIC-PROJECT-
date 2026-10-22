/**
 * @file main.cpp
 * @brief Command-line driver that connects Tasks 1-6.
 *
 * Usage:
 *  - `logic_tool demo`                         run the pipeline on two built-in formulas
 *  - `logic_tool formula "<infix>" [--report f.html]`   run the pipeline on one formula
 *  - `logic_tool cnf <a.cnf> [b.cnf ...] [options]`     analyse benchmark files
 *  - `logic_tool dir <folder> [options]`                analyse every .cnf file in a folder
 *
 * Options: `--report <file.html>` (default report.html), `--max-vars N` (default 20),
 *          `--no-recursive` (skip recursive variants).
 */
#include "common.h"
#include "dimacs.h"
#include "task1_infix_to_prefix.h"
#include "task2_prefix_to_tree.h"
#include "task3_traversals.h"
#include "task4_height.h"
#include "task5_evaluate.h"
#include "task6_analysis.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

using namespace std;

namespace fs = filesystem;

/** @brief Prints the result of every task for one (small) formula to the console. */
static void runConsoleDemo(const string &infix)
{
    cout << "=====================================================\n";
    cout << "Infix input : " << infix << "\n";

    string prefix = infixToPrefix(infix); // Task 1
    cout << "Task 1 prefix: " << prefix << "\n";

    Node *root = prefixToTreeRecursive(prefix); // Task 2
    Node *root2 = prefixToTreeIterative(prefix);
    cout << "Task 2 tree built (" << countNodes(root) << " nodes)\n";

    cout << "Task 3 in-order   rec : " << inOrderRecursive(root) << "\n";
    cout << "                  iter: " << inOrderIterative(root2) << "\n";
    cout << "       pre-order  rec : " << preOrderRecursive(root) << "\n";
    cout << "                  iter: " << preOrderIterative(root2) << "\n";
    cout << "       post-order rec : " << postOrderRecursive(root) << "\n";
    cout << "                  iter: " << postOrderIterative(root2) << "\n";

    cout << "Task 4 height rec: " << heightRecursive(root) << "  iter: " << heightIterative(root2) << "\n";

    cout << "Task 5 truth table:\n";
    TruthTableResult t = truthTable(root, &cout);
    if (t.computed)
        cout << (t.tautology ? "-> tautology" : t.satisfiable ? "-> satisfiable, not a tautology"
                                                              : "-> unsatisfiable")
             << " (" << t.satisfying << " of " << t.rows << " rows true)\n";
    else
        cout << "(skipped: too many atoms)\n";

    freeTree(root);
    freeTree(root2);
}

/** @brief Prints usage help. */
static void usage()
{
    cout << "Usage:\n"
            "  logic_tool demo\n"
            "  logic_tool formula \"<infix formula>\" [--report f.html]\n"
            "  logic_tool cnf <a.cnf> [b.cnf ...] [options]\n"
            "  logic_tool dir <folder> [options]\n"
            "Options: --report <file.html>  --max-vars N  --no-recursive\n";
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage();
        return 1;
    }
    string mode = argv[1];

    vector<string> positional;
    string reportPath;
    unsigned maxVars = 20;
    bool useRecursive = true;
    for (int i = 2; i < argc; ++i)
    {
        string a = argv[i];
        if (a == "--report" && i + 1 < argc)
            reportPath = argv[++i];
        else if (a == "--max-vars" && i + 1 < argc)
            maxVars = static_cast<unsigned>(stoul(argv[++i]));
        else if (a == "--no-recursive")
            useRecursive = false;
        else
            positional.push_back(a);
    }

    vector<FileMetrics> results;
    try
    {
        if (mode == "demo")
        {
            runConsoleDemo("((p+q)*(~r))");
            runConsoleDemo("((p>q)+(~(q*r)))");
            return 0;
        }
        else if (mode == "formula")
        {
            if (positional.empty())
            {
                usage();
                return 1;
            }
            runConsoleDemo(positional[0]);
            if (!reportPath.empty())
                results.push_back(analyseFormula("formula", positional[0], maxVars, useRecursive));
        }
        else if (mode == "cnf" || mode == "dir")
        {
            vector<string> files;
            if (mode == "cnf")
            {
                files = positional;
            }
            else
            {
                if (positional.empty())
                {
                    usage();
                    return 1;
                }
                for (const auto &e : fs::recursive_directory_iterator(positional[0]))
                    if (e.is_regular_file() && e.path().extension() == ".cnf")
                        files.push_back(e.path().string());
                sort(files.begin(), files.end());
            }
            if (files.empty())
            {
                cerr << "no .cnf files given/found\n";
                return 1;
            }
            if (reportPath.empty())
                reportPath = "report.html";
            for (const string &f : files)
            {
                cout << "Analysing " << f << " ... " << flush;
                try
                {
                    CnfInfo cnf = readDimacs(f);
                    string infix = cnfToInfix(cnf);
                    FileMetrics m = analyseFormula(fs::path(f).filename().string(), infix, maxVars, useRecursive);
                    m.numClauses = cnf.numClauses;
                    cout << (m.ok ? "ok" : "FAILED: " + m.error) << "\n";
                    results.push_back(m);
                }
                catch (const exception &e)
                {
                    FileMetrics m;
                    m.name = fs::path(f).filename().string();
                    m.error = e.what();
                    cout << "FAILED: " << e.what() << "\n";
                    results.push_back(m);
                }
            }
        }
        else
        {
            usage();
            return 1;
        }
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    if (!reportPath.empty())
    {
        writeHtmlReport(results, reportPath);
        cout << "HTML report written to " << reportPath << "\n";
    }
    return 0;
}
