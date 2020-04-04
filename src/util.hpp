#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <random>
#include <math.h>

#pragma once



namespace ABM
{
    struct SparseMatrix
    {
        std::vector<double> Values;
        std::vector<size_t> ColumnIdx;
        std::vector<size_t> RowIdx;

        // the tuple is (row idx, col idx, value)
        // the tuple-vector is not required to be sorted.
        SparseMatrix(std::vector<std::tuple<size_t, size_t, double>>& tripel); 

        SparseMatrix() : Values(std::vector<double>()), ColumnIdx(std::vector<size_t>()), RowIdx(std::vector<size_t>()) {}

        // returns the value or 0
        double At(size_t row, size_t col) const;

    };

    // computes f(d_ik) according to eq (3) in add. file
    double Dist(const std::pair<double,double> locA, const std::pair<double,double> locB, const double param_a, const double param_b);

    namespace Rng
    {
        class InnerDevice
        {
            private:
            std::random_device _dev;
            std::mt19937 _rng;
            std::uniform_real_distribution<> _dist;

            public:

            InnerDevice() : _rng(std::mt19937(_dev())) {}

            double Value()
            {
                return _dist(_rng);
            }
        }; 
    }
}