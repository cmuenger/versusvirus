
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

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


std::vector<std::vector<std::string> > import_csv(const std::string filename)
{
	std::ifstream file(filename);
 
	std::vector<std::vector<std::string> > dataList;
 
	std::string line = "";
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

int main()
{
   	std::vector<std::vector<std::string> > dataList = import_csv("example.csv");
 
	// Print the content of row by row on screen
	for(std::vector<std::string> vec : dataList)
	{
		for(std::string data : vec)
		{
			std::cout<<data << " , ";
		}
		std::cout<<std::endl;
	}
	return 0; 

}