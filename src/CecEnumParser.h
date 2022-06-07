/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#pragma once

#include <string>
#include <memory>
#include <set>

namespace cec
{
    struct EnumClass;

   /**
     * @brief Get EnumClass from stream.
     * 
     */ 
    class CecEnumParser
    {
    public:
        static std::shared_ptr<EnumClass> parse(const std::string &stream);
    private:
        static bool eatEnumClassName(std::shared_ptr<EnumClass> pEnumClass,
                                     const std::string &token);
                                     
        static bool eatEnumClassFullNamePart1(std::shared_ptr<EnumClass> pEnumClass,
                                     const std::string &token);
        static bool eatEnumClassFullNamePart2(std::shared_ptr<EnumClass> pEnumClass,
                                     const std::string &token);
        static bool eatEnumClassFullNamePart3(std::shared_ptr<EnumClass> pEnumClass,
                                     const std::string &token);
        static bool eatEnumClassFullNamePart4(std::shared_ptr<EnumClass> pEnumClass,
                                     const std::string &token);

        static bool eatEnumClassScalarType(std::shared_ptr<EnumClass> pEnumClass,
                                     const std::string &token);
        static bool eatEnumClassKey(std::shared_ptr<EnumClass> pEnumClass,
                                    const std::string &token, 
                                    std::set<std::string>& enumClassKeySet);
        static bool eatEnumClassNumber(std::shared_ptr<EnumClass> pEnumClass,
                                       const std::string &token);
        
        /**
         * @brief The number of keys shouldn't overflow int64_t max.
         * 
         * @see EnumCodeGenerator::genEnumClassMaximalNumber implementation.
         * 
         * @param pEnumClas 
         * @return true 
         * @return false 
         */
        static bool doPostWork(std::shared_ptr<EnumClass> pEnumClas);

        static int64_t toNumber(const std::string &token, 
                                const int radix, 
                                bool &result);
    };
} // namespace cec