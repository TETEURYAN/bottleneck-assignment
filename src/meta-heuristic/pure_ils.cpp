#include "ils.hpp"
#include "vnd.hpp"

#include <random>
#include <chrono>
#include <algorithm>

namespace ils {

int evaluateGargalo(const std::vector<std::vector<int>>& cost, const std::vector<int>& assignment) {
    int g = 0;
    for (int i = 0; i < (int)cost.size(); ++i)
        g = std::max(g, cost[i][assignment[i]]);
    return g;
}

std::vector<int> ILSSolver::run_ils(
    const CostMatrix& cost,
    const std::vector<int>& initialAssignment,
    int maxIterations
) {
    int n = cost.size();
    std::vector<int> best = initialAssignment;
    std::vector<int> current = best;
    std::vector<int> perturbed;

    int bestGargalo = evaluateGargalo(cost, best);

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

    for (int iter = 0; iter < maxIterations; ++iter) {
        perturbed = current;

        // Perturbação: aplicar 3 swaps aleatórios
        for (int p = 0; p < 3; ++p) {
            int i = rng() % n;
            int j = rng() % n;
            if (i != j) std::swap(perturbed[i], perturbed[j]);
        }

        // Aplicar busca local (VND)
        auto local = vnd::run_vnd(cost, perturbed);
        int localGargalo = evaluateGargalo(cost, local);

        if (localGargalo < bestGargalo) {
            best = local;
            bestGargalo = localGargalo;
        }

        current = best;
    }

    return best;
}

} // namespace ils
