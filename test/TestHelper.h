#pragma once

#include <iostream>
#include <iomanip>

struct TestHelper
{
    int error_count = 0;
    int total_count = 0;
    void operator()(bool value, int line, const char *file)
    {
        ++total_count;
        if (value == false)
        {
            ++error_count;
            std::cout << "[fail]:";
            std::cout << " assert-line = " << line;
            std::cout << " file = " << file;
            std::cout << "\n";
        }
        else
        {
            std::cout << "[pass]:";
            std::cout << " pass-line = " << line;
            std::cout << " file = " << file;
            std::cout << "\n";
        }
    }
    int report()
    {
        std::cout << "[test-report]-----------------------------------\n";
        std::cout << "\t[total] = " << total_count;
        std::cout << ", [pass] = " << total_count - error_count;
        std::cout << ", [fail] = " << error_count;
        std::cout << "\n";
        return error_count;
    }
};
