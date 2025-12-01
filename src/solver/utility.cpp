#include "solver\utility.hpp"

void substract(std::vector<int>& x, const std::vector<int>& b_j) {
    for (size_t i = 0; i < x.size(); i++) {
        x[i] -= b_j[i];
    }
}

std::vector<int>& multiply(std::vector<int>& v, int m) {
    for (int & i : v) {
        i *= m;
    }
    return v;
}

std::vector<int> sum(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    for (size_t i = 0u; i < a.size(); i++) {
        result.push_back(a[i] + b[i]);
    }
    return result;
}