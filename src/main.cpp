#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class InputParser{
    public:
        InputParser (int &argc, char **argv){
            for (int i=1; i < argc; ++i)
                this->tokens.push_back(std::string(argv[i]));
        }
        /// @author iain
        const std::string& getCmdOption(const std::string &option) const{
            std::vector<std::string>::const_iterator itr;
            itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()){
                return *itr;
            }
            static const std::string empty_string("");
            return empty_string;
        }
        /// @author iain
        bool cmdOptionExists(const std::string &option) const{
            return std::find(this->tokens.begin(), this->tokens.end(), option)
                   != this->tokens.end();
        }
    private:
        std::vector <std::string> tokens;
};


enum HumanState{
    HEALTHY,
    INFECTED,
    CURED
};

struct Plague{
public:
    int infectStr;
    int tickForBeCured;
};

struct Edge{
    int fromID, targetID;
};

struct Human {
public:
    int id;
    HumanState state = HumanState::HEALTHY;
    std::vector<Edge> edges;
    std::vector<int> incomingEdges;
    int infectedTick = -1;
};


int main(int argc, char* argv[]) {
    bool simulationRunning = true;
    int totalTick = 1;
    int currentTick = 0;
    int pop = 10000;
    int minEdgeCount = 2;
    int maxEdgeCount = 10;
    int totalInfectedInStart = 1;

    InputParser parser(argc,argv);

    if(parser.cmdOptionExists("-totalTick")) {
        totalTick = std::stoi(parser.getCmdOption("-totalTick"));
    }
    if(parser.cmdOptionExists("-population")) {
        pop = std::stoi(parser.getCmdOption("-population"));
    }
    if(parser.cmdOptionExists("-minEdgeCountPerHuman")) {
        minEdgeCount = std::stoi(parser.getCmdOption("-minEdgeCountPerHuman"));
    }
    if(parser.cmdOptionExists("-maxEdgeCountPerHuman")) {
        maxEdgeCount = std::stoi(parser.getCmdOption("-maxEdgeCountPerHuman"));
    }
    if(parser.cmdOptionExists("-totalInfectedInStart")) {
        totalInfectedInStart = std::stoi(parser.getCmdOption("-totalInfectedInStart"));
    }

    

    std::vector<Human> population = std::vector<Human>(pop);
    for(int i = 0;i < population.size();i++) {
        population[i].id = i;
    }
    for(auto& human : population) {
        int edgeCount = (rand() % (maxEdgeCount-minEdgeCount))+minEdgeCount;
        human.edges.resize(edgeCount);

        for(auto& edge : human.edges) {
            edge.fromID = human.id;
            int targetID = rand() % population.size();
            while(targetID == edge.fromID) {
                targetID = rand() % population.size();
            }
            edge.targetID = targetID;
            population[edge.targetID].incomingEdges.push_back(edge.fromID);
        }
    }

    for(int i = 0;i < totalInfectedInStart;i++) {
        int id = rand() % pop;
        while(population[id].state == HumanState::INFECTED) {
            id = rand() % pop;
        }
        population[id].state = HumanState::INFECTED;
    }

    Plague plague = Plague{80,10};
    if(parser.cmdOptionExists("-plagueInfectSTR")) {
        plague.infectStr = std::stoi(parser.getCmdOption("-plagueInfectSTR"));
    }
    if(parser.cmdOptionExists("-plagueTickForBeCured")) {
        plague.tickForBeCured = std::stoi(parser.getCmdOption("-plagueTickForBeCured"));
    }

    //simulation loop
    while (simulationRunning) {
        for(auto& human : population) {
            if(human.state == HumanState::INFECTED) {
                if(currentTick - human.infectedTick == plague.tickForBeCured) {
                    human.state = HumanState::CURED;
                }
            }
            if(human.state == HumanState::INFECTED) {
                for(auto& edge : human.edges) {
                    if(population[edge.targetID].state == HumanState::CURED || population[edge.targetID].state == HumanState::INFECTED) continue;
                    int infect = rand() % (100-plague.infectStr);
                    if(infect == 0) {
                        population[edge.targetID].state = HumanState::INFECTED;
                        population[edge.targetID].infectedTick = currentTick;
                    }
                }
            }
            if(human.state == HumanState::HEALTHY) {
                for(auto id : human.incomingEdges) {
                    if(population[id].state!=HumanState::INFECTED) continue;
                    int infect = rand() % (100-plague.infectStr);
                    if(infect == 0) {
                        human.state = HumanState::INFECTED;
                        human.infectedTick = currentTick;
                    }
                }
            }
        }

        int totalInfected = 0;
        int totalHealthy = 0;
        int totalCured = 0;
        for(auto& human : population) {
            if(human.state == HumanState::HEALTHY) totalHealthy++;
            if(human.state == HumanState::INFECTED) totalInfected++;
            if(human.state == HumanState::CURED) totalCured++;
        }

        std::cout << "--------" << currentTick << "--------" << std::endl;
        std::cout << "Healthy Count: " << totalHealthy << std::endl;
        std::cout << "Infected Count: " << totalInfected << std::endl;
        std::cout << "Cured Count: " << totalCured << std::endl;
        std::cout << "--------" << "--------" << std::endl;
        currentTick++;
        if(currentTick >= totalTick) simulationRunning = false;
    }
    std::cout << "plague_simulation finished!" << std::endl;
    return 0;
}
