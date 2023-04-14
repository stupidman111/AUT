#include "../include/hw1.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>

namespace algebra {

Matrix CreateMatrix(size_t n, size_t m, double v) {
    if (n < 0 || m < 0) {
        throw std::logic_error("wrong col or row\n");
    }
    Matrix x;
    for (int i = 0; i < n; ++i) {
        x.emplace_back(std::vector<double>(m, v));
    }
    return x;
}

Matrix zeros(size_t n, size_t m) {
    return CreateMatrix(n, m, 0.0);
}

Matrix ones(size_t n, size_t m) {
    return CreateMatrix(n, m, 1.0);
}

Matrix random(size_t n, size_t m, double min, double max) {//generate a random num in [min, max]
    if (min > max) {
        throw std::logic_error("min should be less than max\n");
    }
    auto x {zeros(n, m)};
    std::default_random_engine e;
    std::uniform_real_distribution<double> u(min, max);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            x[i][j] = u(e);
        }
    }
    return x;
}

void show(const Matrix& matrix) {
    for (const auto& vec : matrix) {
        for (double val : vec) {
            //std::left--<ios>--Adjust output to the left
            //std::setw(int n)--<iomanip>--Set field width
            //std::setprecision(int n)--<iomanip>--Set decimal precision
            std::cout << std::left << std::setw(5) << std::setprecision(3) << val;
        }
        std::cout << std::endl;
    }
}

Matrix multiply(const Matrix& matrix, double c) {
    if (matrix.size() == 0) { return matrix; }

    //Matrix m(matrix);//直接初始化
    auto col { matrix.size() };
    auto row { matrix.front().size() };
    Matrix m(zeros(col, row));
    for (size_t i = 0; i < col; ++i) {
        for (size_t j = 0; j < row; ++j) {
            m[i][j] = matrix[i][j] * c;
        }
    }
    return m;
}

Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
    if (matrix1.size() == 0 || matrix2.size() == 0) { return Matrix{}; }

    auto col_1 { matrix1.size() }, col_2 { matrix2.size() };
    auto row_1 { matrix1.front().size() }, row_2 { matrix2.front().size() };
    if (row_1 != col_2) {
        throw std::logic_error("matrix1's row must equals to matrix2's col\n");
    }

    Matrix m(zeros(col_1, row_2));
    for (int i = 0; i < col_1; ++i) {
        for (int j = 0; j < row_2; ++j) {
            for (int k = 0; k < col_2; ++k) {
                m[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return m;
}

Matrix sum(const Matrix& matric, double c) {
    if (matric.size() == 0) { return Matrix{}; }

    auto col { matric.size() }, row { matric.front().size() };
    Matrix m(zeros(col, row));
    
    for (int i = 0; i < col; ++i) {
        for (int j = 0; j < row; ++j) {
            m[i][j] = matric[i][j] + c;
        }
    }
    return m;
}

Matrix sum(const Matrix& matrix1, const Matrix& matrix2) {
    if (matrix1.size() == 0 && matrix2.size() == 0) { return Matrix{}; };

    if (matrix1.size() == 0 || matrix2.size() == 0) { 
        throw std::logic_error("calculation error");
    };

    auto col_1 {matrix1.size()}, col_2 {matrix2.size()};
    auto row_1 {matrix1.front().size()}, row_2 {matrix2.front().size()};
    if (col_1 != col_2 || row_1 != row_2) {
        throw std::logic_error("dimession error\n");//维度错误
    }

    Matrix m(zeros(col_1, row_1));

    for (int i = 0; i < col_1; ++i) {
        for (int j = 0; j < row_1; ++j) {
            m[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return m;
}

Matrix transpose(const Matrix& matrix) {
    if (matrix.size() == 0) { return Matrix{}; };

    auto col {matrix.size()}, row {matrix.front().size()};
    Matrix m(zeros(row, col));

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            m[i][j] = matrix[j][i];
        }
    }
    return m;
}


void rotate(Matrix& matrix) {//要求是方阵--先上下交换向量，再按对角交换元素
    int n = matrix.size();
    for (int i = 0, j = n - 1; i < j; ++i, --j) matrix[i].swap(matrix[j]);
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            std::swap(matrix[i][j], matrix[j][i]);
        }
    }

}

Matrix minor(const Matrix& matrix, size_t n, size_t m) {
    if (n >= matrix.size() || m >= matrix[0].size()) {
        throw std::logic_error("out of bound\n");
    }
    Matrix mat;

    for (size_t i = 0; i < matrix.size(); ++i) {
        if (i == n) {
            continue;
        }
        std::vector<double> tmpV;
        for (size_t j = 0; j < matrix[0].size(); ++j) {
            if (j == m) {
                continue;
            }
            tmpV.push_back(matrix[i][j]);
        }
        mat.push_back(tmpV);
    }

    return mat;
}

double determinant(const Matrix& matrix) {
    if (matrix.empty()) {//
        return 1;
    } else if (matrix.size() != matrix[0].size()) {
        throw std::logic_error("matrix is not a square matrix");
    } else if (matrix.size() == 1) {
        return matrix[0][0];
    } else if (matrix.size() == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    } else {
        double res = 0.0;

        for (int i = 0; i < matrix.size(); i++) {
            res += matrix[i][0] * pow(-1, i) * determinant(minor(matrix, i, 0));
        }

        return res;
    }
    return 0;
}

Matrix inverse(const Matrix& matrix) {//求逆矩阵--A* / |A|
    if (matrix.size() == 0) { return Matrix{}; };
    if (matrix.size() != matrix[0].size() || determinant(matrix) == 0) {
        throw std::logic_error("The inverse of matrix doesn't exist\n");
    }

    double res = determinant(matrix);
    
    Matrix m(zeros(matrix.size(), matrix.size()));
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            m[i][j] = pow(-1, i + j) * determinant(minor(matrix, i, j));
        }
    }
    m = transpose(m);//转置
    return multiply(m, 1.0 / res);
}

Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis) {
    

    auto row_1 { matrix1.size() }, row_2 { matrix2.size() };
    auto col_1 { matrix1[0].size() }, col_2 { matrix2[0].size() };
    if (axis == 0) {//上下相加
        if (col_1 != col_2) { throw std::logic_error("can't concatenate\n"); }

        Matrix m(zeros(row_1 + row_2, col_1));
        for (int i = 0; i < row_1 + row_2; ++i) {
            for (int j = 0; j < col_1; ++j) {
                if (i < row_1) {
                    m[i][j] = matrix1[i][j];
                } else {
                    m[i][j] = matrix2[i - row_1][j];//这里不能使用`m[i][j] = matrix2[i % col_2][j]`，因为若col_2 >= col_1的话，后续会出问题
                }
            }
        }
        return m;
    } //左右相加
        if (row_1 != row_2) { throw std::logic_error("can't concatenate\n");}

        Matrix m(zeros(row_1, col_1 + col_2));
        for (int i = 0; i < row_1; ++i) {
            for (int j = 0; j < col_1 + col_2; ++j) {
                if (j < col_1) {
                    m[i][j] = matrix1[i][j];
                } else {
                    m[i][j] = matrix2[i][j - col_1];//这里不能使用`m[i][j] = matrix2[i][j % row_1]`，因为若col_2 >= col_1的话，后续会出问题
                }
            }
        }
        return m;
    
    return Matrix{};
}   

Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2) {//交换第r1行与第r2行
    if (matrix.empty()) { return Matrix{}; }
    if (r1 >= matrix.size() || r2 >= matrix.size()) {
        throw std::logic_error("out of bound");
    }
    Matrix m(matrix);
    m[r1].swap(m[r2]);

    return m;
}

Matrix ero_multiply(const Matrix& matrix, size_t r, double c) {//将第r行乘上数c
    if (matrix.empty()) { return Matrix{}; }
    if (r >= matrix.size()) {
        throw std::logic_error("out of bound");
    }
    Matrix m(matrix);
    for (int i = 0; i < m[0].size(); ++i) {
        m[r][i] *= c;
    }
    return m;
}

Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {//将第r1行乘上数c并加到第r2行上
    if (matrix.size() == 0) { return Matrix{}; }
    if (r1 >= matrix.size() || r2 >= matrix.size()) {
        throw std::logic_error("out of bound");
    }
    Matrix m(matrix);
    for (int i = 0; i < m[0].size(); ++i) {
        m[r2][i] += m[r1][i] * c;
    }
    return m;
}

void mul_sum(Matrix& matrix, size_t r1, double c, size_t r2) {
    for (int i = 0; i < matrix[r2].size(); i++) {
        matrix[r2][i] += (matrix[r1][i] * c);
    }
}

Matrix upper_triangular(const Matrix& matrix) {
    if (matrix.empty()) {
        return Matrix{};
    }
    if (matrix.size() != matrix[0].size()) {
        throw std::logic_error("not a square matrix\n");
    }
    Matrix x = ero_multiply(matrix, 0, 1);
    if (x[0][0] == 0) {
        int row = 1;
        while (x[row][0] == 0 && row < x.size()) {
            row++;
        }
        if (row == x.size()) {
            throw std::logic_error("impossible");
        }
        x = ero_swap(x, 0, row);
    }

    auto m{matrix.size()};

    for (int i = 0; i < m; i++) {
        for (int j = i + 1; j < m; j++) {
            mul_sum(x, i, (-x[j][i] / x[i][i]), j);
        }
    }
    return x;
}

}  // namespace algebra