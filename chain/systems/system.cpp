//
//  system.cpp
//  chain
//
//  Created by Danny van Swieten on 20/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "system.hpp"


void System::update(World &w, double dt)
{
    StateUpdate update;
    while(stateUpdates.try_dequeue(update))
        update(w, dt);
    
    (*this)(w, dt);
}

void System::onStart(World& w)
{
    PreStartTask task;
    while(preStartTasks.try_dequeue(task))
        task(w);
    
    start(w);
}

void System::scheduleStateUpdate(StateUpdate update)
{
    stateUpdates.enqueue(update);
}

void System::schedulePreStartTask(PreStartTask update)
{
    preStartTasks.enqueue(update);
}
