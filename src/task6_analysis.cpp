/** @file task6_analysis.cpp @brief Implementation of Task 6 (measurement + HTML report). */
#include "task6_analysis.h"
#include "common.h"
#include "task1_infix_to_prefix.h"
#include "task2_prefix_to_tree.h"
#include "task3_traversals.h"
#include "task4_height.h"
#include "task5_evaluate.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>

#if defined(__linux__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

using namespace std;

long peakRssKb()
{
#if defined(__linux__)
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_maxrss; // KB on Linux
#elif defined(__APPLE__)
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_maxrss / 1024; // bytes on macOS
#else
    return -1;
#endif
}

/** @brief Runs @p f once and returns the elapsed wall-clock time in milliseconds. */
template <typename F>
static double timeMs(F &&f)
{
    auto t0 = chrono::steady_clock::now();
    f();
    auto t1 = chrono::steady_clock::now();
    return chrono::duration<double, milli>(t1 - t0).count();
}

FileMetrics analyseFormula(const string &name, const string &infix, unsigned maxVars, bool useRecursive)
{
    FileMetrics m;
    m.name = name;
    m.infixChars = infix.size();
    Node *root = nullptr;
    try
    {
        auto stage = [&](const string &label, double ms)
        { m.stages.push_back({label, ms}); };

        // Task 1
        string prefix;
        stage("Task 1: infix to prefix", timeMs([&]
                                                { prefix = infixToPrefix(infix); }));

        // Task 2
        if (useRecursive)
        {
            Node *tmp = nullptr;
            stage("Task 2: prefix to tree (recursive)", timeMs([&]
                                                               { tmp = prefixToTreeRecursive(prefix); }));
            freeTree(tmp);
        }
        stage("Task 2: prefix to tree (iterative)", timeMs([&]
                                                           { root = prefixToTreeIterative(prefix); }));

        m.nodes = countNodes(root);
        m.treeBytes = m.nodes * sizeof(Node);

        // Task 3
        string inR, inI, preR, preI, postR, postI;
        if (useRecursive)
        {
            stage("Task 3: in-order (recursive)", timeMs([&]
                                                         { inR = inOrderRecursive(root); }));
            stage("Task 3: pre-order (recursive)", timeMs([&]
                                                          { preR = preOrderRecursive(root); }));
            stage("Task 3: post-order (recursive)", timeMs([&]
                                                           { postR = postOrderRecursive(root); }));
        }
        stage("Task 3: in-order (iterative)", timeMs([&]
                                                     { inI = inOrderIterative(root); }));
        stage("Task 3: pre-order (iterative)", timeMs([&]
                                                      { preI = preOrderIterative(root); }));
        stage("Task 3: post-order (iterative)", timeMs([&]
                                                       { postI = postOrderIterative(root); }));
        m.traversalsAgree = useRecursive ? (inR == inI && preR == preI && postR == postI) : true;
        m.roundTrip = (inI == stripSpaces(infix));

        // Task 4
        int h1 = 0, h2 = 0;
        if (useRecursive)
            stage("Task 4: height (recursive)", timeMs([&]
                                                       { h1 = heightRecursive(root); }));
        stage("Task 4: height (iterative)", timeMs([&]
                                                   { h2 = heightIterative(root); }));
        m.height = h2;
        if (useRecursive && h1 != h2)
            m.traversalsAgree = false;

        // Task 5: one evaluation under a reproducible pseudo-random assignment
        vector<string> atoms = collectAtoms(root);
        m.numVars = atoms.size();
        mt19937 rng(12345);
        vector<char> vals(atoms.size());
        for (char &v : vals)
            v = static_cast<char>(rng() & 1U);
        bool e1 = false, e2 = false;
        if (useRecursive)
            stage("Task 5: evaluate (recursive)", timeMs([&]
                                                         { e1 = evaluateRecursive(root, vals); }));
        stage("Task 5: evaluate (iterative)", timeMs([&]
                                                     { e2 = evaluateIterative(root, vals); }));
        m.evalAgree = useRecursive ? (e1 == e2) : true;
        m.sampleValue = e2;

        // Task 5: full truth table (only for small formulas)
        if (atoms.size() <= maxVars)
            stage("Task 5: full truth table", timeMs([&]
                                                     { m.table = truthTable(root, nullptr, maxVars); }));
        else
            m.table.numVars = atoms.size();

        m.peakRssKb = peakRssKb();
        m.ok = true;
    }
    catch (const exception &e)
    {
        m.ok = false;
        m.error = e.what();
    }
    freeTree(root);
    return m;
}

// HTML report

static string esc(const string &s)
{
    string r;
    for (char c : s)
    {
        switch (c)
        {
        case '&':
            r += "&amp;";
            break;
        case '<':
            r += "&lt;";
            break;
        case '>':
            r += "&gt;";
            break;
        case '"':
            r += "&quot;";
            break;
        default:
            r += c;
        }
    }
    return r;
}

static string fmt(double v, int prec = 3)
{
    ostringstream o;
    o << fixed << setprecision(prec) << v;
    return o.str();
}

static string human(size_t bytes)
{
    if (bytes >= (1u << 20))
        return fmt(bytes / 1048576.0, 2) + " MB";
    if (bytes >= 1024)
        return fmt(bytes / 1024.0, 1) + " KB";
    return to_string(bytes) + " B";
}

static const char *badge(bool ok) { return ok ? "<span class='b ok'>yes</span>" : "<span class='b bad'>no</span>"; }

void writeHtmlReport(const vector<FileMetrics> &results, const string &path)
{
    ofstream o(path);
    size_t okCount = 0;
    for (const auto &r : results)
        okCount += r.ok ? 1 : 0;

    o << R"(<!DOCTYPE html><html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Propositional Logic Parse Tree - Results &amp; Analysis</title>
<style>
:root{--bg:#0f1226;--card:#1a1e3a;--ink:#e8eaf6;--mut:#9aa3c7;--acc:#7c8cff;--acc2:#38d9a9;--bad:#ff6b8a;--line:#2b3160}
*{box-sizing:border-box}body{margin:0;font-family:Segoe UI,Roboto,Helvetica,Arial,sans-serif;background:linear-gradient(160deg,#0f1226,#161a3a);color:var(--ink);line-height:1.55}
header{padding:48px 6vw 28px;background:linear-gradient(120deg,#3a3f9e,#7c4dff 60%,#38d9a9);}
header h1{margin:0 0 6px;font-size:2rem}header p{margin:0;opacity:.9}
main{padding:24px 6vw 60px;max-width:1300px;margin:auto}
h2{margin:36px 0 12px;color:var(--acc);border-bottom:1px solid var(--line);padding-bottom:6px}
.cards{display:grid;grid-template-columns:repeat(auto-fit,minmax(190px,1fr));gap:14px}
.card{background:var(--card);border:1px solid var(--line);border-radius:14px;padding:16px}
.card .v{font-size:1.7rem;font-weight:700;color:var(--acc2)}.card .l{color:var(--mut);font-size:.85rem}
table{width:100%;border-collapse:collapse;background:var(--card);border-radius:12px;overflow:hidden;font-size:.9rem}
th,td{padding:9px 12px;text-align:right;border-bottom:1px solid var(--line)}th{background:#222755;color:#cfd4ff;text-align:right;white-space:nowrap}
td:first-child,th:first-child{text-align:left}tr:hover td{background:#20254a}
.wrap{overflow-x:auto;border-radius:12px}
.b{padding:2px 9px;border-radius:99px;font-size:.78rem;font-weight:600}.ok{background:#12352f;color:var(--acc2)}.bad{background:#3d1a28;color:var(--bad)}
details{background:var(--card);border:1px solid var(--line);border-radius:12px;margin:10px 0;padding:10px 16px}summary{cursor:pointer;font-weight:600}
code{background:#12163a;padding:1px 6px;border-radius:5px;color:#c7d0ff}.note{color:var(--mut);font-size:.9rem}
footer{text-align:center;color:var(--mut);padding:24px}
</style></head><body>
<header><h1>Propositional Logic Parse Trees</h1>
<p>CS F214 Logic in CS &middot; Assignment 1 &middot; Part A &mdash; results, efficiency and memory analysis</p></header><main>)";

    o << "<h2>Summary</h2><div class='cards'>"
      << "<div class='card'><div class='v'>" << results.size() << "</div><div class='l'>formulas / files analysed</div></div>"
      << "<div class='card'><div class='v'>" << okCount << "</div><div class='l'>processed successfully</div></div>";
    size_t totalNodes = 0;
    int maxH = 0;
    for (const auto &r : results)
    {
        totalNodes += r.nodes;
        if (r.height > maxH)
            maxH = r.height;
    }
    o << "<div class='card'><div class='v'>" << totalNodes << "</div><div class='l'>parse-tree nodes built in total</div></div>"
      << "<div class='card'><div class='v'>" << maxH << "</div><div class='l'>largest tree height (levels)</div></div></div>";

    o << "<h2>Results per file</h2><div class='wrap'><table><tr><th>File / formula</th><th>Vars</th><th>Clauses</th>"
         "<th>Nodes</th><th>Height</th><th>Tree memory</th><th>Peak RSS</th><th>Rec = Iter</th><th>Round trip</th>"
         "<th>Truth table</th></tr>";
    for (const auto &r : results)
    {
        o << "<tr><td>" << esc(r.name) << "</td>";
        if (!r.ok)
        {
            o << "<td colspan='9' style='text-align:left'><span class='b bad'>error</span> " << esc(r.error) << "</td></tr>";
            continue;
        }
        o << "<td>" << r.numVars << "</td><td>" << (r.numClauses ? to_string(r.numClauses) : "-") << "</td><td>" << r.nodes
          << "</td><td>" << r.height << "</td><td>" << human(r.treeBytes) << "</td><td>"
          << (r.peakRssKb >= 0 ? human(static_cast<size_t>(r.peakRssKb) * 1024) : "n/a") << "</td><td>"
          << badge(r.traversalsAgree && r.evalAgree) << "</td><td>" << badge(r.roundTrip) << "</td><td>";
        if (r.table.computed)
            o << r.table.satisfying << " / " << r.table.rows << " true"
              << (r.table.tautology ? " (tautology)" : (r.table.satisfiable ? "" : " (unsatisfiable)"));
        else
            o << "skipped (" << r.numVars << " atoms)";
        o << "</td></tr>";
    }
    o << "</table></div><p class='note'>Tree memory = nodes &times; sizeof(Node) = " << sizeof(Node)
      << " bytes per node (a lower bound). Peak RSS is the whole process, so it only ever grows from file to file. "
         "&ldquo;Round trip&rdquo; = in-order output equals the original infix input.</p>";

    o << "<h2>Running time per algorithm (milliseconds)</h2>";
    for (const auto &r : results)
    {
        if (!r.ok)
            continue;
        o << "<details><summary>" << esc(r.name) << "</summary><div class='wrap'><table><tr><th>Algorithm</th><th>Time (ms)</th></tr>";
        for (const auto &s : r.stages)
            o << "<tr><td>" << esc(s.name) << "</td><td>" << fmt(s.ms) << "</td></tr>";
        o << "</table></div></details>";
    }

    o << R"(<h2>Complexity analysis</h2><p class="note">n = number of tokens / tree nodes, h = tree height, w = maximum tree width, v = distinct atoms.</p>
<div class="wrap"><table><tr><th>Algorithm</th><th>Time</th><th>Extra space</th><th>Remark</th></tr>
<tr><td>Task 1: infix &rarr; prefix</td><td>O(n)</td><td>O(n)</td><td style="text-align:left">Each token is pushed/popped at most once.</td></tr>
<tr><td>Task 2: prefix &rarr; tree (recursive)</td><td>O(n)</td><td>O(h) stack</td><td style="text-align:left">One call per token; deep trees can overflow the call stack.</td></tr>
<tr><td>Task 2: prefix &rarr; tree (iterative)</td><td>O(n)</td><td>O(n)</td><td style="text-align:left">Explicit heap stack, no overflow risk.</td></tr>
<tr><td>Task 3: traversals (recursive)</td><td>O(n)</td><td>O(h) stack</td><td style="text-align:left">Every node visited once.</td></tr>
<tr><td>Task 3: traversals (iterative)</td><td>O(n)</td><td>O(h)</td><td style="text-align:left">Post-order list variant uses O(n) for the node list.</td></tr>
<tr><td>Task 4: height (recursive)</td><td>O(n)</td><td>O(h) stack</td><td style="text-align:left">1 + max(height(left), height(right)).</td></tr>
<tr><td>Task 4: height (iterative, BFS)</td><td>O(n)</td><td>O(w)</td><td style="text-align:left">Queue holds one level at a time.</td></tr>
<tr><td>Task 5: one evaluation</td><td>O(n)</td><td>O(h) / O(n)</td><td style="text-align:left">Bottom-up, every node evaluated once.</td></tr>
<tr><td>Task 5: truth table</td><td>O(2<sup>v</sup> &middot; n)</td><td>O(n)</td><td style="text-align:left">Exponential in the number of atoms, so it is only run when v is small.</td></tr>
<tr><td>Memory of the tree</td><td>&ndash;</td><td>O(n)</td><td style="text-align:left">One Node per token; a balanced tree has h = O(log n).</td></tr>
</table></div>
<h2>Observations</h2>
<details open><summary>Recursive vs iterative</summary><p>Both versions of every task produce identical output and have the same asymptotic running time. The iterative ones replace the call stack with a heap-allocated stack, so they also work on trees whose height would overflow the recursion stack. The benchmark converter builds balanced trees (h = O(log n)), which is why the recursive versions are safe on the SAT files.</p></details>
<details open><summary>Why the truth table is skipped for big benchmarks</summary><p>A formula with v atoms has 2<sup>v</sup> rows, so even 40 atoms is out of reach. SAT benchmark files usually have hundreds to thousands of variables, therefore they are evaluated under one assignment instead of being enumerated.</p></details>
<details open><summary>Memory usage</summary><p>The parse tree needs one node per token. In a CNF with L literals and C clauses the tree has about 2L + C nodes (L leaves, ~L-C OR nodes, C-1 AND nodes and the negation nodes), i.e. memory is linear in the size of the input.</p></details>
</main><footer>Generated by the Part A project &middot; documentation: see the Doxygen pages in docs/html</footer></body></html>)";
}
