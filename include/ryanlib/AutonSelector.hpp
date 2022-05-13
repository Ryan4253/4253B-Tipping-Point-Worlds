#pragma once
#include<functional>
#include<map>
#include<optional>
#include<fstream>

namespace ryan{

/**
 * @brief Singleton is a design pattern that allows only one instance of a class to be created.
 *  This is useful for classes that need to be initialized once and only once. To use this, 
 *  Simply inherit from the Singleton class and make the constructor protected. Then, simply
 *  call the getInstance() function to interface with the class.
 * 
 * @tparam T The type of the class that is being made a singleton.
 */
template <typename T>
class Singleton{
    public:
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    /**
     * @brief Get the Instance of the singleton
     * 
     * @return T& the reference of the instance
     */
    static T& getInstance(){
        if(instance == nullptr){
            instance = new T_Instance();
        }
        return *instance;
    }

    protected:
    Singleton(){}

    private:
    struct T_Instance : public T{
        T_Instance() : T() {}
    };

    static inline T* instance = nullptr;
};

/**
 * @brief Custom Auton Selector that uses the SD Card to operate. The desired autonomous
 *   is written into the SD Card. Then, the SD Card is read during competition and what
 *   autonomous to run is determined. 
 * 
 *   Generally, the increment, decrement and select functions will be bounded to controller
 *   buttons. The routine can then be set before a match by going into the code and saving the route.
 * 
 *   Note that this class is a singleton: you can access it using the AutonSelector::getInstance() method
 *   Also, the zeroth index is always a "Do Nothing" routine
 * 
 */

class AutonSelector : public Singleton<AutonSelector>{
    public:
    /**
     * @brief Adds an autonomous to the selector
     * 
     * @param iRoute the autonomous routine function
     * @param iName the name of the autnomous
     */
    void addRoute(std::function<void()> iRoute, std::string iName);

    /**
     * @brief Increments the candidate choice by 1. If it goes over the number of autonomous, it will go back to zero
     * 
     */
    void incrementAuton();

    /**
     * @brief Decrements the candidate choice by 1. If it reaches zero, it will go to the top.
     * 
     */
    void decrementAuton();

    /**
     * @brief Sets the current candidate choice as the autonomous and write it to the SD Card
     * 
     */
    void selectAuton();

    /**
     * @brief Sets the given index as the autonomous and write it to the SD Card
     * 
     * @param index the given index
     */
    void selectAuton(int index);

    /**
     * @brief Sets the given autonomous name as the autonomous and write it to the SD Card
     * 
     * @param iName the given name
     */
    void selectAuton(std::string iName);

    /**
     * @brief Get the name of the current selected autonomous routine
     * 
     * @return std::string the name of the current selected autonomous routine
     */
    std::string getSelectedAuton();

    /**
     * @brief Get the name of the current candidate autonomous routine
     * 
     * @return std::string the name of the current candidate autonomous routine
     */
    std::string getCandidateAuton();

    /**
     * @brief Gets the number of autonomous
     * 
     * @return int the number of autonomous
     */
    int getAutonCount();

    /**
     * @brief Gets name of the autonomous given an index
     * 
     * @return int the index to check
     */
    std::string getAutonName(int index);

    /**
     * @brief Executes the autonomous written into the SD Card
     * 
     */
    void execute();

    protected:

    /**
     * @brief Construct a new Auton Selector object
     * 
     */
    AutonSelector();

    /**
     * @brief Writes the current selected autonomous to the SD Card
     * 
     */
    void writeToSD();

    /**
     * @brief Reads the selected autonomous from the SD Card
     * 
     * @return std::optional<std::string> the name of the auton, std::nullopt if not found
     */
    std::optional<std::string> readFromSD();

    private:
    int currentAuton = 0;
    int candidate = 0;
    std::vector<std::string> name;
    std::map<std::string, std::function<void()>> auton;
};

}
