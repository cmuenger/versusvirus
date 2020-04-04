#ifndef IMPORT_HPP
#defene IMPORT_HPP

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
        getline(file, line, ';')
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

void importComutter()
{
    std::vector<std::vector<std::string> > dataList = import_csv("CommutePairs.csv");

    for(std::vector<std::string> vec : dataList)
	{
        for(std::string data : vec)
		{
			std::cout<<data << " , ";
		}
    }

}

std::vector<Municipality> importMunicipalies()
{
    std::vector<Municipality> municipality;
    std::vector<std::vector<std::string> > dataList = import_csv("CommutePairs.csv", false);

    for(std::vector<std::string> vec : dataList)
	{
        if(vec.size <=5)
        {
            std::err<<"Municipality data does not match!"<<std::endl;
            exit(-1);
        }
        Municipality town;
        
        town.BfsId = std::stoi(vec[0]);
        town.Name  = vec[1];
        town.Plz   = std::stoi(vec[2]);
        town.Coordinates = make_pair(std::stoi(vec[3]),std::stoi(vec[4]));

        municipality.push_back(town);
    }
    
    return municipality
}




#endif