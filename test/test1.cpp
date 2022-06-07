//-----------------------------------------------------------------
// Test target = Color
//-----------------------------------------------------------------

#include "Color.hpp" // Test target file.

#include "TestHelper.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <set>

TestHelper testHelper;
#define assert_true(x) testHelper((x), __LINE__, __FILE__);

//
// --------------------------------------------------------------------------
//

void test_enum_scalar_type()
{
    assert_true(sizeof(char) == sizeof(Color));
    assert_true(sizeof(char) == sizeof(Color::_min()));
    assert_true(sizeof(char) == sizeof(Color::_max()));
    
    Color instance = Color::Red;
    assert_true(sizeof(char) == sizeof(instance.value));
    assert_true(sizeof(char) == sizeof(instance));

    assert_true(Color::_min() <= Color::_max());
    auto size = Color::_max() - Color::_min() + 1;
    assert_true(Color::_size() == size);

    assert_true(std::string("Color") == Color::_name());
    assert_true(std::string("char") == Color::_type());
}

void test_enum_class_value()
{
    {
        auto instance = Color(Color::_min());
        assert_true(Color::_good(instance));
    }

    {
        auto instance = Color(Color::_max());
        assert_true(Color::_good(instance));
    }
    {
        auto instance = Color(Color::_max() + 1);
        assert_true(Color::_good(instance) == false);
    }
}

void test_iterate_enum_value()
{
    for (auto ev = Color::_min(); ev <= Color::_max(); ++ev)
    {
        auto instance = Color(ev);
        assert_true(Color::_good(instance));
    }
}

void test_iterate_keyTable()
{
    std::set<std::string> tokenSet;
    for (auto i = 0; i < Color::_size(); i++)
    {
        std::string str(Color::_keyTable()[i]);
        tokenSet.insert(str);
        assert_true(!str.empty());
    }
    assert_true(tokenSet.size() == Color::_size());
}

void test_switch_case()
{
    // The first key may be the last key, so we should split test.
    {
        Color instance = Color::_min();
        switch (instance)
        {
        case Color::Red:
        {
            assert_true(1);
            break;
        }
        default:
        {
            assert_true(0);
        }
        }
    }

    {
        Color instance = Color::_max();
        switch (instance)
        {
        case Color::Blue:
        {
            assert_true(1);
            break;
        }
        default:
        {
            assert_true(0);
        }
        }
    }
}

void test_enum_to_string()
{
    {
        auto instance = Color(Color::_min());
        auto str1 = std::string(Color::_toString(instance));
        auto str2 = std::string(Color::_keyTable()[0]);
        assert_true(!str1.empty() && str1 == str2);
    }
    {
        auto instance = Color(Color::_max());
        auto str1 = std::string(Color::_toString(instance));
        auto str2 = std::string(Color::_keyTable()[Color::_size() - 1]);
        assert_true(!str1.empty() && str1 == str2);
    }
    {
        auto instance = Color(Color::_max() + 1);
        assert_true(Color::_good(instance) == false);
        auto str1 = std::string(Color::_toString(instance));
        assert_true(str1.empty());
    }
}

void test_enum_from_string()
{
    {
        auto str = std::string(Color::_keyTable()[0]);
        auto instance = Color::_fromString(str.c_str());
        assert_true(Color::_good(instance));
    }
    {
        auto str = std::string(Color::_keyTable()[Color::_size() - 1]);
        auto instance = Color::_fromString(str.c_str());
        assert_true(Color::_good(instance));
    }
    {
        auto str = std::string("a bad string");
        auto instance = Color::_fromString(str.c_str());
        assert_true(Color::_good(instance) == false);
    }
    {
        auto instance = Color::_fromString(nullptr);
        assert_true(Color::_good(instance) == false);
    }
    {
        auto instance = Color::_fromString("");
        assert_true(Color::_good(instance) == false);
    }
}

int unit_test_Color()
{
    test_enum_scalar_type();
    test_enum_class_value();
    test_iterate_enum_value();
    test_iterate_keyTable();
    test_switch_case();
    test_enum_to_string();
    test_enum_from_string();

    return testHelper.report();
}

int main()
{
    return unit_test_Color();
}
