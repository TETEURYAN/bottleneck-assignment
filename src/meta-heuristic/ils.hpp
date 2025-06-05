#ifndef ILS_HPP
#define ILS_HPP

#include "../bap.hpp"

using namespace std;

namespace ils {

    using CostMatrix = vector<vector<int>>;

    class ILSSolver {
    public:
        std::vector<int> solve_ils(
            const CostMatrix& cost,
            int maxIter,
            int bestGargalo,
            const std::vector<int>& initialAssignment,
            const std::vector<std::vector<int>>& initialLambda,
            int& finalGargalo,
            std::vector<std::vector<int>>& finalLambda
        );
    };

    std::vector<std::vector<int>> vnd_heuristica(
        const CostMatrix& cost,
        const std::vector<int>& assignment,
        int originalGargalo
    );

    int evaluateGargalo(const CostMatrix& cost, const std::vector<int>& assignment);

    std::vector<int> solveGreedy(const CostMatrix& cost);

} // namespace ils

#endif
