#include "solver\algorithms\SOL-01.hpp"
#include "solver\algorithms\TSSBuilder.hpp"

#include <string>
#include <fstream>
#include <iostream>

std::vector<int> split(const std::string& s, const std::string& delimiter=",") {
    std::vector<int> row;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
        auto elem = s.substr(start, end - start);
        row.push_back(std::stoi(elem));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    row.push_back(std::stoi(s.substr(start)));

    return row;
}

basis_vectors_t read_data(const std::string& filename) {
    std::ifstream file(filename);
    basis_vectors_t data;
    std::string line;
    while (std::getline(file, line)) {
        data.push_back(split(line));
    }
    return data;
}

void print_data(const basis_vectors_t& data) {
    for (const auto& row : data) {
        std::cout << "(";
        for (const auto& elem : row) {
            std::cout << elem << ",";
        }
        std::cout << ")" << std::endl;
    }
}

void print_data(const std::vector<std::vector<bool>>& data) {
    for (const auto& row : data) {
        std::cout << "(";
        for (const auto& elem : row) {
            std::cout << elem << ",";
        }
        std::cout << ")" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Solver usage: SDLES <input_file> <method>. List of possible methods: TSS, SOL" << std::endl;
    }
    if (argc != 3) {
        std::cerr << "Wrong number of command line arguments:  2 arguments required, " << argc << " provided instead." << std::endl;
    }
    try {
        std::string filename = argv[1];
        basis_vectors_t data = read_data(filename);
        SLDEProblem problem = SLDEProblem::CreateProblem(data);
        std::string method = argv[2];
        if (method == "TSS") {
            basis_vectors_t solution = TSSBuilder::Solve(problem);
            print_data(solution);
        }
        else if (method == "SOL") {
            std::vector<std::vector<bool>> solution = SOL_01Solver::Solve(problem);
            print_data(solution);
        }
        else {
            std::cerr << "Wrong SLDE solving method name. List of possible methods: TSS, SOL" << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception encountered during execution: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}