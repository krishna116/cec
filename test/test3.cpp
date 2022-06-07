#include "CecEnumLexer.h"

#include "TestHelper.h"
TestHelper testHelper;
#define assert_true(x) testHelper((x), __LINE__, __FILE__);

#include <iostream>
#include <string>
#include <vector>

using namespace cec;
bool one_test(const std::string& str)
{
    CecEnumLexer::Context ctx;
    ctx.curser = reinterpret_cast<const uint8_t*>(str.data());
    ctx.limit = reinterpret_cast<const uint8_t*>(str.data()) + str.size();
    CecEnumLexer::TokenType token = CecEnumLexer::TokenType::LexerError;

    while (static_cast<int>(token = CecEnumLexer::lex(ctx)) > 0)
    {
        std::cout << "TokenId = " << static_cast<int>(token)
                  << ", TokenName = [" << ctx.token()
                  << "]\n";
    }

    return token == CecEnumLexer::TokenType::LexerOk;
}

std::string utf8Str = 
R"(
    /**
     *  대한민국 汉字体系 Русский ประเทศไทย
     */
    enum class Font
    {
        Korean,     // 대한민국
        kanji,      // 汉字体系
        Russian,    // Русский
        Thai        // ประเทศไทย
    };
)";

int test_lexer()
{
    assert_true(one_test("aaa"));
    assert_true(one_test("aaa123"));
    assert_true(one_test("Aa123"));
    assert_true(one_test("Aa_123"));
    assert_true(one_test("_Aa_123"));
    assert_true(one_test("0xff"));
    assert_true(one_test("+0xff"));
    assert_true(one_test("-0xff"));
    assert_true(one_test("123456"));
    assert_true(one_test("-123456"));
    assert_true(one_test("+123456"));
    assert_true(one_test("{:;,}"));
    assert_true(one_test("enum class Color:{Red=1,Blue};"));
    assert_true(one_test(""));
    assert_true(one_test("\n"));
    assert_true(one_test("\0"));
    assert_true(one_test(utf8Str));

    return testHelper.report();
}

int main()
{
    return test_lexer();
}