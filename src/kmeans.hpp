#ifndef KMENAS_HPP
#define KMENAS_HPP

#include "common.h"



void testCluster()
{
    std::vector<std::vector<float>> points = {
        {1, 2},
        {2, 1},
        {3, 1},
        {5, 4},
        {5, 5},
        {6, 5},
        {10, 8},
        {7, 9},
        {11, 5},
        {14, 9},
        {14, 14},
        {15, 11}
    };

    auto res = clusterKMeans(points, 3);
    std::cout << "centers: \n" << std::get<0>(res) << std::endl;
    std::cout << "labels: \n";
    for (auto &label : std::get<1>(res))
    {
        std::cout << label << " ";
    }
    std::cout << std::endl;
}

#endif // KMENAS_HPP
