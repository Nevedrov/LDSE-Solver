#ifndef SOLVER_H
#define SOLVER_H
#include <vector>

using basis_vectors_t = std::vector<std::vector<int>>;

class ProblemRow {
    std::vector<int>* origin;
    size_t row;
    size_t cols;

    ProblemRow() = default;

public:

    ProblemRow(std::vector<int>* origin, size_t row, size_t cols):
    origin(origin),
    row(row),
    cols(cols)
    {}

    const int& operator[](size_t i) const {
        return origin->at(row * cols + i);
    }

    int& operator[](size_t i) {
        return origin->at(row * cols + i);
    }
};

class SLDEProblem {
    std::vector<int> matrix;
    size_t _rows = 0;
    size_t _cols = 0;

    SLDEProblem() = default;

public:

    static int scalar_product(const std::vector<int>& a, const std::vector<int>& b);

    static SLDEProblem CreateProblem(size_t rows, size_t cols);

    static SLDEProblem CreateDiagonalProblem(size_t size);

    static SLDEProblem CreateProblem(std::initializer_list<std::initializer_list<int>> data);

    static SLDEProblem CreateProblem(const basis_vectors_t& data);

    void transpose();

    static int calculate_equation_func(size_t row,
        const std::vector<int>& basis_vector, SLDEProblem& problem);

    const ProblemRow operator[](size_t row) const;

    ProblemRow operator[](size_t row);

    std::vector<int> get_row_data(size_t i) const;

    const int& get(size_t i, size_t j) const;

    int& get(size_t i, size_t j);

    size_t rows() const;

    size_t cols() const;
};

#endif