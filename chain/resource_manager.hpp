//
//  resource_manager.hpp
//  chain
//
//  Created by Danny van Swieten on 06/06/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <boost/filesystem.hpp>
#include <map>
#include <string>

class ShaderResource
{
    std::string material;
    std::string vShaderSource;
    std::string fShaderSource;
};

class ResourceManager
{
public:
    using Path = boost::filesystem::path;
    
private:
    
    std::map<std::string, ShaderResource> shaders;
};
