#ifndef UTIL_HPP
#define UTIL_HPP




#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <random>
#include <math.h>


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

        void init(std::vector<int> modes, int seed = 17)
        {
            //gen(seed);
            for(auto i : modes) N += i;
            N =0;
        }

        int sample()
        {
        /* double random = dis(gen);

            int i=0;
            double sum=modes[i];
            while(random > sum/N)
            {
                i++;
                sum+=modes[i];
            }
            return i;*/

            return 0;
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
        const char** _argv;
        std::vector<char> _options;
        std::vector<std::string> _optArgs;
        std::vector<std::string> _helplines;

        int _timeHorizon;
        int _dt;

        virtual void HandleArg(const char opt, const std::string optArg);

        void AddHelpline(std::string option, std::string descr);

        public:

        CommandLineInterface(const int argc, const char** argv);

        void PrintHelp() const;
        virtual void ParseArgs();

        int getTimeHorizon() const;
        int getTimeDelta() const;
    };
}
