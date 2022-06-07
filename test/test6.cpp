#include "CecEnumParser.h"
#include "CecCodeGenerator.h"
#include "cec.h"

#include "TestHelper.h"
TestHelper testHelper;
#define assert_true(x) testHelper((x), __LINE__, __FILE__);

#include <string>
#include <vector>

using namespace cec;

bool one_test(const std::string& in, const std::string sample)
{
    auto pEnumClass = CecEnumParser::parse(in);
    if(pEnumClass != nullptr)
    {
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
    else
    {
        std::cout<<"unit test CecEnumParser::parse failed"<<std::endl;
        return false;
    }

    auto result = CecCodeGenerator::genCode(*pEnumClass, sample);
    if(!result.empty())
    {
        std::cout<<"--[genCode-begin]-------------------------"<<std::endl;
        std::cout<<result<<std::endl;
        std::cout<<"--[genCode-end]---------------------------"<<std::endl;
        return true;
    }
    return false;
};

int test_code_generator()
{
    std::string in{"enum class Color:char{Red = -1, Green, Blue};"};
    std::string sample = R"(
[GenCecEnumName]        = {cec:enum:name}
[GenCecEnumFullName]    = {cec:enum:fullName}
[GenCecEnumType]        = {cec:enum:type}
[GenCecEnumMin]         = {cec:enum:min}
[GenCecEnumMax]         = {cec:enum:max}
[GenCecEnumSize]        = {cec:enum:size}
[GenCecEnumFirstKey]    = {cec:enum:firstKey}
[GenCecEnumLastKey]     = {cec:enum:lastKey}
[GenCecEnumKeyList]-----------------------------
{cec:enum:keyList}
[GenCecEnumKeyValueList]------------------------
{cec:enum:keyValueList}
-------------------------------------------------
[GenCecSignature]       = {cec:signature}
)";
    assert_true(one_test(in,sample));

//--------------------------------------------------------------

    in = 
R"(
/* head comment */
enum class Color{
    Red,    // key comment 1;
    Green,  // key comment 2; 
    Blue    // key comment 3;
};
)";
    sample = 
R"(
{cec:keep:comment}
struct Color
{
    {cec:enum:keyList}
    //--------------
    {cec:enum:keyValueList}
}
)";
    assert_true(one_test(in,sample));
    
    return testHelper.report();
}

int main()
{
    return test_code_generator();
}