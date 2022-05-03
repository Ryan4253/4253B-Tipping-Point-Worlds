#pragma once
#include "main.h"

enum class TallBranchState {
    IDLE, LOADING, LOADED, UNLOADING, CLAMPED, UNLOADED
};

class TallBranchStateMachine : public StateMachine<TallBranchState> {
    private:
    
    public: 
    TallBranchStateMachine() = default;
    void move();
};