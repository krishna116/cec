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

namespace cec
{
    struct EnumClass;
    /**
     * @brief Check EnumClass is valid or not.
     *
     */
    class CecEnumChecker
    {
    public:
        /**
         * @brief Check EnumClass is valid or not.
         * 
         * @param enumClass     The EnumClass to be checked.
         * 
         * @return true         It is valid.
         * @return false        It is invalid.
         */
        static bool check(const EnumClass &enumClass);

    private:
        /**
         * @brief Check EnumClass.name is valid or not.
         * 
         * @param enumClass     The EnumClass to be checked.
         * 
         * @return true         It is valid.
         * @return false        It is invalid.
         */
        static bool checkEnumClassName(const EnumClass &enumClass);
        /**
         * @brief Check EnumClass.type is valid or not.
         * 
         * @param enumClass     The EnumClass to be checked.
         * 
         * @return true         It is valid.
         * @return false        It is invalid.
         */
        static bool checkEnumClassType(const EnumClass &enumClass);
        /**
         * @brief Check EnumClass.type and value range is valid or not.
         * 
         * @param enumClass     The EnumClass to be checked.
         * 
         * @return true         It is valid.
         * @return false        It is invalid.
         */
        static bool checkEnumClassTypeAndRange(const EnumClass &enumClass);
        /**
         * @brief Check EnumClass.keys is valid or not.
         * 
         * @param enumClass     The EnumClass to be checked.
         * 
         * @return true         It is valid.
         * @return false        It is invalid.
         */
        static bool checkEnumClassKeys(const EnumClass &enumClass);
    };
} // namespace cec
