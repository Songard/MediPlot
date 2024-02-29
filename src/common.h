#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <vector>
#include <cmath>
#include "Eigen/Dense"

std::tuple<float, float> getAvgVar(const std::vector<float> &inX);
Eigen::MatrixXf getCovariance(const std::vector<std::vector<float>> &inMat);
Eigen::MatrixXf getPearsonCorr(const Eigen::MatrixXf &cov, const std::vector<float> &vars);
std::tuple<Eigen::VectorXf, float, float>fitLeastSquareAndPR(const std::vector<float> &inX, const std::vector<float> &inY, const int inDegree);
Eigen::MatrixXf PCA(const std::vector<std::vector<float>> &in, const int k);
std::tuple<Eigen::MatrixXf, std::vector<int>> clusterKMeans(const std::vector<std::vector<float>> &in, const int k, const int maxIter);
std::tuple<Eigen::MatrixXf, std::vector<int>>clusterKMeans(const std::vector<std::vector<float>> &in, const int k);

#endif // COMMON_H
