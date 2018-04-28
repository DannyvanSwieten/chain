//
//  game.cpp
//  chain
//
//  Created by Danny van Swieten on 24/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "game.hpp"


Game::Game(const std::string &name):
name(name), engine(name)
{
    
}

void Game::start()
{
    onStart();
    engine.run();
}

