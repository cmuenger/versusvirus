#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <random>
#include <math.h>

#pragma once

typedef size_t index_t;

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

    class multimodal_distribution
    {
        //std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<> dis;

        std::vector<int> p;
        double N;
        public:
        multimodal_distribution(std::vector<index_t> modes,int seed = 17)   
            : gen(), dis(), N(0)
        {
            for(auto i : modes)
            {
                p.push_back(N);
                N+=modes[i];
            }
            
            for(auto i : p)
            {
                i/=N;
            }
        
        }

        int sample()
        {
            double random = dis(gen);

            int i=0;
            while(random > p[i])
            {
                i++;

            }
            return i;
        }
    };

    class uniform_distribution
    {
        //std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<double> dis;

        std::vector<int> p;
        double N;
        public:
        uniform_distribution()   
            : gen(), dis(0.0,1.0), N(0)
        {        }

        int sample_int( const int a, const int b)
        {
            double random = dis(gen);
            return (int)(random*(b-a)+a);
        }

        double sample()
        {
            return dis(gen);
        }
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

    class CommandLineInterface
    {
        protected:
        const int _argc;
        char** _argv;
        std::vector<char> _options;
        std::vector<std::string> _optArgs;
        std::vector<std::string> _helplines;

        int _timeHorizon;
        int _dt;

        virtual void HandleArg(const char opt, const std::string optArg);

        void AddHelpline(std::string option, std::string descr);

        public:

        CommandLineInterface(const int argc, char** argv);

        void PrintHelp() const;
        virtual void ParseArgs();

        int getTimeHorizon() const;
        int getTimeDelta() const;
    };
}
