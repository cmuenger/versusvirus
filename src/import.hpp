#ifndef IMPORT_HPP
#define IMPORT_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "entities.hpp"

namespace ABM {
    void split(std::string const &str, const char delim,
			std::vector<std::string> &out)
    {
        // construct a stream from the string
        std::stringstream ss(str);

        std::string s;
        while (std::getline(ss, s, delim)) {
            out.push_back(s);
        }
    }

std::vector<std::vector<std::string> > import_csv(const std::string filename, bool includeHeader)
{
	std::ifstream file(filename);
 
	std::vector<std::vector<std::string> > dataList;
 
	std::string line = "";

    //Ignore first line (header)
    if(!includeHeader)
    {
        getline(file, line, ';');
    }
	// Iterate through each line and split the content using delimeter
	while (getline(file, line, ';'))
	{
		std::vector<std::string> vec;
		split(line,',',vec);
		dataList.push_back(vec);
	}
	// Close the File
	file.close();
 
	return dataList;
}

std::vector<Municipality> importMunicipalies()
{
    std::vector<Municipality> municipality;
    std::vector<std::vector<std::string> > dataList = import_csv("CommutePairs.csv", false);

    for(std::vector<std::string> vec : dataList)
	{
        if(vec.size() <=5)
        {
            std::cerr<<"Municipality data to small!"<<std::endl;
            exit(-1);
        }
        Municipality town;
        
        town.BfsId = std::stoi(vec[0]);
        town.Name  = vec[1];
        town.Plz  = std::stoi(vec[2]);
        town.Coordinates = std::make_pair(std::stod(vec[3]),std::stod(vec[4]));

        municipality.push_back(town);
    }
    
    return municipality;
}

std::vector<HelpPopulation> importHelpPopulation()
{
    std::vector<HelpPopulation> pop;
    std::vector<std::vector<std::string> > dataList = import_csv("Population.csv", false);

    for(std::vector<std::string> vec : dataList)
	{
        if(vec.size() <=12)
        {
            std::cerr<<"Pop data to small!"<<std::endl;
            exit(-1);
        }
        HelpPopulation town;
        
        town.BfsId = std::stoi(vec[0]);
        town.NPeople  = std::stoi(vec[1]);
        town.pYoung = std::stod(vec[2]);
        town.pMiddle = std::stod(vec[3]);
        town.pOld = std::stod(vec[4]);
        town.NHouseholds = std::stoi(vec[5]);
        town.NWorker = std::stoi(vec[6]);
        town.NWorkplaces = std::stoi(vec[7]);
       
        pop.push_back(town);
    }

    return pop;
}

std::vector<Commuter> importCommuter()
{
    std::vector<Commuter> Commuters;
    std::vector<std::vector<std::string> > dataList = import_csv("CommutePairs.csv", false);

    for(std::vector<std::string> vec : dataList)
	{
        if(vec.size() <=4)
        {
            std::cerr<<"Commuter data to small!"<<std::endl;
            exit(-1);
        }
        Commuter c;
        for(std::string data : vec)
		{
			c.BfsId = std::stoi(vec[0]);
            c.WorkId  = std::stoi(vec[1]);
            c.CantonId = std::stod(vec[2]);
            c.NCommuters = std::stod(vec[3]);
		}

        Commuters.push_back(c);
    }

    return Commuters;
}

}


#endif