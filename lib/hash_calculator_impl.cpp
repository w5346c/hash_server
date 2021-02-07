#include "hash_calculator_impl.h"

#include <iomanip>
#include <sstream>

namespace hash_server {

std::string HashCalculatorImpl::CalculateHashString(const std::string& s)
{
    // std::hash is implementation-dependent;
    // in production it would be better to use some standard hash algorithm like SHA

    std::stringstream stream;
    stream << std::hex << std::hash<std::string>{}(s);

    std::string res{stream.str()};
    return res;
}

std::shared_ptr<HashCalculator> CreateHashCalculator()
{
    return std::make_shared<HashCalculatorImpl>();
}

} // namespace hash_server