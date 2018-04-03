//
//  component_storage.hpp
//  chain
//
//  Created by Danny on 29/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <experimental/optional>
#include <tuple>
#include <vector>

template<class C>
class ComponentStorage
{
public:
	
	void reserveStorage(size_t N)
	{
		storage.reserve(N);
	}
	
	void expand()
	{
		storage.emplace_back();
	}
	
	void initialize(size_t index)
	{
		storage[index] = C();
	}
	
	void reset(size_t index)
	{
		storage[index] = std::experimental::nullopt;
	}
	
	size_t size() const noexcept
	{
		return storage.size();
	}
	
	auto begin()
	{
		return storage.begin();
	}
	
	auto end()
	{
		return storage.end();
	}
	
	auto& getStorage()
	{
		return storage;
	}
    
    bool has(size_t index) const
    {
        return (bool)storage[index];
    }
	
private:
	
	std::vector<std::experimental::optional<C>> storage;
};

template<typename... Ts>
class ComponentStorageCollection: public std::tuple<ComponentStorage<Ts>...>
{
public:
	
	template<size_t Index = 0>
	void reserve(size_t N)
	{
		std::get<Index>(*this).reserveStorage(N);
		if constexpr (Index < sizeof...(Ts) - 1) reserve<Index + 1>(N);
	}
	
	template<size_t Index = 0>
	constexpr void expand()
	{
		std::get<Index>(*this).expand();
		if constexpr (Index < sizeof...(Ts) - 1) expand<Index + 1>();
	}
	
	template<class C>
	constexpr void initialize(size_t index)
	{
		std::get<ComponentStorage<C>>(*this).initialize(index);
	}
	
	template<class C>
	constexpr void reset(size_t index)
	{
		std::get<ComponentStorage<C>>(*this).reset(index);
	}
	
	template<size_t Index = 0>
	constexpr void resetAll(size_t index)
	{
		std::get<Index>(*this).reset(index);
		if constexpr (Index < sizeof...(Ts) - 1) resetAll<Index + 1>(index);
	}
	
	template<class C>
	auto& getStorage()
	{
		return std::get<ComponentStorage<C>>(*this).getStorage();
	}
	
	template<class C>
	auto begin()
	{
		return std::get<ComponentStorage<C>>(*this).begin();
	}
	
	template<class C>
	auto end()
	{
		return std::get<ComponentStorage<C>>(*this).end();
	}
	
	auto size() const noexcept
	{
		return std::get<0>(*this).size();
	}
    
    template<typename... Components>
    auto getAllEntitiesWithComponents(std::vector<size_t>& output)
    {
        for(auto i = 0; i < size(); i++)
        {
            bool result = (std::get<ComponentStorage<Components>>(*this).has(i) & ...);
            
            if(result)
                output.emplace_back(i);
        }
    }
};
