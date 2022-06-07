#include "CecEnumParser.h"
#include "CecEnumChecker.h"

#include "TestHelper.h"
TestHelper testHelper;
#define assert_true(x) testHelper((x), __LINE__, __FILE__);

#include <string>
#include <vector>

using namespace cec;

bool one_test(const std::string& stream)
{
    auto pEnumClass = CecEnumParser::parse(stream);
    if(pEnumClass == nullptr)
    {
        return false;
    }

    if(CecEnumChecker::check(*pEnumClass))
    {
        return true;
    }
    
    return false;
};

int test_CecEnumChecker()
{
    // Basic check.
    assert_true(one_test("enum class Color:char{Red = -1, Green, Blue};"));
    assert_true(one_test("enum class Color{Red, Green, Blue}"));
    assert_true(one_test("enum Color{Red, Green, Blue}"));
    assert_true(one_test("enum struct Color{Red, Green, Blue}"));
    assert_true(one_test("enum Color{Red}"));
    assert_true(one_test("enum _{a}"));
    assert_true(one_test("enum a{_}"));

    // Value limit.
    assert_true(one_test("enum class Color:char{Red = 124, Green, Blue};"));
    assert_true(one_test("enum class Color:char{Red = 125, Green, Blue};")==false);
    assert_true(one_test("enum class Color:any_user_type{Red = 125, Green, Blue};"));

    // Abnormal input.
    assert_true(one_test("enum Color{}")==false);
    assert_true(one_test("enum Color{")==false);
    assert_true(one_test("enum Color}")==false);
    assert_true(one_test("enum Color") == false);
    assert_true(one_test("enum Color{{}}") == false);
    assert_true(one_test("enum")==false);
    assert_true(one_test("enum 123{Red}")==false);
    assert_true(one_test("enum _123{Red}"));
    assert_true(one_test("enum Color{123}")==false);
    assert_true(one_test("enum Color{_123}"));
    assert_true(one_test("enum Color{123,456}")==false);
    assert_true(one_test("enum Color{f123,F456}"));
    assert_true(one_test("enum Color{0x1,0x0}")==false);
    assert_true(one_test("enum Color{a0x1,b0x0}"));
    assert_true(one_test("enum 0x{a0x1,b0x0}")==false);
    assert_true(one_test("enum 0x0{a0x1,b0x0}")==false);
    assert_true(one_test("enum a0x0{a0x1,b0x0}"));

    // Accept dec and hex number.
    assert_true(one_test("enum class Color:int{Red = 0xff}"));
    assert_true(one_test("enum class Color:int{Red = +0XABCDEF}"));
    assert_true(one_test("enum class Color:int{Red = -0xabcdef}"));
    assert_true(one_test("enum class Color123:int{Red = +123}"));
    assert_true(one_test("enum class Color456:int{Red = -456}"));
    assert_true(one_test("enum class Color789:int{Red = -789}"));
    assert_true(one_test("enum class Color:int{Red = 0xffffffff}")==false);
    assert_true(one_test("enum class Color:int{Red = 0x7fffffff}")==false);
    assert_true(one_test("enum class Color:int{Red = 0x7ffffffe}"));

    // C++ keyword cannot be the name.
    assert_true(one_test("enum a{char}")==false);
    assert_true(one_test("enum a{int}")==false);
    assert_true(one_test("enum a{short}")==false);
    assert_true(one_test("enum a{long}")==false);
    assert_true(one_test("enum char{a}")==false);
    assert_true(one_test("enum int{a}")==false);
    assert_true(one_test("enum short{a}")==false);
    assert_true(one_test("enum long{a}")==false);
    assert_true(one_test("class enum a{b}")==false);
    assert_true(one_test("enum class a{b}")==true);
    assert_true(one_test("enum a{b}")==true);
    assert_true(one_test("enum enum{b}")==false);
    assert_true(one_test("enum struct{b}")==false);

    // By pass.
    assert_true(one_test("enum a{a}")==true);

    // Duplicated key.
    assert_true(one_test("enum foo{a,a}")==false);

    // The name and key size limit 31
    assert_true(one_test("enum a{a123456789_123456789_123456789_}"));
    assert_true(one_test("enum a{a123456789_123456789_123456789__}")==false);
    assert_true(one_test("enum a123456789_123456789_123456789_{a}"));
    assert_true(one_test("enum a123456789_123456789_123456789__{a}")==false);

    // Test empty string.
    assert_true(one_test("")==false);
    assert_true(one_test("\0")==false);
    assert_true(one_test("\n")==false);
    assert_true(one_test("\r")==false);

    {
        // range[1, 127] has 127 keys, it should pass.
        std::string in{"enum class Color:char{"};
        for(int i = 1; i <= 126; i++)
        {
            in += "a"+std::to_string(i) +",";
        }
        in += "a_end};";
        assert_true(one_test(in));
    }
    {
        // range[1, 128] has 128 keys, it should fail.
        std::string in{"enum class Color:char{"};
        for(int i = 1; i <= 127; i++)
        {
            in += "a"+std::to_string(i) +",";
        }
        in += "a_end};";
        assert_true(one_test(in) == false);
    }
    {
        // range[-1, 125] has 127 keys, it should pass.
        std::string in{"enum class Color:char{az=-1"};
        for(int i = 0; i <= 125; i++)
        {
            in += ",a"+std::to_string(i);
        }
        in += "}";
        assert_true(one_test(in));
    }
    {
        // range[-1, 126] has 128 keys, it should fail.
        std::string in{"enum class Color:char{az=-1"};
        for(int i = 0; i <= 126; i++)
        {
            in += ",a"+std::to_string(i);
        }
        in += "}";
        assert_true(one_test(in)==false);
    }

    return testHelper.report();
}

int main()
{
    return test_CecEnumChecker();
}