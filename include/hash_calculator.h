#pragma once

#include <string>
#include <memory>

namespace hash_server {

class HashCalculator
{
public:
    virtual std::string CalculateHashString(const std::string& s) = 0;
};

std::shared_ptr<HashCalculator> CreateHashCalculator();

} // namespace hash_server