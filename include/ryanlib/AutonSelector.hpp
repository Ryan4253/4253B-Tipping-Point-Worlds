#pragma once
#include<functional>
#include<map>
#include<optional>
#include<fstream>

namespace ryan{

template <typename T>
class Singleton{
    public:
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static T& getInstance(){
        if(instance == nullptr){
            instance = new T_Instance();
        }
        return instance;
    }

    protected:
    Singleton(){}

    private:
    struct T_Instance : public T{
        T_Instance() : T() {}
    };

    static inline T* instance = nullptr;
};

class AutonSelector : public Singleton<AutonSelector>{
    public:
    void addRoute(std::function<void()> iRoute, std::string iName);

    void incrementAuton();
    void decrementAuton();

    void selectAuton();
    void selectAuton(int index);
    void selectAuton(std::string iName);

    std::string getSelectedAuton();
    std::string getCandidateAuton();

    int getAutonCount();
    std::string getAutonName(int index);

    void execute();

    protected:
    AutonSelector();
    void writeToSD();
    std::optional<std::string> readFromSD();

    private:
    int currentAuton = 0;
    int candidate = 0;
    std::vector<std::string> name;
    std::map<std::string, std::function<void()>> auton;
};

}
