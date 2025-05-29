#ifndef VND_HPP
#define VND_HPP

#include <vector>

namespace vnd {

    using CostMatrix = std::vector<std::vector<int>>;

    int evaluateGargalo(const CostMatrix& cost, const std::vector<int>& assignment);

    std::vector<int> run_vnd(
        const CostMatrix& cost,
        const std::vector<int>& initialAssignment
    );

} // namespace vnd

#endif
