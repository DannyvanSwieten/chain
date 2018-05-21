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
#include <lua.hpp>

class World;

class System
{
public:
    
    using StateUpdate = std::function<void(World&, double dt)>;
    using PreStartTask = std::function<void(World&)>;
    
    System(World& w);
    
    void onStart(World&);
    void update(World&, double);
    void scheduleStateUpdate(StateUpdate update);
    void schedulePreStartTask(PreStartTask);

    virtual void start(World&) { }
    virtual void operator()(World&, double) = 0;
    virtual void reflect(chaiscript::ChaiScript& scriptContext) { };
    virtual void reflect(lua_State* scriptContext) { };
    
protected:
    
    World& world;
    
private:
    
    moodycamel::ConcurrentQueue<StateUpdate> stateUpdates;
    moodycamel::ConcurrentQueue<PreStartTask> preStartTasks;
};
