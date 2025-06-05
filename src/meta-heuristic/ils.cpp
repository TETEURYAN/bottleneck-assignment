#include "ils.hpp"
#include <random>
#include <chrono>
#include <algorithm>
#include <climits>

#include "../bap.hpp"

using namespace std;
using namespace bap;

namespace ils {

int evaluateGargalo(const CostMatrix& cost, const std::vector<int>& assignment) {
    int g = 0;
    for (int i = 0; i < (int)cost.size(); ++i)
        g = std::max(g, cost[i][assignment[i]]);
    return g;
}

std:: vector<int> solveGreedy (const CostMatrix& cost) {
    int n = cost.size();
    std::vector<int> assignment(n);
    std::vector<bool> used(n, false);

    for (int i = 0; i < n; ++i) {
        int minCost = INT_MAX;
        int bestJ = -1;

        for (int j = 0; j < n; ++j) {
            if (!used[j] && cost[i][j] < minCost) {
                minCost = cost[i][j];
                bestJ = j;
            }
        }

        assignment[i] = bestJ;
        used[bestJ] = true;
    }

    return assignment;
}

std::vector<std::vector<int>> vnd_heuristica(
    const CostMatrix& cost,
    const std::vector<int>& currentAssignment,
    int originalGargalo
) {
    int n = cost.size();
    std::vector<std::vector<int>> lambda(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        int j = currentAssignment[i];
        int originalCost = cost[i][j];
        int delta = 0;

        while (true) {
            ++delta;
            int newCost = originalCost + delta;
            if (newCost > originalGargalo) break;

            CostMatrix modified = cost;
            modified[i][j] = newCost;

            int g = evaluateGargalo(modified, currentAssignment);
            if (g > originalGargalo) break;

            lambda[i][j] = delta;
        }
    }

    return lambda;
}

std::vector<int> ILSSolver::solve_ils(
    const CostMatrix& cost,
    int maxIter,
    int bestGargalo,
    const std::vector<int>& initialAssignment,
    const std::vector<std::vector<int>>& initialLambda,
    int& finalGargalo,
    std::vector<std::vector<int>>& finalLambda
) {
    int n = cost.size();
    std::vector<int> best = initialAssignment;
    std::vector<int> current = best;
    std::vector<int> perturbed;
    finalGargalo = bestGargalo;
    finalLambda = initialLambda;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    for (int iter = 0; iter < maxIter; ++iter) {
        perturbed = current;

        for (int p = 0; p < 3; ++p) {
            int i = rng() % n;
            int j = rng() % n;
            if (i != j) swap(++perturbed[i], perturbed[j]);
        }

        for (int i = 0; i < n; ++i) {
            for (int k = i + 1; k < n; ++k) {
                swap(perturbed[i], perturbed[k]);
                int newGargalo = evaluateGargalo(cost, perturbed);
                if (newGargalo > finalGargalo)
                    swap(perturbed[i], perturbed[k]);
            }
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                int temp = perturbed[i];
                perturbed.erase(perturbed.begin() + i);
                perturbed.insert(perturbed.begin() + j, temp);
                int newGargalo = evaluateGargalo(cost, perturbed);
                if (newGargalo > finalGargalo) {
                    perturbed.erase(perturbed.begin() + j);
                    perturbed.insert(perturbed.begin() + i, temp);
                }
            }
        }

        int pertGargalo = evaluateGargalo(cost, perturbed);

        if (pertGargalo < finalGargalo) {
            auto λ = vnd_heuristica(cost, perturbed, pertGargalo);
            int countRobust = 0, countBestRobust = 0;

            for (int i = 0; i < n; ++i)
                if (λ[i][perturbed[i]] > 0) countRobust++;
            for (int i = 0; i < n; ++i)
                if (finalLambda[i][best[i]] > 0) countBestRobust++;

            if ((countRobust > countBestRobust) ||
                (countRobust == countBestRobust && pertGargalo <= finalGargalo)) {
                best = perturbed;
                finalGargalo = pertGargalo;
                finalLambda = λ;
            }
        }
    }

    return best;
}

} // namespace ils
