#include "solver\algorithms\SOL-01.hpp"
#include "solver\utility.hpp"

#include <iostream>

void add(std::vector<std::vector<bool>>& basis, const std::vector<bool>& s) {
    for (size_t i = 0; i < basis.size(); ++i) {
        auto basis_vector = basis[i];
        bool min_flag = true;
        bool max_flag = true;
        for (size_t j = 0; j < basis_vector.size(); ++j) {
            if (s[j] >= basis_vector[j]) {
                min_flag = false;
            }
            if (s[j] < basis_vector[j]) {
                max_flag = false;
                if (!min_flag) {
                    break;
                }
            }
        }
        if (min_flag) {
            basis.erase(basis.begin() + i);
            basis.push_back(s);
            return;
        }
        else if (max_flag) {
            return;
        }
    }
    basis.push_back(s);
}

bool is_zero_row(const std::vector<int>& row) {
    for (const auto& elem: row) {
        if (elem != 0) {
            return false;
        }
    }
    return true;
}

int scalar_product(const std::vector<int>& x, const std::vector<int>& b_j) {
    int sum = 0;
    for (size_t i = 0; i < x.size(); i++) {
        sum += x[i] * b_j[i];
    }
    return sum;
}

void SOL(std::vector<std::vector<bool>>& basis, const SLDEProblem& problem, std::vector<int>& x, std::vector<bool>& s, size_t i) {
    for (size_t j = i; j < problem.rows(); j++) {
        if (s[j] == 0) {
            auto b_j = problem.get_row_data(j);
            if (scalar_product(x, b_j) < 0) {
                s[j] = true;
                x = sum(x, b_j);
                if (is_zero_row(x)) {
                    add(basis, s);
                    substract(x, b_j);
                    s[j] = false;
                }
                else {
                    SOL(basis, problem, x, s, i);
                    s[j] = false;
                    substract(x, b_j);
                }
            }
        }
    }
}

std::vector<std::vector<bool>> SOL_01Solver::Solve(SLDEProblem &problem) {
    problem.transpose();
    size_t cols = problem.cols();
    size_t rows = problem.rows();
    std::vector<std::vector<bool>> basis;
    for (size_t i = 0; i < rows-1; ++i) {
        auto x = problem.get_row_data(i);
        auto s = std::vector<bool>(rows,false);
        s[i] = true;
        if (is_zero_row(x)) {
            add(basis, s);
        }
        SOL(basis,problem,x,s,i+1);
    }

    return basis;
}
