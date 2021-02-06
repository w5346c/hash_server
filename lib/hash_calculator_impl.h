#pragma once

#include "hash_calculator.h"

namespace hash_server {

class HashCalculatorImpl : public HashCalculator
{
public:
    std::string CalculateHashString(const std::string& s) override;
};

} // namespace hash_server