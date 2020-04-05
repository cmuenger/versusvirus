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

    CommandLineInterface::CommandLineInterface(const int argc, char** argv)
    : _argc(argc), _argv(argv), _helplines(std::vector<std::string>())
    {
        AddHelpline("-h x", "Print this help");
        AddHelpline("-t x", "Put the time horizon in days (multiple of dt), default = 21");
        AddHelpline("-d x", "time delta in days, default = 1");
        _dt = 1;
        _timeHorizon = 21;
    }

    void CommandLineInterface::HandleArg(const char opt, const std::string optArg)
    {
        switch(opt)
        {
            case 't':
                _timeHorizon = std::stoi(optArg);
                break;
            case 'd':
                _dt = std::stoi(optArg);
            case 'h':
                PrintHelp();
                std::exit(0);
                break;
            default:
                PrintHelp();
                std::exit(-2);
        }
    }

    void CommandLineInterface::ParseArgs()
    {
        for(int i = 0; i < _argc; i++)
        {
            if(_argv[i][0] == '-')
            {
                _options.push_back(_argv[i][1]);
                i++;
                _optArgs.push_back( _argv[i]);
            }
        }

        for(int i = 0; i < _options.size(); i++)
        {
            HandleArg(_options[i], _optArgs[i]);
        }
    }

    void CommandLineInterface::AddHelpline(std::string option, std::string descr) 
    {
        _helplines.push_back("'" + option + "': "+descr);
    }

    void CommandLineInterface::PrintHelp() const
    {
        for(auto line : _helplines)
        {
            std::cout << line << std::endl;
        }
    }

    int CommandLineInterface::getTimeHorizon() const
    {
        return _timeHorizon;
    }

    int CommandLineInterface::getTimeDelta() const
    {
        return _dt;
    }




}