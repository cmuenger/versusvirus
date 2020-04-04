#include "util.hpp"


#include <math.h>

namespace ABM
{
    

    SparseMatrix::SparseMatrix(std::vector<std::tuple<size_t, size_t, double>>& tripel) 
        : Values(std::vector<double>()), ColumnIdx(std::vector<size_t>()), RowIdx(std::vector<size_t>())
        {
            std::sort(tripel.begin(), tripel.end(), [](std::tuple<size_t, size_t, double> a, std::tuple<size_t, size_t, double> b) { 
                if (std::get<0>(a) < std::get<0>(b))
                {
                    return true;
                } else if (std::get<0>(a) == std::get<0>(b))
                {
                    return std::get<1>(a) < std::get<1>(b);
                }
                return false;
            });

            Values.reserve(tripel.size());
            RowIdx.push_back(0);
            size_t currentRow = 0;
            for(auto tpl : tripel)
            {
                while(std::get<0>(tpl) > currentRow)
                {
                    RowIdx.push_back( Values.size());
                    currentRow++;
                }

                Values.push_back( std::get<2>(tpl));
                ColumnIdx.push_back( std::get<1>(tpl));
            }
            RowIdx.push_back( Values.size());
        }

    double SparseMatrix::At(const size_t row, const size_t col) const
    {    
        // todo: implement nice binary search
        for(size_t start = RowIdx[row]; start < RowIdx[row + 1]; start++)
        {
            if( ColumnIdx[start] == col)
            {
                return Values[start];
            }
        }
        return 0.;
    }

    double Dist(const std::pair<double,double> locA, const std::pair<double,double> locB, const double param_a, const double param_b)
    {
        double dist = std::sqrt( std::pow( locA.first - locB.first, 2) + std::pow(locA.second - locB.second, 2) );
        return 1./(1. + std::pow( dist/param_a, param_b) );
    }
}