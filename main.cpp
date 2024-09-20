// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <iostream>
#include <cassert>

#include "mdmatrix.h"


int main() {
    MDMatrix<int, 3, 3, 3> matrix;
    assert(matrix.begin() == matrix.begin());
    for (auto &i : matrix) {
        for (auto &j : i) {
            for (auto &l : j) {
                l = 2;
            }
        }
    }
    matrix *= 52;
//    matrix -= matrix;
    std::cout << matrix[1][2][0] << std::endl;
    return 0;
}
