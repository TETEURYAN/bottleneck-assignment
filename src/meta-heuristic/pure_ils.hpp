#ifndef ILS_HPP
#define ILS_HPP

#include <vector>

namespace ils {

    using CostMatrix = std::vector<std::vector<int>>;

    class ILSSolver {
    public:
        std::vector<int> run_ils(
            const CostMatrix& cost,
            const std::vector<int>& initialAssignment,
            int maxIterations
        );
    };

} // namespace ils

#endif