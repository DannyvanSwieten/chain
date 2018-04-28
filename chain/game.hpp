//
//  game.hpp
//  chain
//
//  Created by Danny van Swieten on 24/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "engine.hpp"

// Abtract baseclass for a game.

class Game
{
public:
    
    Game(const std::string& name);
    virtual ~Game() = default;
    
    void start();
    void pause();
    void stop();
    
    virtual void onStart()  = 0;
    virtual void onPause()  = 0;
    virtual void onStop()   = 0;
    
protected:
    
    Engine engine;
    std::string name;
};
