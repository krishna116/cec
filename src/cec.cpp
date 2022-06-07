/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "CecWaiter.h"
#include "config.h"
#include "cec.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace cec;

int ParseArgs(int argc, char *argv[])
{
    CecWaiter waiter;
    bool hasSetSourceFile = false;
    bool good = true;

    auto showHelp = []()
    {
        std::cout << config::HelpStr << std::endl;
        return 0;
    };

    if (argc == 1)
    {
        return showHelp();
    }

    int i = 1;
    while (i < argc)
    {
        if ((std::string(argv[i]) == "--output" || std::string(argv[i]) == "-o" )
            && (i + 1 < argc) 
            && argv[i + 1][0] != '-')
        {
            waiter.setTargetFile(std::string(argv[i + 1]));
            i += 2;
        }
        else if ((std::string(argv[i]) == "--import" || std::string(argv[i]) == "-i") 
                && (i + 1 < argc) 
                && argv[i + 1][0] != '-')
        {
            waiter.setSampleFile(std::string(argv[i + 1]));
            i += 2;
        }
        else if ((std::string(argv[i]) == "--export" || std::string(argv[i]) == "-e") 
                && (i + 1 < argc) 
                && argv[i + 1][0] != '-' 
                && argc == 3)
        {
            return waiter.exportSample(std::string(argv[i + 1]));
            // i += 2;
        }
        else if (std::string(argv[i]) == "--help" 
                || std::string(argv[i]) == "-h")
        {
            return showHelp();
        }
        else if (std::string(argv[i]) == "--version" 
                || std::string(argv[i]) == "-v")
        {
            std::cout << config::VersionStr << std::endl;
            return 0;
        }
        else if (i < argc && argv[i][0] != '-' && !hasSetSourceFile)
        {
            waiter.setSourceFile(std::string(argv[i]));
            hasSetSourceFile = true;
            i += 1;
        }
        else
        {
            good = false;
            break;
        }
    }

    if(!good || !hasSetSourceFile)
    {
        print_error(" Invalid parameters input.\n");
        return -1;
    }

    return waiter.work();
}

int main(int argc, char *argv[])
{
    return ParseArgs(argc, argv);
}
