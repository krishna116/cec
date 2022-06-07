#include "Color.hpp" // Test target.

#include <iostream> // for test console io
#include <string>   // for test compare string
#include <cassert>  // for test assert

#define assert_true(x) assert(x)

int main()
{
    // Create enum.
    Color c1 = Color::Blue;
    assert_true(c1 == Color::Blue);

    // Convert enum to string.
    assert_true(std::string("Blue") == Color::_toString(c1));

    // Convert string to enum.
    Color c2 = Color::_fromString("Red");
    Color c3 = Color::Red;
    assert_true(Color::_good(c2));
    assert_true(Color::_good(c3));
    assert_true(c2 == c3);

    // Convert string to enum, a bad example
    Color c4 = Color::_fromString("a bad string");
    assert_true(std::string("") == Color::_toString(c4));
    assert_true(Color::_good(c4) == false);

    // Iterate.
    for (auto ev = Color::_min(); ev <= Color::_max(); ++ev){
        std::cout << std::to_string(ev);
        std::cout << " ==> ";
        std::cout << Color::_toString(Color(ev));
        std::cout << std::endl;
    }

    // Iterate all keys.
    for (auto i = 0; i < Color::_size(); i++){
        std::cout << "key[" << i << "] = " << Color::_keyTable()[i] << std::endl;
    }

    // Size check.
    Color c = Color::Blue;
    assert_true(sizeof(c) == sizeof(Color));
    assert_true(sizeof(c) == sizeof(char));
    assert_true(sizeof(c) == sizeof(Color::_min()));
    assert_true(sizeof(c) == sizeof(Color::_max()));

    // Switch-case.
    Color c5 = Color::Green;
    switch (c5){
        case Color::Red: { assert_true(0); break; }
        case Color::Green: { assert_true(1); break; }
        case Color::Blue: { assert_true(0); break; }
        default: { assert_true(0); }
    }

    // Others.
    assert_true(Color::_size() == 3);
    assert_true(std::string("Color") == Color::_name());
    assert_true(std::string("char") == Color::_type());

    std::cout << "--done--" << std::endl;
    return 0;
}