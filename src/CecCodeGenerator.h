#pragma once

#include <string>

namespace cec
{
    struct EnumClass;
    class CecCodeGenerator
    {
    public:
        static std::string genCode(const EnumClass &ec, 
                                    const std::string &sample);
    };

} // namespace cec
