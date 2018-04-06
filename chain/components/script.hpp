//
//  script.hpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <functional>
#include <string>

struct Script
{
	std::function<void(size_t, double)> update;
};
