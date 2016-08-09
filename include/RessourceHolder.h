#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <cassert>
#include <fstream>
#include "DebugFun.h"

template<typename Resource, typename Identifier>
class ResourceHolder
{
public:
	void load(Identifier id, const std::string& filename)
	{
		assert(DEBUGFUN::fileTest(filename));

		std::unique_ptr<Resource> res(new Resource());
		if (!(res->loadFromFile(filename)))
			throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

		insertResource(id, std::move(res));
	}

	template<typename Parameter>
	void load(Identifier id, const std::string& filename, const Parameter& secondParam)
	{
		assert(DEBUGFUN::fileTest(filename));

		std::unique_ptr<Resource> res(new Resource());
		if (!res->loadFromFile(filename, secondParam))
			throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

		insertResource(id, std::move(res));
	}

	Resource& resource(Identifier id)
	{
		auto found = resourceById_.find(id);
		assert(found != resourceById_.end());

		return *found->second;
	}
	const Resource& resource(Identifier id) const
	{
		auto found = resourceById_.find(id);
		assert(found != resourceById_.end());

		return *found->second;
	}

private:
	void insertResource(Identifier id, std::unique_ptr<Resource> res)
	{
		// Insert and check success
		auto inserted = resourceById_.insert(std::make_pair(id, std::move(res)));
		assert(inserted.second);
	}

private:
	std::map<Identifier, std::unique_ptr<Resource> > resourceById_;
};
