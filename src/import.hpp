#ifndef IMPORT_HPP
#define IMPORT_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <random>

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

std::vector<HelpCoordinates> importMunicipalies()
{
    std::vector<HelpCoordinates> municipality;
    std::vector<std::vector<std::string> > dataList = import_csv("../data/Municipalies.csv", false);

    for(std::vector<std::string> vec : dataList)
	{
        if(vec.size() < 5)
        {
            std::cerr<<"Municipality data to small!"<<std::endl;
            exit(-1);
        }
        HelpCoordinates town;
        
        town.BfsId = std::stoi(vec[0]);
        town.Name = vec[1];
        town.Coordinates = std::make_pair(std::stod(vec[3]),std::stod(vec[4]));

        municipality.push_back(town);
    }
    
    return municipality;
}

std::tuple<std::vector<HelpPopulation>,std::vector<index_t>,std::map<index_t,index_t>> importHelpPopulation()
{
    std::vector<HelpPopulation> pop;
    std::vector<index_t> id;
    std::map<index_t,index_t> map;
    index_t i =0;
    std::vector<std::vector<std::string> > dataList = import_csv("../data/PopulationData2014.csv", false);

    for(std::vector<std::string> vec : dataList)
	{
        if(vec.size() <10)
        {
            for(std::string s : vec)
            {
                std::cout<<s<<" "<<std::endl;
            }
            std::cerr<<"Pop data to small!"<<std::endl;
            exit(-1);
        }
        HelpPopulation town;
        
        town.BfsId = std::stoi(vec[0]);
        town.KId  =  std::stoi(vec[1]);
        town.NPeople  = std::stoi(vec[3]);
        town.pYoung = std::stod(vec[5]);
        town.pMiddle = std::stod(vec[6]);
        town.pOld = std::stod(vec[7]);
        //town.NHouseholds = std::stoi(vec[6]);
        town.NWorker = std::stoi(vec[8]);
        town.NWorkplaces = std::stoi(vec[9]);
       
        pop.push_back(town);
        
        //Creat mapping between internal id and Bfs-Id
        id.push_back(town.BfsId );
        map.insert(std::make_pair(town.BfsId, i));
        i++;
    }

    return std::make_tuple(pop,id,map);
}

std::vector<Commuter> importCommuter()
{
    std::vector<Commuter> Commuters;
    std::vector<std::vector<std::string> > dataList = import_csv("../data/CommutePairs.csv", false);

    for(std::vector<std::string> vec : dataList)
	{
        if(vec.size() <4)
        {
            std::cerr<<"Commuter data to small!"<<std::endl;
            exit(-1);
        }
        Commuter c;
        for(std::string data : vec)
		{
			c.BfsId = std::stoi(vec[0]);
            c.WorkId  = std::stoi(vec[1]);
            c.CantonId = std::stoi(vec[2]);
            c.NCommuters = std::stoi(vec[3]);
		}

        Commuters.push_back(c);
    }

    return Commuters;
}


std::vector<HelpHousehold> importHouseholds()
{
    std::vector<HelpHousehold> Households;
    std::vector<std::vector<std::string> > dataList = import_csv("../data/Households2014.csv", false);

    for(std::vector<std::string> vec : dataList)
	{
        if(vec.size() <7)
        {
            std::cerr<<"Houshold data to small!"<<std::endl;
            exit(-1);
        }
        HelpHousehold h;
        for(std::string data : vec)
		{
			h.BfsId = std::stoi(vec[0]);
            h.P1  = std::stoi(vec[1]);
            h.P2 = std::stoi(vec[2]);
            h.P3  = std::stoi(vec[3]);
            h.P4  = std::stoi(vec[4]);
            h.P5  = std::stoi(vec[5]);
            h.P6  = std::stoi(vec[6]);

            h.NHouseholds =  h.P1+ h.P2+ h.P3 + h.P4+ h.P5 + h.P6; 
		}

        Households.push_back(h);
    }

    return Households;
}


void computeCommute(std::vector<Commuter> commuters, std::vector<HelpPopulation> pop,
                    std::vector<index_t> id,std::map<index_t,index_t> map)
{
    //sum incomming commuter per municipality
    std::map<index_t,index_t>::iterator it;

    //incoming to municipality
    std::map<index_t,index_t> incoming_commuters_m;
    //incoming to canton (unkown municipality) 
    std::map<index_t,index_t> incoming_commuters_k;
    for(Commuter c : commuters)
    {
        index_t idx = c.WorkId;
        index_t idx_k = c.CantonId;

        if(idx != 7777)
        {
            it = incoming_commuters_m.find(idx);
            if(it == incoming_commuters_m.end())
            {
                incoming_commuters_m.insert(std::make_pair(idx,c.NCommuters));
            }
            else
            {
                it->second += c.NCommuters;
            }
            
        }
        else if(idx_k != 77)
        {
             it = incoming_commuters_k.find(idx_k);
            if(it == incoming_commuters_k.end())
            {
                incoming_commuters_k.insert(std::make_pair(idx_k,c.NCommuters));
            }
            else
            {
                it->second += c.NCommuters;
            }
        }
    }

    std::cout<<incoming_commuters_m.size()<<std::endl;
      std::cout<<incoming_commuters_k.size()<<std::endl;
    int a=0, b=0;
    for(const auto& it : incoming_commuters_m)
    {
        const auto& iter = map.find(it.first);
        if(iter == map.end())
        {
            std::cerr<<"Bfs-Id "<<it.first<<" not found"<<std::endl;
        }
        index_t idx = iter->second;
        if(it.second>pop[idx].NWorker)
        {
            std::cout<<pop[idx].NWorker<<" "<<it.second<<std::endl;
        }
        a += pop[idx].NWorker; b += it.second;
    }

    for(const auto& it : incoming_commuters_k)
    {
       b += it.second;
    }

    // std::cout<<a<<" "<<b<<std::endl;
}

int sample_multimodal(std::vector<int> modes)
{
     std::random_device rd;
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double random = dis(gen);

    double N = 0;
    for(auto i : modes) N += i;
   
    int i=0;
    double sum=modes[i];
    while(random > sum/N)
    {
        i++;
        sum+=modes[i];
    }
    return i;
    
}

}


#endif