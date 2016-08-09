#pragma once

#include <fstream>
#include <string>
#include <bitset>
#include "utils.h"

namespace DEBUGFUN
{
	inline bool fileTest(const std::string& fn)
	{
		std::ifstream f(fn.c_str());
		return f.good();
	}
}