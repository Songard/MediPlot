#include "common.h"
std::tuple<float, float> getAvgVar(const std::vector<float> &inX)
{
    if (inX.empty())
    {
        throw std::invalid_argument("inX.empty()");
    }

    Eigen::Map<const Eigen::VectorXf> x(inX.data(), inX.size());
    float avg = x.mean();
    auto diff = x.array() - avg;
    float var = diff.square().sum() / (inX.size() - 1);
    return { avg, var };
}

Eigen::MatrixXf getCovariance(const std::vector<std::vector<float>> &inMat)
{
    if (inMat.empty())
    {
        throw std::invalid_argument("inMat.empty()");
    }

    size_t row = inMat.size();
    size_t col = inMat[0].size();

    Eigen::MatrixXf mat(row, col);
    for (int i = 0; i < row; i++)
    {
        if (inMat[i].size() != col)
        {
            throw std::invalid_argument("inMat[i].size() != col");
        }
        for (int j = 0; j < col; j++)
        {
            mat(i, j) = inMat[i][j];
        }
    }
    mat.transposeInPlace();

    Eigen::MatrixXf centered = mat.rowwise() - mat.colwise().mean();
    Eigen::MatrixXf cov = (centered.adjoint() * centered) / double(mat.rows() - 1);

    return cov;
}

Eigen::MatrixXf getPearsonCorr(const Eigen::MatrixXf &cov, const std::vector<float> &vars)
{
    if (vars.empty())
    {
        throw std::invalid_argument("vars.empty()");
    }

    size_t row = cov.rows();
    size_t col = cov.cols();

    if (row != col)
    {
        throw std::invalid_argument("row != col");
    }

    if (row != vars.size())
    {
        throw std::invalid_argument("row != vars.size()");
    }

    Eigen::MatrixXf relativity(row, col);
    for (int i = 0; i < row; i++)
    {
        relativity(i, i) = 1;
        for (int j = i + 1; j < col; j++)
        {
            relativity(i, j) = cov(i, j) / sqrt(vars[i] * vars[j]);
            relativity(j, i) = relativity(i, j);
        }
    }
    return relativity;
}


std::tuple<Eigen::VectorXf, float, float>
fitLeastSquareAndPR(const std::vector<float> &inX, const std::vector<float> &inY, const int inDegree)
{
    if (inX.size() != inY.size())
    {
        throw std::invalid_argument("inX.size() != inY.size()");
    }

    if (inDegree <= 0)
    {
        throw std::invalid_argument("inDegree <= 0");
    }

    if (inX.empty())
    {
        throw std::invalid_argument("inX.empty()");
    }

    Eigen::MatrixXf A(inX.size(), inDegree + 1);
    Eigen::VectorXf b(inY.size());

    for (int i = 0; i < inX.size(); i++)
    {
        for (int j = 0; j < inDegree + 1; j++)
        {
            A(i, j) = float(pow(inX[i], j));
        }
        b(i) = inY[i];
    }

    Eigen::VectorXf x = A.colPivHouseholderQr().solve(b);

    float pValue = 0;
    for (int i = 0; i < inX.size(); i++)
    {
        float y = 0;
        for (int j = 0; j < inDegree + 1; j++)
        {
            y += x(j) * float(pow(inX[i], j));
        }
        pValue += float(pow(y - inY[i], 2));
    }

    float r2Value = 0;
    float avg = b.mean();
    for (int i = 0; i < inX.size(); i++)
    {
        r2Value += float(pow(b(i) - avg, 2));
    }
    r2Value = 1 - pValue / r2Value;

    return { x, pValue, r2Value };
}

Eigen::MatrixXf PCA(const std::vector<std::vector<float>> &in, const int k)
{
    if (in.empty())
    {
        throw std::invalid_argument("in.empty()");
    }

    if (k <= 0)
    {
        throw std::invalid_argument("k <= 0");
    }

    size_t row = in.size();
    size_t col = in[0].size();

    Eigen::MatrixXf mat(row, col);
    for (int i = 0; i < row; i++)
    {
        if (in[i].size() != col)
        {
            throw std::invalid_argument("in[i].size() != col");
        }
        for (int j = 0; j < col; j++)
        {
            mat(i, j) = in[i][j];
        }
    }

    Eigen::VectorXf avg = mat.colwise().mean();
    Eigen::MatrixXf centered = mat.rowwise() - avg.transpose();

    Eigen::MatrixXf cov = centered.adjoint() * centered;
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eig(cov);
    Eigen::MatrixXf eigenVectors = eig.eigenvectors();

    Eigen::MatrixXf result = centered * eigenVectors.rightCols(k);

    return result;
}
std::tuple<Eigen::MatrixXf, std::vector<int>>
clusterKMeans(const std::vector<std::vector<float>> &in, const int k, const int maxIter)
{
    if (in.empty())
    {
        throw std::invalid_argument("in.empty()");
    }

    if (k <= 0)
    {
        throw std::invalid_argument("k <= 0");
    }

    if (maxIter <= 0)
    {
        throw std::invalid_argument("maxIter <= 0");
    }

    size_t row = in.size();
    size_t col = in[0].size();

    Eigen::MatrixXf mat(row, col);
    for (int i = 0; i < row; i++)
    {
        if (in[i].size() != col)
        {
            throw std::invalid_argument("in[i].size() != col");
        }
        for (int j = 0; j < col; j++)
        {
            mat(i, j) = in[i][j];
        }
    }

    Eigen::MatrixXf centers(k, col);
    std::vector<int> labels(row);
    std::vector<int> counts(k);
    std::vector<int> oldLabels(row);

    // init centers
    for (int i = 0; i < k; i++)
    {
        int idx = rand() % row;
        for (int j = 0; j < col; j++)
        {
            centers(i, j) = mat(idx, j);
        }
    }

    int iter = 0;
    while (iter < maxIter)
    {
        // assign labels
        for (int i = 0; i < row; i++)
        {
            float minDist = std::numeric_limits<float>::max();
            int minIdx = -1;
            for (int j = 0; j < k; j++)
            {
                float dist = (mat.row(i) - centers.row(j)).norm();
                if (dist < minDist)
                {
                    minDist = dist;
                    minIdx = j;
                }
            }
            labels[i] = minIdx;
        }

        // update centers
        centers.setZero();
        counts.assign(k, 0);
        for (int i = 0; i < row; i++)
        {
            centers.row(labels[i]) += mat.row(i);
            counts[labels[i]]++;
        }
        for (int i = 0; i < k; i++)
        {
            if (counts[i] > 0)
            {
                centers.row(i) /= float(counts[i]);
            }
        }
        ++iter;

    }
    return { centers, labels };
}

std::tuple<Eigen::MatrixXf, std::vector<int>>
clusterKMeans(const std::vector<std::vector<float>> &in, const int k)
{
    return clusterKMeans(in, k, 100);
}

void testCovariance()
{
    std::vector<std::vector<float>> mat = {
        {1.2f, 2.3f, 3.4f, 8.8f},
        {4.5f, 5.6f, 6.7f, 7.2f}
    };

    auto cov = getCovariance(mat);
    std::cout << "cov: \n" << cov << std::endl;

    std::vector<float> vars;
    for (auto vec : mat)
    {
        auto avgVar = getAvgVar(vec);
        vars.push_back(std::get<1>(avgVar));
    }
    auto rel = getPearsonCorr(cov, vars);
    std::cout << "pearson corr: \n" << rel << std::endl;
}

