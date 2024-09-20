// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <iostream>
#include <cassert>

#include "mdmatrix.h"

consteval MDMatrix<int, 3, 3, 3> GetEmptyMatrix() {
    MDMatrix<int, 3, 3, 3> matrix;
    for (auto &i : matrix) {
        for (auto &j : i) {
            for (auto &l : j) {
                l = 0;
            }
        }
    }
    return matrix;
}

int main() {
    static constinit MDMatrix<int, 3, 3, 3> matrix = GetEmptyMatrix();
    matrix += matrix;
    std::cout << matrix[1][2][0] << std::endl;
    return 0;
}
