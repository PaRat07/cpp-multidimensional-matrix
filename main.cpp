// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <iostream>

#include "mdmatrix.h"


int main() {
    MDMatrix<int, 3, 3, 3> matrix;
    matrix[1][2][0] = 1;
    matrix *= 52;
    matrix -= matrix;
    std::cout << matrix[1][2][0] << std::endl;
    return 0;
}
