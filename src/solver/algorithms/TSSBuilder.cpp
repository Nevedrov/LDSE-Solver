#include "solver\solver.hpp"
#include "solver\utility.hpp"
#include "solver\algorithms\TSSBuilder.hpp"

#include <iostream>
#include <limits>

basis_vectors_t get_basis_vectors(size_t n) {
  basis_vectors_t basis(n);
  for (size_t i = 0u; i < n; ++i) {
    auto basis_vector = std::vector<int>(n, 0);
    basis_vector[i] = 1;
    basis[i] = basis_vector;
  }
  return basis;
}

void split_basis_vectors(size_t row, SLDEProblem& problem, basis_vectors_t& basis, basis_vectors_t& m_zero,
  basis_vectors_t& m_minus, basis_vectors_t& m_plus) {
  for (auto& basis_vector : basis) {
    int scalar_product = SLDEProblem::calculate_equation_func(row,basis_vector,problem);
    if (scalar_product == 0) {
      m_zero.push_back(basis_vector);
    }
    else if (scalar_product < 0) {
      m_minus.push_back(basis_vector);
    }
    else {
      m_plus.push_back(basis_vector);
    }
  }
}

void calculate_next_basis(size_t row, SLDEProblem& problem, basis_vectors_t& basis, basis_vectors_t& m_zero,
  basis_vectors_t& m_minus, basis_vectors_t& m_plus) {
  basis.clear();

  for (auto& basis_vector : m_zero) {
    basis.push_back(basis_vector);
  }

  for (auto neg_vector : m_minus) {
    for (auto pos_vector : m_plus) {
      auto neg = -SLDEProblem::calculate_equation_func(row,neg_vector,problem);
      auto pos = SLDEProblem::calculate_equation_func(row,pos_vector,problem);
      multiply(pos_vector,neg);
      multiply(neg_vector,pos);
      auto a = sum(neg_vector,pos_vector);
      basis.push_back(sum(neg_vector,pos_vector));
    }
  }

  m_zero.clear();
  m_minus.clear();
  m_plus.clear();
}

void clear(basis_vectors_t& basis, size_t cols) {
  int max_elem = std::numeric_limits<int>::min();
  for (auto& basis_vector: basis) {
    for (auto& elem : basis_vector) {
      if (elem > max_elem) {
        max_elem = elem;
      }
    }
  }

  size_t i(0u);
  while (true) {
    auto& x = basis[i];
    size_t j(i + 1u);
    while (true) {
      if (j >= basis.size()) {
        break;
      }
      auto& row = basis[j];
      bool clear_flag = true;
      for (size_t k = 0u; k < cols; ++k) {
        if (row[k] >= max_elem * x[k]) {
          clear_flag = false;
          break;
        }
      }
      if (clear_flag) {
        basis.erase(basis.begin() + i);
      }
      j++;
    }
    if (i >= basis.size()) {
      return;
    }
    i++;
  }

}

basis_vectors_t TSSBuilder::Solve(SLDEProblem& problem) {
  auto rows = problem.rows();
  auto cols = problem.cols();
  auto basis = get_basis_vectors(cols);
  std::vector<std::vector<int>> m_zero;
  std::vector<std::vector<int>> m_minus;
  std::vector<std::vector<int>> m_plus;
  m_zero.reserve(cols * rows);
  m_minus.reserve(cols * rows);
  m_plus.reserve(cols * rows);

  for (size_t i = 0; i < rows; i++) {
    split_basis_vectors(i, problem, basis, m_zero, m_minus, m_plus);
    calculate_next_basis(i, problem, basis, m_zero, m_minus, m_plus);
  }
  clear(basis, cols);
  return basis;
}