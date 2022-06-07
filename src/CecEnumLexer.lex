/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "CecEnumLexer.h"

using namespace cec;

CecEnumLexer::TokenType CecEnumLexer::lex(CecEnumLexer::Context& ctx) 
{
    uint8_t yych;

    auto eatComment = [&](TokenType type)
    {
        ctx.tokenInfoArrayForCmt.push_back(TokenInfo(type, ctx.token()));
        return type;
    };

    for(;;)
    {
        ctx.yytext = ctx.curser;
    /*!local:re2c:lex
        re2c:define:YYCTYPE     = "uint8_t";
        re2c:flags:utf-8        = 1;
        re2c:yyfill:enable      = 0;
        re2c:define:YYCURSOR    = "ctx.curser";
        re2c:define:YYMARKER    = "ctx.yymarker";
        re2c:eof                = 0;
        re2c:define:YYLIMIT     = "ctx.limit";

        Dec             = ("+"|"-")?[0-9]{1,31};
        Hex             = ("+"|"-")?("0x"|"0X")[0-9A-Fa-f]{1,31}; 
        TokenNumber     = Dec | Hex;
        TokenEnum       = "enum";
        TokenClass      = "class" | "struct";
        TokenCName      = [a-zA-Z_][a-zA-Z0-9_]{0,30};
        TokenColon      = ":";
        TokenLB         = "{";
        TokenEq         = "=";
        TokenComma      = ",";
        TokenRB         = "}";
        TokenSemicolon  = ";";

        CommentLine     = ("//" [^\n]* "\n")+;
        CommentMult     = ("/*" ([^*] | ("*" [^/]))* "*""/")+;
        Spaces           = ([ \t\v\n\r])+;

        // Be careful, the token order matters.
        TokenEnum   {   ctx.yyleng = ctx.curser - ctx.yytext;
                        return eatComment(TokenType::TokenEnum); 
                    }

        TokenClass  {   ctx.yyleng = ctx.curser - ctx.yytext;
                        return TokenType::TokenClass; }

        TokenCName  {   ctx.yyleng = ctx.curser - ctx.yytext;
                        return eatComment(TokenType::TokenCName); 
                    }

        TokenColon  {   ctx.yyleng = ctx.curser - ctx.yytext;
                        return TokenType::TokenColon; }

        TokenLB     {   ctx.yyleng = ctx.curser - ctx.yytext;
                        return eatComment(TokenType::TokenLB); 
                    }

        TokenEq     {   ctx.yyleng = ctx.curser - ctx.yytext;
                        return TokenType::TokenEq; }

        TokenNumber {   ctx.yyleng = ctx.curser - ctx.yytext;
                        return TokenType::TokenNumber; }

        TokenComma  {   ctx.yyleng = ctx.curser - ctx.yytext;
                        return eatComment(TokenType::TokenComma); 
                    }

        TokenRB     {   ctx.yyleng = ctx.curser - ctx.yytext;
                        return eatComment(TokenType::TokenRB); 
                    }

        TokenSemicolon {ctx.yyleng = ctx.curser - ctx.yytext;
                        return TokenType::TokenSemicolon; }

        CommentLine {  ctx.yyleng = ctx.curser - ctx.yytext;
                       eatComment(TokenType::TokenComment);
                       continue;
                    }
        CommentMult {  ctx.yyleng = ctx.curser - ctx.yytext;
                       eatComment(TokenType::TokenComment);
                       continue;
                    }
        Spaces      {  ctx.yyleng = ctx.curser - ctx.yytext;
                       eatComment(TokenType::TokenComment);
                       continue; 
                    }
        $           {  return TokenType::LexerOk; }
        *           {  return TokenType::LexerError; }
    */
    }
    
    return TokenType::LexerOk;
}

// NOLINTBEGIN(*)
bool CecEnumLexer::isValidCName(const uint8_t* str, int length)
{
    const uint8_t*YYCURSOR = str;
    const uint8_t*YYLIMIT = str + length;
    const uint8_t*YYMARKER;
    for(;;)
    {
    /*!local:re2c:isValidCName
        re2c:define:YYCTYPE     = "uint8_t";
        re2c:flags:utf-8        = 1;
        re2c:yyfill:enable = 0;
        re2c:eof = 0;

        CName = [a-zA-Z_][a-zA-Z0-9_]{0,30};

        CName   { return true; }
        $       { return false; }
        *       { return false; }
    */
    }

    return false;
}
// NOLINTEND(*)

// NOLINTBEGIN(*)
bool CecEnumLexer::isValidDecNumber(const uint8_t* str, int length)
{
    const uint8_t*YYCURSOR = str;
    const uint8_t*YYLIMIT = str + length;
    const uint8_t*YYMARKER;
    for(;;)
    {
    /*!local:re2c:isValidDecNumber
        re2c:define:YYCTYPE     = "uint8_t";
        re2c:flags:utf-8        = 1;
        re2c:yyfill:enable = 0;
        re2c:eof = 0;

        DecNum = ("+"|"-")?[0-9]{1,31};

        DecNum  { return (YYCURSOR - str) == length; }
        $       { return false; }
        *       { return false; }
    */
    }

    return false;
}
// NOLINTEND(*)

// NOLINTBEGIN(*)
bool CecEnumLexer::isValidHexNumber(const uint8_t* str, int length)
{
    const uint8_t *YYCURSOR = str;
    const uint8_t *YYLIMIT = str + length;
    const uint8_t *YYMARKER;
    for(;;)
    {
    /*!local:re2c:isValidHexNumber
        re2c:define:YYCTYPE     = "uint8_t";
        re2c:flags:utf-8        = 1;
        re2c:yyfill:enable = 0;
        re2c:eof = 0;

        HexNum = ("+"|"-")? "0" [xX][0-9A-Fa-f]{1,31}; 

        HexNum  { return (YYCURSOR - str) == length;}
        $       { return false; }
        *       { return false; }
    */
    }

    return false;
}
// NOLINTEND(*)


std::size_t GetIndentionSize(const std::string& in)
{
    auto pos = in.find_first_not_of(" \t");
    if(pos == std::string::npos)
    {
        pos = 0;
    }
    return pos;
}

// NOLINTBEGIN(*)
bool CecEnumLexer::getFullCecEnumKeyListTag(const std::string& in, 
                                    std::string& cecEnumKeyTableTag,
                                    std::size_t& indention
                                    )
{
    const uint8_t * begin = (const uint8_t *)in.data();
    const uint8_t *YYCURSOR = begin;
    const uint8_t *YYLIMIT = begin + in.size();
    const uint8_t *YYMARKER;
    const uint8_t *yyText;
    for(;;)
    {
        yyText = YYCURSOR;
    /*!local:re2c:getFullCecEnumKeyTableTag
        re2c:define:YYCTYPE     = "uint8_t";
        re2c:flags:utf-8        = 1;
        re2c:yyfill:enable = 0;
        re2c:eof = 0;

        PrefixSpaces            = ([ \t])+;
        SuffixSpaces            = ([ \t\v\n\r])+;
        CecEnumKeyTableTag      = "{cec:enum:keyList}"; 
        //CecEnumKeyTableFullTag  = (PrefixSpaces)? CecEnumKeyTableTag (SuffixSpaces)?;
        CecEnumKeyTableFullTag  = (PrefixSpaces)? CecEnumKeyTableTag ;

        CecEnumKeyTableFullTag  { 
                cecEnumKeyTableTag = std::string((const char*)yyText, YYCURSOR - yyText);
                indention = GetIndentionSize(cecEnumKeyTableTag);
                return true;
            }
        $   { return false; }
        *   { continue; }
    */
    }

    return false;
}
// NOLINTEND(*)

// NOLINTBEGIN(*)
bool CecEnumLexer::getFullCecEnumKeyValueListTag(const std::string& in, 
                                    std::string& cecEnumKeyValueTableTag,
                                    std::size_t& indention
                                    )
{
    const uint8_t * begin = (const uint8_t *)in.data();
    const uint8_t *YYCURSOR = begin;
    const uint8_t *YYLIMIT = begin + in.size();
    const uint8_t *YYMARKER;
    const uint8_t *yyText;
    for(;;)
    {
        yyText = YYCURSOR;
    /*!local:re2c:getFullCecEnumKeyValueTableTag
        re2c:define:YYCTYPE     = "uint8_t";
        re2c:flags:utf-8        = 1;
        re2c:yyfill:enable = 0;
        re2c:eof = 0;

        PrefixSpaces                 = ([ \t])+;
        SuffixSpaces                 = ([ \t])+;
        CecEnumKeyValueTableTag      = "{cec:enum:keyValueList}"; 
        FullCecEnumKeyValueTableTag  = (PrefixSpaces)? CecEnumKeyValueTableTag (SuffixSpaces)?;

        FullCecEnumKeyValueTableTag  { 
                cecEnumKeyValueTableTag = std::string((const char*)yyText, YYCURSOR - yyText);
                indention = GetIndentionSize(cecEnumKeyValueTableTag);
                return true;
            }
        $   { return false; }
        *   { continue; }
    */
    }

    return false;
}
// NOLINTEND(*)

// NOLINTBEGIN(*)
bool CecEnumLexer::getFullCecKeepCommentTag(const std::string& in, 
                                    std::string& cecKeepCommentTag
                                    )
{
    const uint8_t * begin = (const uint8_t *)in.data();
    const uint8_t *YYCURSOR = begin;
    const uint8_t *YYLIMIT = begin + in.size();
    const uint8_t *YYMARKER;
    const uint8_t *yyText;
    for(;;)
    {
        yyText = YYCURSOR;
    /*!local:re2c:getFullCecKeepCommentTag
        re2c:define:YYCTYPE     = "uint8_t";
        re2c:flags:utf-8        = 1;
        re2c:yyfill:enable = 0;
        re2c:eof = 0;

        PrefixSpaces           = ([ \t])+;
        SuffixSpaces           = ([ \t\v\n\r])+;
        CecKeepCommentTag      = "{cec:keep:comment}"; 
        FullCecKeepCommentTag  = (PrefixSpaces)? CecKeepCommentTag (SuffixSpaces)?;

        FullCecKeepCommentTag  { 
                cecKeepCommentTag = std::string((const char*)yyText, YYCURSOR - yyText);
                return true;
            }
        $   { return false; }
        *   { continue; }
    */
    }
    return false;
}
// NOLINTEND(*)
