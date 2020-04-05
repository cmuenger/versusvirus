#include "initializeHouseholds.hpp"


namespace ABM
{

	namespace Rng
	{
		class Device
		{
			public:
			static InnerDevice Gen;
		};

		InnerDevice Device::Gen;
	}

	std::vector<Agent> initPopulation()
	{
		std::vector<HelpHousehold> helpHouseholds = importHouseholds();
		std::tuple< std::vector<HelpPopulation>, std::vector<index_t>,std::map<index_t,index_t>> popImport = importHelpPopulation();
		std::vector<HelpPopulation>helpPopulation = std::get<0>(popImport);
		if (helpHouseholds.size() != helpPopulation.size())
			std::cerr<< "Lists dont match in lenght";
		
		index_t householdIdx = 0;
		std::vector<Agent> population;
		for (auto iter = 0; iter < helpHouseholds.size(); iter++)
		{
			std::vector<size_t> numHouseholdOfSize = {helpHouseholds[iter].P1, helpHouseholds[iter].P2, helpHouseholds[iter].P3, 
																			helpHouseholds[iter].P4, helpHouseholds[iter].P5, helpHouseholds[iter].P6};
			std::vector<Agent> townPop = initHouseholds(householdIdx, helpPopulation[iter],numHouseholdOfSize);
			//Due to the construction of townPop perhaps randmoly scramble to get rid of effects such as ordered by household and ordered by household size
			population.insert(std::end(population), std::begin(townPop), std::end(townPop));
		}
		return population;
	}

	//IDEA Iter in other function through all municipalities, to call this function.
	//Create all agents whose household is in municipality
	std::vector<Agent> initHouseholds(
								index_t baseHausholdIdx, 
								HelpPopulation townPopData, 
								std::vector<size_t> numHouseholdOfSize ) //Numbers of Household with size idx+1 

	{	
		//Local population statistics
		double pYoung = townPopData.pYoung; // 
		double pMiddle = townPopData.pMiddle;//
		double pOld = townPopData.pOld;//
		double pAdult = pOld+pMiddle;
		
		//Local Household statistics 
		//ggf as input
		
		double totHouseholds=0; //Number of Households to consider for initialization
		for (int iterSizes= 0;iterSizes<6; iterSizes++)
		{
			iterSizes.pushBack(numOfHouseholdsOfSize);
			totHouseholds+= numOfHouseholdsOfSize;
		}
		
		//Start with creating members of approriate age
		std::vector<Agent> townPop;
		
		/*---------------------------------------------------------------
		Logic of following: in each iterHousehold loop assign one member per household
		
		1st:  hh w. 6 members, hh w. 5 members, ... hh w. 2 members, hh w. 1 member
		2nd: hh w. 6 members, hh w. 5 members, ... hh w. 2 members
		...
		5th: hh w. 6 members, hh w. 5 members
		6th: hh w. 6 members,
		
		Allocate remaining population uniform at random to housholds 6+
		---------------------------------------------------------------*/
		//Allocate 
		for(index_t iterHouseholdLeader = 0; iterHouseholdLeader < totHouseholds; iterHouseholdLeader++)
		{
			Agent householdLeader;
			//rndVal= rand()%pAdult;
			// if(rndVal <pMiddle)
			// {
			// 	pMiddle--;
			// 	householdLeader.Age=AgeCat::Adult;
			// }
			// else
			// {
			// 	householdLeader.Age=AgeCat::Old;
			// 	pOld--
			// }
			// pAdult--;

			// ------------------------------------------------------------------------
			// call to Rng::Device::Gen.Value() generates x ~uni(0,1)
			// Note: pYoung, pMiddle etc are percentages, not abs. counts
			if(Rng::Device::Gen.Value() < pMiddle)  
			{
				householdLeader.Age = AgeCat::Adult;
			} else
			{
				householdLeader.Age = AgeCat::Old;
			}


			Household tmpHaushold;
			tmpHaushold.Municipality = baseHausholdIdx+iterHousholdLeader;
			householdLeader.Household = tmpHaushold;

			townPop.push_back(householdLeader);//Add Leader to List
			
		}
		totHouseholds = totHouseholds - householdOfSize[0];
		double pTot = pYoung + pMiddle +pOld; // <--- equals 1
		//allocating Other members of the House
		for (int iterSize= 1;iterSize<6; iterSize++)
		{
			for(index_t iterHouseholdMember = 0; iterHouseholdMember < totHouseholds; iterHouseholdMember++)
			{
				Agent householdMember;
				//rndVal= rand()%pTot;
				
				//if(rndVal <pYoung)
				//--------------------------------------------------
				double randomValue = Rng::Device::Gen.Value();
				if( randomValue < pYoung)
				{
					pYoung--;
					householdLeader.Age = AgeCat::Minor;
				}
				//else if(rndVal <pMiddle)
				else if(randomValue < pMiddle)
				{
					pMiddle--;
					householdLeader.Age = AgeCat::Adult;
				}
				else
				{
					householdLeader.Age = AgeCat::Old;
					pOld--;
				}
				pTot--;
				householdMember.Household = townPop[iterHouseholdMember].Household; //Correct Syntax??
				townPop.push_back(householdMember);//Add Member to List
			}
			totHouseholds = totHouseholds - householdOfSize[iterSize];
		}
		//Allocate remaining population to households of size 6+

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, householdSize[5]);
		for( int iterPop=0; iterPop<pTot; iterPop++)
		{
			//rndVal = rand() % householdOfSize[5];
			int rndIdx = dis(gen);
			Agent householdMember;
			
			if (pYoung>0)
			{
				householdMember.Age = AgeCat::Minor;
				pYoung--;
			}
			else if(pMiddle>0) 
			{
				householdMember.Age = AgeCat::Adult;
				pMiddle--;
			}
			else
			{
				householdMember.Age = AgeCat::Old;
			}
			householdMember.Household=townPop[rndIdx].Household;
		}	

		return townPop;
	}

}

