#ifndef UTIL_HPP
#define UTIL_HPP

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

#endif