#ifndef EXAMPLES_HPP
#define EXAMPLES_HPP

#include "common.h"

void testLesatSquare()
{
    std::vector<float> x = {1, 2, 3, 4, 5};
    std::vector<float> y = {1, 4, 9, 16, 25};

    auto res1 = fitLeastSquareAndPR(x, y, 1);
    std::cout << "res1: \n" << std::get<0>(res1) << std::endl;
    std::cout << "p = " << std::get<1>(res1) << std::endl;
    std::cout << "r2 = " << std::get<2>(res1) << std::endl;
    std::cout << std::endl;

    auto res2 = fitLeastSquareAndPR(x, y, 2);
    std::cout << "res2: \n" << std::get<0>(res2) << std::endl;
    std::cout << "p = " << std::get<1>(res2) << std::endl;
    std::cout << "r2 = " << std::get<2>(res2) << std::endl;
    std::cout << std::endl;

    auto res3 = fitLeastSquareAndPR(x, y, 3);
    std::cout << "res3: \n" << std::get<0>(res3) << std::endl;
    std::cout << "p = " << std::get<1>(res3) << std::endl;
    std::cout << "r2 = " << std::get<2>(res3) << std::endl;
    std::cout << std::endl;
}
#endif // EXAMPLES_HPP
