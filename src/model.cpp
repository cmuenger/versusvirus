enum state { S, E, I, R}

class agent
{
    int household;
    int workplace;
    int age;
    state ;

    void update();
}

class workplace
{
    int location;
    std::list<agent*> workers;
}

class household
{
    int location;
    std::list<agent*> house;
}

//Needed household size distribution
void generateHouseholds();

//Needed Age distribution and household structure
void createAgentsInHousholds();

//Workplace size distribution
void generateWorkplaces();

void assignAgentsToWorkplaces();

void computeDistanceBetweenLocations()