#include "bap.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <queue>
#include <functional>

using namespace std;

namespace bap {

    void printMatrix(const CostMatrix& matrix) {
        for (const auto& row : matrix) {
            for (int val : row) cout << val << "\t";
            cout << "\n";
        }
    }

    bool isFeasible(const CostMatrix& cost, int threshold, vector<int>& match) {
        int n = (int)cost.size();
        vector<vector<int>> adj(n);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (cost[i][j] <= threshold)
                    adj[i].push_back(j);

        match.assign(n, -1);
        vector<bool> visited(n);

        function<bool(int)> bpm = [&](int u) {
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    if (match[v] == -1 || bpm(match[v])) {
                        match[v] = u;
                        return true;
                    }
                }
            }
            return false;
        };

        for (int u = 0; u < n; ++u) {
            fill(visited.begin(), visited.end(), false);
            if (!bpm(u)) return false;
        }

        return true;
    }

    int solveBAP(const CostMatrix& cost, vector<int>& assignment) {
        // int n = (int)cost.size();
        int low = 0, high = 0;
        for (const auto& row : cost)
            for (int c : row)
                high = max(high, c);

        int result = high;
        while (low <= high) {
            int mid = (low + high) / 2;
            vector<int> match;
            if (isFeasible(cost, mid, match)) {
                result = mid;
                high = mid - 1;
                assignment = match;
            } else {
                low = mid + 1;
            }
        }
        return result;
    }

    // Função para calcular as margens de robustez λ₍ᵢⱼ₎
    vector<vector<int>> computeLambda(const vector<vector<int>>& cost, const vector<int>& assignment, int optimalCost) {
        int n = cost.size();
        vector<vector<int>> lambda(n, vector<int>(n, 0));
        const int maxDelta = 100;

        for (int i = 0; i < n; ++i) {
            int j = assignment[i];
            int originalCost = cost[i][j];
            int delta = 0;

            while (delta < maxDelta) {
                vector<vector<int>> modifiedCost = cost;
                modifiedCost[i][j] = originalCost + delta + 1;
                vector<int> newAssignment;
                int newOptimal = solveBAP(modifiedCost, newAssignment);

                // Verifica se a atribuição mudou ou o custo piorou
                if (newOptimal > optimalCost || newAssignment[j] != i)
                    break;

                ++delta;
            }

            lambda[i][j] = delta;  // Tolerância máxima que mantém (i,j) na solução ótima
        }

        return lambda;
    }

    vector<int> solve_ils(const CostMatrix& cost, int maxIter, int& gargalo, vector<vector<int>>& lambda) {
        vector<int> assignment;
        int initialGargalo = solveBAP(cost, assignment);
        auto lambdaInit = ils::vnd_heuristica(cost, assignment, initialGargalo);

        ils::ILSSolver solver;
        return solver.solve_ils(cost, maxIter, initialGargalo, assignment, lambdaInit, gargalo, lambda);
    }


} // namespace bap

