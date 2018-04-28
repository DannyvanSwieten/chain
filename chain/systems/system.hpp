//
//  system.hpp
//  chain
//
//  Created by Danny van Swieten on 20/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <moodycamel/concurrentqueue.h>
#include <chaiscript/chaiscript.hpp>

class World;

class System
{
public:
    
    using StateUpdate = std::function<void(World&, double dt)>;
    
    void update(World&, double);
    void scheduleStateUpdate(StateUpdate update);
    
    virtual void operator()(World&, double) = 0;
    virtual void reflect(chaiscript::ChaiScript& scriptContext) { };
    
protected:
    
    moodycamel::ConcurrentQueue<StateUpdate> stateUpdates;
};
