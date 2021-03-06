#include "postprocess.hpp"
#include <iostream>
#include <filesystem>
#include <png++/image.hpp>
#include <png++/rgba_pixel.hpp>

// for testing
#include <random>

namespace ABM
{

	void create_json(const int* data, std::string name){
		
	}


	// Input: number of different type of people on pixel
	// Output: rgb values for the pixel
	std::tuple<int,int,int> get_color(float susceptible, float exposed, float infected, float recovered){
		float total = susceptible + exposed + infected + recovered;
		// susectible is green, exposed is yellow (means it is added to red and green in equal parts)
		
		// infected is red and recovered is blue
		int red = std::round(255*((infected + 0.5*exposed)/total));
		int green = std::round(128*((susceptible + 0.5*exposed)/total));
		int blue = std::round(128*(recovered/total));
		std::tuple<int,int,int> rgb = std::make_tuple(red,green,blue);
		return rgb;
	}

	// Input: int array[hight][width][4] with data for each geographical location (hight, width)
	// that gives the Susceptible, Exposed, Infected and Recovered on the 4 channels
	// Side effect: genereate heat map png from the given map.
	void create_heatmap(int* infection_map, std::string name){

		std::cout << "starting write of " << name << std::endl;

		// create array in the right size of the picutre
		int x_step = MAP_WIDTH/PIC_WIDTH+1;
		int y_step = MAP_HEIGHT/PIC_HEIGHT+1;
		int* heat_map1 = (int*) calloc(MAP_WIDTH*MAP_HEIGHT*4,sizeof(int));
		int* heat_map2 = (int*) calloc(MAP_WIDTH*MAP_HEIGHT*4,sizeof(int));
		for(int column = 0; column < PIC_WIDTH; column++){
			for (int row = 0; row < PIC_HEIGHT; row++){
				// copy data over into correct size
				unsigned int sus = 0;
				unsigned int exp = 0;
				unsigned int inf = 0;
				unsigned int rec = 0;
				for(int i_map_x = x_step*(column); i_map_x < x_step*(column+1) && i_map_x < MAP_WIDTH; i_map_x++){
					for(int i_map_y = y_step*(row); i_map_y < y_step*(row+1) && i_map_y < MAP_HEIGHT; i_map_y++){
						sus += infection_map[(i_map_x*MAP_HEIGHT +i_map_y)*4 + 0];
						exp += infection_map[(i_map_x*MAP_HEIGHT +i_map_y)*4 + 1];
						inf += infection_map[(i_map_x*MAP_HEIGHT +i_map_y)*4 + 2];
						rec += infection_map[(i_map_x*MAP_HEIGHT +i_map_y)*4 + 3];
					}
				}
				heat_map1[(column*PIC_HEIGHT + row)*4 + 0] = sus;
				heat_map1[(column*PIC_HEIGHT + row)*4 + 1] = exp;
				heat_map1[(column*PIC_HEIGHT + row)*4 + 2] = inf;
				heat_map1[(column*PIC_HEIGHT + row)*4 + 3] = rec;
			}
		}

		// low pass over picture to distrubute color
		for(int column = LOW_PASS_X; column < PIC_WIDTH-LOW_PASS_X; column++){
			for(int row = LOW_PASS_Y; row < PIC_HEIGHT-LOW_PASS_Y; row++){
				float sus = 0;
				float exp = 0;
				float inf = 0;
				float rec = 0;
				for(int i = -LOW_PASS_X; i < LOW_PASS_X; i++){
					for(int j = -LOW_PASS_Y; j < LOW_PASS_Y; j++){
						sus += (float) heat_map1[((column+i)*PIC_HEIGHT + (row+j))*4 + 0];
						exp += (float) heat_map1[((column+i)*PIC_HEIGHT + (row+j))*4 + 1];
						inf += (float) heat_map1[((column+i)*PIC_HEIGHT + (row+j))*4 + 2];
						rec += (float) heat_map1[((column+i)*PIC_HEIGHT + (row+j))*4 + 3];
					}
				}
				heat_map2[(column*PIC_HEIGHT + row)*4 + 0] = std::round(sus/((2*LOW_PASS_X+1)*(2*LOW_PASS_Y+1)));
				heat_map2[(column*PIC_HEIGHT + row)*4 + 1] = std::round(exp/((2*LOW_PASS_X+1)*(2*LOW_PASS_Y+1)));
				heat_map2[(column*PIC_HEIGHT + row)*4 + 2] = std::round(inf/((2*LOW_PASS_X+1)*(2*LOW_PASS_Y+1)));
				heat_map2[(column*PIC_HEIGHT + row)*4 + 3] = std::round(rec/((2*LOW_PASS_X+1)*(2*LOW_PASS_Y+1)));
			}
		}

		png::image heat_map_png = png::image<png::rgba_pixel>(PIC_WIDTH,PIC_HEIGHT);

		// get colors for all channels and set alpha to some value
		for(int column = 0; column < PIC_WIDTH; column++){
			for(int row = 0; row < PIC_HEIGHT; row++){
				auto [red, green, blue] = 
					get_color(
						(float) heat_map2[(column*PIC_HEIGHT + row)*4 + 0],
						(float) heat_map2[(column*PIC_HEIGHT + row)*4 + 1],
						(float) heat_map2[(column*PIC_HEIGHT + row)*4 + 2],
						(float) heat_map2[(column*PIC_HEIGHT + row)*4 + 3]);
				png::rgba_pixel pixel = png::rgba_pixel(red,green,blue,ALPHA);
				heat_map_png.set_pixel(column,PIC_HEIGHT-1-row,pixel);
			}
		}

		heat_map_png.write(name+".png");

		free(heat_map1);
		free(heat_map2);

		return;
	}

	// Input: population, municipality vector indexed by BfsIds and int for date
	// Side effect: creates a folder with date as name and containing all heat maps,
	// and possibly a jason with further data to be desplayed on the website.
    void vizualize(Population& population, int date, std::map<index_t,index_t> map){

    	std::cout << "starting visualization of day " << date << std::endl;

    	// ensure the directory existst
		std::string dir = std::to_string(date);

		if(!std::filesystem::exists(dir)){
			std::filesystem::create_directory(dir);
		}

    	// create output map to store values tripple * for 3 dimenstional array
    	int* household_map	= (int*) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int* workplace_map	= (int*) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int* young_map 		= (int*) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int* middle_map 	= (int*) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int* old_map 		= (int*) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));

    	// data for jason, output
    	int* kantons = (int*) calloc(4*26,sizeof(int)); // TODO USE THIS

    	// some shortcuts to make it more readable
    	const std::vector<Household>& households = population.Households;
    	const std::vector<Workplace>& workplaces = population.Workplaces;
    	const std::vector<Municipality>& municipalities = population.Municipalities;
    	const std::vector<Agent>& agents = population.Agents;

    	// fill all maps with the number of people
    	// go through all households and fill de general household map as well as all edges
	
    	for(Agent agent_iter :  population.Agents){
			
			// find health
			int health;
			switch(agent_iter.Health){
				case Susceptible 	: health = 0; break;
				case Exposed 		: health = 1; break;
				case Infected 		: health = 2; break;
				case Recovered 		: health = 3; break;
			}

    		// get Workplace and Household ids
    		index_t house_m = households[agent_iter.Household].Municipality;
    		index_t workp_m = workplaces[agent_iter.Workplace].Municipality;


    		// get coordinates for map to access

			std::pair<double,double> house_coord(municipalities[map[house_m]].Coordinates);
			int house_x_coord = std::round(house_coord.first - LL_X)/BIN_SIZE;
			int house_y_coord = std::round(house_coord.second - LL_Y)/BIN_SIZE;
			// increment maps
			household_map[4*(house_x_coord*MAP_HEIGHT + house_y_coord) + health] += 1;
			switch(agent_iter.Age){
				case Minor 	: young_map[4*(house_x_coord*MAP_HEIGHT + house_y_coord) + health] += 1; break;
				case Adult 	: middle_map[4*(house_x_coord*MAP_HEIGHT + house_y_coord) + health] += 1; break;
				case Old 	: old_map[4*(house_x_coord*MAP_HEIGHT + house_y_coord) + health] += 1; break;

			if(workp_m != ~0){
				std::pair<double,double> workp_coord(municipalities[map[workp_m]].Coordinates);
				int workp_x_coord = std::round(workp_coord.first - LL_X)/BIN_SIZE;
				int workp_y_coord = std::round(workp_coord.second - LL_Y)/BIN_SIZE;
				workplace_map[4*(workp_x_coord*MAP_HEIGHT + workp_y_coord) + health] += 1;
			}

			// increment canton counters
			index_t kanton_id = municipalities[map[house_m]].KId;
			kantons[kanton_id*4 + health] += 1;

			}
			
    	}

    	create_json(kantons, dir+"/cases");

    	create_heatmap(household_map, dir+"/house");
    	create_heatmap(workplace_map, dir+"/work");
    	create_heatmap(young_map, dir+"/young");
    	create_heatmap(middle_map, dir+"/middle");
    	create_heatmap(old_map, dir+"/old");

    	free(kantons);
    	free(household_map);
    	free(workplace_map);
    	free(young_map);
    	free(middle_map);
    	free(old_map);

    }
}