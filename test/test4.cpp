#include "CecEnumParser.h"
#include "cec.h"

#include <string>
#include <iostream>

using namespace cec;

int test_enum_parser()
{
    std::string stream{"enum class Color{Red = -1, Green, Blue};"};
    //std::string stream{"enum af"};

    auto pEnumClass = CecEnumParser::parse(stream);

    if(pEnumClass != nullptr)
    {
        if(pEnumClass->name.empty()) return 1;
        if(pEnumClass->fullName.empty()) return 1;
        if(pEnumClass->type.empty()) return 1;
        if(pEnumClass->keys.empty()) return 1;

        std::cout<<"--[dump]----------------------\n";
        std::cout<<"enum-name      = "<<pEnumClass->name<<std::endl;
        std::cout<<"enum-full-name = "<<pEnumClass->fullName<<std::endl;
        std::cout<<"type           = "<<pEnumClass->type<<std::endl;
        std::cout<<"number         = " << pEnumClass->number<<std::endl;
        for(auto const& key: pEnumClass->keys)
        {
            std::cout<<"  [+][key] = "<<key<<std::endl;
        }
        std::cout<<"[total key size] = "<<pEnumClass->keys.size()<<std::endl;
    }

    return pEnumClass == nullptr;
}

int test_enum_parser_with_comment()
{
    std::string stream{R"(
/* head comment */
enum class Color:char
{
    Red = -1,   // comment1;
    Green,      // comment2;
    Blue        // comment3;
};      
)"};
    //std::string stream{"enum af"};

    auto pEnumClass = CecEnumParser::parse(stream);
    if(pEnumClass != nullptr)
    {
        std::cout<<"--[dump-head-comment-begin]-----------------------------"<<std::endl;
        std::cout<<pEnumClass->comment.headComment<<std::endl;
        std::cout<<"--[dump-head-comment-finished]---------------------------"<<std::endl;
        std::cout<<"--[dump-keys-comment-begin]---------------------------"<<std::endl;
        for(auto const& line: pEnumClass->comment.keyComments)
        {
            std::cout<<line<<std::endl;
        }
        std::cout<<"--[dump-keys-comment-end]-----------------------------"<<std::endl;
    }

    return pEnumClass == nullptr; 
}

int main()
{
    int i = 0;
    i+= test_enum_parser();
    i+= test_enum_parser_with_comment();
    return i;
}