#include "matrix.hpp"
using namespace t_m;

int main() {
    Matrix<int> m1; 
    std::cout << "Empty matrix: " << m1.get_rows() << "x" << m1.get_cols() << std::endl;
    std::cout << std::endl << std::endl;

    Matrix<int> m2(3, 4); 
    std::cout << "Matrix " << m2.get_rows() << "x" << m2.get_cols() << ", capacity: " << m2.get_capasity() << std::endl;
    std::cout << std::endl << std::endl;

    Matrix<int> m3 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::cout << "Initializer list matrix 3x3:" << std::endl;

    int i = 1;
    for (auto it = m3.begin(); it != m3.end(); ++it, ++i) {
        if(i%3 == 1){
            std::cout << std::endl;
        }
        std::cout << *it << " ";
    }
    std::cout << std::endl << std::endl; 

    Matrix<int> m4(m3); 
    std::cout << "Copied matrix:" << std::endl;
    for (const auto& elem : m4) {
        std::cout << elem << " ";
    }
    std::cout << std::endl << std::endl;

    m3.emplace(m3.begin() + 2, 77);
    std::cout << "After emplacing 77 at position 2:" << std::endl;
    for (const auto& elem : m3) std::cout << elem << " ";
    std::cout << std::endl << std::endl;

    Matrix<int> mat(3, 3);
    mat.insert(mat.begin(), {1, 2, 3});

    Matrix<std::string> strMat(2, 2);
    strMat.insert(strMat.begin(), {"Hello", "World"}); 

    for (size_t i = 0; i < mat.get_rows(); ++i) {
        for (size_t j = 0; j < mat.get_cols(); ++j) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    for (size_t i = 0; i < strMat.get_rows(); ++i) {
        for (size_t j = 0; j < strMat.get_cols(); ++j) {
            std::cout << strMat[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
