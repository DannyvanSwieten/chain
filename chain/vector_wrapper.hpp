//
//  vector_wrapper.h
//  chain
//
//  Created by Danny van Swieten on 21/05/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <vector>

template<typename T>
struct vector_wrapper
{
    void push_back(const T& value_type)
    {
        value.push_back(value_type);
    }
    
    auto size() const
    {
        return value.size();
    }
    
    auto data() const
    {
        return value.data();
    }
    
    auto operator[](size_t index) const
    {
        return value[index];
    }
    
    std::vector<T> value;
};
