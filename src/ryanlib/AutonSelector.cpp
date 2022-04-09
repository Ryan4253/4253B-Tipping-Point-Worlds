#include "AutonSelector.hpp"

namespace ryan{

AutonSelector::AutonSelector(){
    currentAuton = 0;
    candidate = 0;
    name.push_back("Default");
    auton["Default"] = [](){};
}

void AutonSelector::addRoute(std::function<void()> iRoute, std::string iName){
    auton[iName] = iRoute;
    this->name.push_back(iName);
}

void AutonSelector::incrementAuton(){
    candidate = (candidate + 1) % name.size();
}

void AutonSelector::decrementAuton(){
    candidate = (candidate - 1 + name.size()) % name.size();
}

void AutonSelector::selectAuton(){
    currentAuton = candidate;

    writeToSD();
}

void AutonSelector::selectAuton(int index){
    currentAuton = index;
    candidate = index;

    writeToSD();
}

void AutonSelector::selectAuton(std::string iName){
    for(int i = 0; i < name.size(); i++){
        if(name[i] == iName){
            currentAuton = i;
            candidate = i;
            break;
        }
    }

    writeToSD();
}

std::string AutonSelector::getSelectedAuton(){
    std::optional<std::string> selectedAuton = readFromSD();
    if(selectedAuton == std::nullopt){
        return name[currentAuton];
    }
    return selectedAuton.value();
}

std::string AutonSelector::getCandidateAuton(){
    return name[candidate];
}

int AutonSelector::getAutonCount(){
    return name.size();
}

std::string AutonSelector::getAutonName(int index){
    return name[index];
}

void AutonSelector::execute(){
    std::optional<std::string> routeName = readFromSD();
    if(readFromSD() == std::nullopt){
        return;
    }
    if(auton.find(routeName.value()) == auton.end()){
        return;
    }

    auton[routeName.value()]();
}

void AutonSelector::writeToSD(){
    std::ofstream file{"/usd/auton.txt", std::ios::out | std::ios::trunc}; // file opened here
    file << name[currentAuton];
}

std::optional<std::string> AutonSelector::readFromSD(){
    std::ifstream file{"/usd/auton.txt", std::ios::in}; // file opened here
    if(!file.is_open()){
        return std::nullopt;
    }
    std::string ret; file >> ret;
    return ret;
}

}