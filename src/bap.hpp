#ifndef BAP_HPP
#define BAP_HPP

#include <vector>
#include <string>
#include "meta-heuristic/ils.hpp"

namespace bap {

    using CostMatrix = std::vector<std::vector<int>>;

    // Resolve o Bottleneck Assignment Problem
    int solveBAP(const CostMatrix& cost, std::vector<int>& assignment);

    // Calcula a margem de robustez λ para cada aresta da atribuição ótima
    std::vector<std::vector<int>> computeLambda(const CostMatrix& cost, const std::vector<int>& assignment, int optimalCost);

    // Imprime a matriz
    void printMatrix(const CostMatrix& matrix);

    std::vector<int> solve_ils(const CostMatrix& cost, int maxIter, int& gargalo, std::vector<std::vector<int>>& lambda);

} // namespace bap

#endif
