#include "solver\solver.hpp"

#include <iostream>

std::vector<int> SLDEProblem::get_row_data(size_t i) const {
    return {matrix.begin() + i * _cols, matrix.begin() + i * _cols + _cols};
}

int& SLDEProblem::get(size_t i, size_t j) {
    int& element = matrix[i * _cols + j];
    return element;
}

const int& SLDEProblem::get(size_t i, size_t j) const {
    const int& element = matrix[i * _cols + j];
    return element;
}

ProblemRow SLDEProblem::operator[](size_t i) {
    ProblemRow row(&matrix, i, _cols);
    return row;
}

const ProblemRow SLDEProblem::operator[](size_t i) const {
    ProblemRow row(const_cast<std::vector<int>*>(&matrix), i, _cols);
    return row;
}

SLDEProblem SLDEProblem::CreateProblem(size_t rows, size_t cols) {
    SLDEProblem new_problem;
    new_problem.matrix = std::vector<int>(rows*cols, 0);
    new_problem._rows = rows;
    new_problem._cols = cols;
    return new_problem;
}

SLDEProblem SLDEProblem::CreateDiagonalProblem(size_t size) {
    SLDEProblem new_problem = CreateProblem(size, size);
    for (size_t i = 0; i < size; ++i) {
        new_problem[i][i] = 1;
    }
    return new_problem;
}

SLDEProblem SLDEProblem::CreateProblem(std::initializer_list<std::initializer_list<int>> data) {
    SLDEProblem problem = CreateProblem(data.size(), data.begin()->size());
    for (const auto& row: data) {
        for (const auto& elem: row) {
            problem.matrix.push_back(elem);
        }
    }
    return problem;
}

SLDEProblem SLDEProblem::CreateProblem(const basis_vectors_t& data) {
    SLDEProblem problem = CreateProblem(data.size(), data.begin()->size());
    size_t pos(0u);
    for (const auto& row: data) {
        for (const auto& elem: row) {
            problem.matrix[pos] = elem;
            pos++;
        }
    }
    return problem;
}

void SLDEProblem::transpose() {
    auto a = matrix.size();
    std::vector<int> trasposed_matrix;
    trasposed_matrix.reserve(matrix.size());
    for (size_t j = 0; j < _cols; ++j) {
        for (size_t i = 0; i < _rows; ++i) {
            trasposed_matrix.push_back(get(i, j));
        }
    };
    std::swap(matrix, trasposed_matrix);
    std::swap(_rows, _cols);
}

int SLDEProblem::scalar_product(const std::vector<int>& a, const std::vector<int>& b) {
    int sum = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

int SLDEProblem::calculate_equation_func(size_t i, const std::vector<int>& basis_vector,
    SLDEProblem& problem) {
    int sum = 0;
    ProblemRow row = problem[i];
    for (size_t j = 0; j < problem.cols(); ++j) {
        sum += row[j] * basis_vector[j];
    }
    return sum;
}

size_t SLDEProblem::rows() const {
    return _rows;
}

size_t SLDEProblem::cols() const {
    return _cols;
}
