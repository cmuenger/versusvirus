#include "postprocess.hpp"
#include <iostream>
#include <filesystem>
#include <png++/image.hpp>
#include <png++/rgba_pixel.hpp>

namespace ABM
{

	// Input: number of different type of people on pixel
	// Output: rgb values for the pixel
	std::tuple<int,int,int> get_color(float susceptible, float exposed, float infected, float recovered){
		float total = susceptible + exposed + infected + recovered;
		// susectible is green, exposed is yellow (means it is added to red and green in equal parts)
		// infected is red and recovered is blue
		int red = std::round(255*((susceptible + 0.5*exposed)/total));
		int green = std::round(255*((exposed + 0.5*exposed)/total));
		int blue = std::round(255*(recovered/total));
		std::tuple<int,int,int> rgb = std::make_tuple(red,green,blue);
		return rgb;
	}

	// Input: int array[hight][width][4] with data for each geographical location (hight, width)
	// that gives the Susceptible, Exposed, Infected and Recovered on the 4 channels
	// Side effect: genereate heat map png from the given map.
	void create_heatmap(int*** infection_map, int date, std::string name){

		std::cout << "starting write of " << name << " date " << date << std::endl;

		// ensure the directory existst
		std::string dir = "day" + std::to_string(date);
		std::string fname = dir + name;

		if(!std::filesystem::exists(dir)){
			std::filesystem::create_directory(dir);
		}

		// create array in the right size of the picutre
		int i_map_x = 0;
		int i_map_y = 0;
		int x_step = MAP_WIDTH/PIC_WIDTH;
		int y_step = MAP_HEIGHT/PIC_HEIGHT;
		int heat_map1[PIC_WIDTH][PIC_HEIGHT][4] = {};
		int heat_map2[PIC_WIDTH][PIC_HEIGHT][4] = {};
		for(int column = 0; column < PIC_WIDTH; column++){
			for (int row = 0; row < PIC_HEIGHT; row++){
				// copy data over into correct size
				for(; i_map_x < x_step*(column+1) && i_map_x < MAP_WIDTH; i_map_x++){
					for(; i_map_y < y_step*(row+1) && i_map_y < MAP_HEIGHT; i_map_y++){
						heat_map1[column][row][0] += infection_map[i_map_x][i_map_y][0];
						heat_map1[column][row][1] += infection_map[i_map_x][i_map_y][1];
						heat_map1[column][row][2] += infection_map[i_map_x][i_map_y][2];
						heat_map1[column][row][3] += infection_map[i_map_x][i_map_y][3];
					}
				}
			}
		}

		// low pass over picture to distrubute color
		for(int column = LOW_PASS_X; column < PIC_WIDTH - LOW_PASS_X-1; column++){
			for(int row = LOW_PASS_Y; row < PIC_HEIGHT - LOW_PASS_Y-1; row++){
				float sus = 0;
				float exp = 0;
				float inf = 0;
				float rec = 0;
				for(int i = -LOW_PASS_X; i <= LOW_PASS_X; i++){
					for(int j = -LOW_PASS_Y; j <= LOW_PASS_Y; j++){
						sus += heat_map1[column+i][row+j][0];
						exp += heat_map1[column+i][row+j][1];
						inf += heat_map1[column+i][row+j][2];
						rec += heat_map1[column+i][row+j][3];
					}
				}
				heat_map2[column][row][0] = std::round(sus/((2*LOW_PASS_X+1)*(2*LOW_PASS_Y+1)));
				heat_map2[column][row][1] = std::round(exp/((2*LOW_PASS_X+1)*(2*LOW_PASS_Y+1)));
				heat_map2[column][row][2] = std::round(inf/((2*LOW_PASS_X+1)*(2*LOW_PASS_Y+1)));
				heat_map2[column][row][3] = std::round(rec/((2*LOW_PASS_X+1)*(2*LOW_PASS_Y+1)));
			}
		}

		png::image heat_map_png = png::image<png::rgba_pixel>(PIC_WIDTH,PIC_HEIGHT);

		// get colors for all channels and set alpha to some value
		for(int column = 0; column < PIC_WIDTH; column++){
			for(int row = 0; row < PIC_HEIGHT; row++){
				auto [red, green, blue] = 
					get_color(
						(float) heat_map2[column][row][0],
						(float) heat_map2[column][row][1],
						(float) heat_map2[column][row][2],
						(float) heat_map2[column][row][3]);
				png::rgba_pixel pixel = png::rgba_pixel(red,green,blue,ALPHA);
				heat_map_png.set_pixel(column,row,pixel);
			}
		}

		heat_map_png.write(fname);

		return;
	}

	// Input: population, municipality vector indexed by BfsIds and int for date
	// Side effect: creates a folder with date as name and containing all heat maps,
	// and possibly a jason with further data to be desplayed on the website.
    void vizualize(Population& population, int date){

    	std::cout << "starting visualization of day " << date << std::endl;

    	// create output map to store values tripple * for 3 dimenstional array
    	int*** household_map= (int***) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int*** workplace_map= (int***) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int*** young_map 	= (int***) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int*** middle_map 	= (int***) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int*** old_map 		= (int***) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));

    	// some shortcuts to make it more readable
    	const std::vector<Household>& households = population.Households;
    	const std::vector<Workplace>& workplaces = population.Workplaces;
    	const std::vector<Municipality>& municipalities = population.Municipalities;
    	const std::vector<Agent>& agents = population.Agents;

    	// fill all maps with the number of people
    	// go through all households and fill de general household map as well as all edges
    	for(Agent agent_iter : agents){
    		// get workplace and Household coordinates
    		index_t house_m = households[agent_iter.Household].Municipality;
    		index_t workp_m = workplaces[agent_iter.Workplace].Municipality;

    		// get coordinates for map to access
			std::pair<double,double> house_coord = municipalities[house_m].Coordinates;
			std::pair<double,double> workp_coord = municipalities[workp_m].Coordinates;

			// get to array indexes
			int house_x_coord = std::round(house_coord.first - LL_X);
			int house_y_coord = std::round(house_coord.second - LL_Y);
			int workp_x_coord = std::round(workp_coord.first - LL_X);
			int workp_y_coord = std::round(workp_coord.second - LL_Y);

			// find health
			int health;
			switch(agent_iter.Health){
				case Susceptible 	: health = 0; break;
				case Exposed 		: health = 1; break;
				case Infected 		: health = 2; break;
				case Recovered 		: health = 3; break;
			}

			// increment all maps
			household_map[house_x_coord][house_y_coord][health] += 1;
			workplace_map[workp_x_coord][workp_y_coord][health] += 1;
			switch(agent_iter.Age){
				case Minor 	: young_map[house_x_coord][house_y_coord][health] += 1;
				case Adult 	: middle_map[house_x_coord][house_y_coord][health] += 1;
				case Old 	: old_map[house_x_coord][house_y_coord][health] += 1;
			}
    	}

    	create_heatmap(household_map, date, "house");
    	create_heatmap(workplace_map, date, "work");
    	create_heatmap(young_map, date, "young");
    	create_heatmap(middle_map, date, "middle");
    	create_heatmap(old_map, date, "old");

    	free(household_map);
    	free(workplace_map);
    	free(young_map);
    	free(middle_map);
    	free(old_map);

    }
}