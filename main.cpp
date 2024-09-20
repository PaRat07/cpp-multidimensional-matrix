// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <iostream>
#include <cassert>

#include "mdmatrix.h"


int main() {
    MDMatrix<int, 3, 3, 3> matrix;
    assert(matrix.begin() == matrix.begin());
    matrix *= 52;
    matrix -= matrix;
    return 0;
}
