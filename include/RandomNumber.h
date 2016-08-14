#pragma once

#include <random>


namespace 
{
    std::default_random_engine createRandomEngine()
    {
        auto seed = static_cast<unsigned long>(std::time(nullptr));
        return std::default_random_engine(seed);
    }

    auto RandomEngine = createRandomEngine();
    int randomUniformInt(int exclusiveMaxInt)
    {
        std::uniform_int_distribution<> distribution(0, exclusiveMaxInt - 1);
        return distribution(RandomEngine);
    }

    template<typename T>
    T randomUniformReal(T exclusiveMaxReal)
    {
        std::uniform_real_distribution<T> distribution(0., exclusiveMaxReal);
        return distribution(RandomEngine);
    }
}