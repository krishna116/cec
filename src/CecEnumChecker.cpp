/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "CecEnumChecker.h"
#include "cec.h"
#include "config.h"

using namespace cec;

bool CecEnumChecker::checkEnumClassName(const EnumClass &enumClass)
{
    auto const &fobbridenNames = config::KeywordForbidden::getForEnumName();
    for (auto const &fobbridenName : fobbridenNames)
    {
        if (fobbridenName == enumClass.name)
        {
            print_error("C or C++ keyword = [", fobbridenName,
                        "], it's unsafe and should not be used as enum-class-name\n");
            return false;
        }
    }
    return true;
}

bool CecEnumChecker::checkEnumClassKeys(const EnumClass &enumClass)
{
    auto const &fobbridenKeys = config::KeywordForbidden::getForEnumName();
    for (auto const &fobbridenKey : fobbridenKeys)
    {
        for (auto const &enumKey : enumClass.keys)
        {
            if (fobbridenKey == enumKey)
            {
                print_error("C or C++ keyword = [", fobbridenKey,
                            "], it's unsafe and should not be used as enum-key's name\n");
                return false;
            }
        }
    }

    for (auto const &enumKey : enumClass.keys)
    {
        if (enumKey == enumClass.name)
        {
            print_info("[warn]: found enum key = [", enumKey,
                       "], which is the same as the enum-class-name.\n");
            // return false;
        }
    }

    return true;
}

bool CecEnumChecker::checkEnumClassType(const EnumClass &enumClass)
{
    auto const &fobbridenTypeNames = config::KeywordForbidden::getForEnumType();
    for (auto const &fobbridenTypeName : fobbridenTypeNames)
    {
        if (enumClass.type == fobbridenTypeName)
        {
            print_error("C or C++ keyword = [", fobbridenTypeName,
                        "], it's unsafe and should not be used as enum-scalar-type\n");
            return false;
        }
    }
    return true;
}

bool CecEnumChecker::checkEnumClassTypeAndRange(const EnumClass &enumClass)
{
    auto inRange = [&](int64_t min, int64_t number, int64_t size, int64_t max)
    {
        // The number is allocating start number,
        // which may be -288/-1/0/123/256 and so on,
        // so it may be invalid,
        // and ++number may cannot hold all the keys.
        if (min <= number && number <= max && size <= max)
        {
            if (number <= 0)
            {
                return (number + size) <= max;
            }
            else
            {
                // Make sure (number + size) don't overflow int64_t;
                int64_t numberHighPart = (int64_t)(number >> 32);
                int64_t sizeHighPart = (int64_t)(number >> 32);
                int64_t highPartOverflow = (numberHighPart + sizeHighPart) >> 32;
                // if high parts overflow, check fail.
                if (highPartOverflow)
                {
                    return false;
                }
                int64_t numberLowPart = (int64_t)(number & 0xffffffff);
                int64_t sizeLowPart = (int64_t)(size & 0xffffffff);
                int64_t lowPartOverflow = (numberLowPart + sizeLowPart) >> 32;

                // if (high parts + low-parts-overflow) overflow, check fail.
                highPartOverflow = (numberHighPart + sizeHighPart + lowPartOverflow) >> 32;
                if (highPartOverflow)
                {
                    return false;
                }

                return (number + size) <= max;
            }
        }
        return false;
    };

    auto TryWarn = [](int64_t number, int64_t size)
    {
        if(number > LONG_MAX || (number+size) > LONG_MAX || number < LONG_MIN)
        {
            print_info("[warn]: the value range is out of [LONG_MIN, LONG_MAX].\n");
            print_info("[warn]: you C/C++ compiler may not support it.\n");
            print_info("    LONG_MIN  = ", LONG_MIN, "\n");
            print_info("    LONG_MAX  = ", LONG_MAX, "\n");
            print_info("    Current scalar type min value = ", number, "\n");
            print_info("    Current scalar type max value = ", number+size-1, "\n");
            print_info("    Current reserved number       = ", number+size, "\n");
        }
        return true;
    };

    int64_t size = enumClass.keys.size();
    for (auto const &typeAndRange : config::TypeAndRange::get())
    {
        if (typeAndRange.type == enumClass.type)
        {
            if (!inRange(typeAndRange.min, enumClass.number, size, typeAndRange.max))
            {
                print_error("enum number is out of range, it cannot hold all the keys.\n");
                print_error("NOTE:\n");
                print_error("  Current enum scalar type = ", typeAndRange.type, "\n");
                print_error("  The scalar type min value = ", typeAndRange.min, "\n");
                print_error("  The scalar type max value = ", typeAndRange.max, "\n");
                print_error("  First enum number = ", enumClass.number, "\n");
                print_error("  Current enum key size = ", size, "\n");
                print_error("  The max available number = ", typeAndRange.max - 1, "\n");
                print_error("  The reserved number = ", typeAndRange.max, "\n");
                return false;
            }
            else
            {
                //
                // The value range is ok,
                // But the C/C++ compiler may not support long data.
                // Try give some warnning message.
                //
                TryWarn(enumClass.number, size);
                break;
            }
        }
    }

    return true;
}

bool CecEnumChecker::check(const EnumClass &enumClass)
{
    if (checkEnumClassName(enumClass) 
        && checkEnumClassType(enumClass) 
        && checkEnumClassTypeAndRange(enumClass) 
        && checkEnumClassKeys(enumClass))
    {
        return true;
    }

    return false;
}