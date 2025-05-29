#include "vnd.hpp"
#include <algorithm>

namespace vnd {

int evaluateGargalo(const CostMatrix& cost, const std::vector<int>& assignment) {
    int gargalo = 0;
    for (int i = 0; i < (int)cost.size(); ++i)
        gargalo = std::max(gargalo, cost[i][assignment[i]]);
    return gargalo;
}

std::vector<int> run_vnd(
    const CostMatrix& cost,
    const std::vector<int>& initialAssignment
) {
    int n = cost.size();
    std::vector<int> current = initialAssignment;
    int currentGargalo = evaluateGargalo(cost, current);
    bool improvement = true;

    while (improvement) {
        improvement = false;

        // Vizinhança 1: Swap
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                std::swap(current[i], current[j]);
                int newGargalo = evaluateGargalo(cost, current);
                if (newGargalo < currentGargalo) {
                    currentGargalo = newGargalo;
                    improvement = true;
                    break;
                } else {
                    std::swap(current[i], current[j]); // desfaz
                }
            }
            if (improvement) break;
        }

        if (improvement) continue;

        // Vizinhança 2: Reinserção
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                int temp = current[i];
                current.erase(current.begin() + i);
                current.insert(current.begin() + j, temp);
                int newGargalo = evaluateGargalo(cost, current);
                if (newGargalo < currentGargalo) {
                    currentGargalo = newGargalo;
                    improvement = true;
                    break;
                } else {
                    current.erase(current.begin() + j);
                    current.insert(current.begin() + i, temp); // desfaz
                }
            }
            if (improvement) break;
        }
    }

    return current;
}

} // namespace vnd
