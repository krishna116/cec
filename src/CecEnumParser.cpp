/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "CecEnumParser.h"
#include "CecEnumLexer.h"
#include "cec.h"
#include "config.h"

#include <stack>

using namespace cec;

void handleEnumClassComment(std::shared_ptr<EnumClass> pEnumClass,
                            CecEnumLexer::Context& ctx)
{
    if(ctx.tokenInfoArrayForCmt.empty()) return;

    std::stack<std::string> tokenStack;
    using TokenType = CecEnumLexer::TokenType;
    auto const begin = ctx.tokenInfoArrayForCmt.begin();
    auto const end = ctx.tokenInfoArrayForCmt.end();
    auto it2 = end;

    for(auto it = begin; it != end; ++it)
    {
        // Find "enum" position.
        if((*it).type == TokenType::TokenEnum)
        {
            it2 = it + 1;
            if(it != begin)
            {
                --it;
            }

            // Get head comment, backward manner.
            while(it!=begin && (*it).type == TokenType::TokenComment)
            {
                tokenStack.push((*it).token);
                --it;
            }

            // Get head comment, backward manner.
            if(it == begin && (*it).type == TokenType::TokenComment)
            {
               tokenStack.push((*it).token);
            }
            break;
        }
    }

    // Store head comment.
    while(!tokenStack.empty())
    {
        pEnumClass->comment.headComment += tokenStack.top();
        tokenStack.pop();
    }

    // Find "{"
    while(it2 != end && (*it2).type != TokenType::TokenLB)
    {
        ++it2;
    }

    // Find first key.
    while(it2 != end && (*it2).type != TokenType::TokenCName)
    {
        ++it2;
    }
    if(it2 == end || (*it2).type != TokenType::TokenCName) return;

    //
    // First key position now, start parsing key's comments.
    //
    // It is the first possiblity.
    //
    // If it can find next comma and next key,
    // then get the comments between next comma and next key.
    //
    auto findNextCommentWithComma = [&](std::string& comment)
    {
        comment.clear();

        if(it2 == end) return it2;

        auto itComma = it2;
        while(itComma != end && (*itComma).type != TokenType::TokenComma)
        {
            ++itComma;
        }

        auto itKey = itComma;
        while(itKey != end && (*itKey).type != TokenType::TokenCName)
        {
            ++itKey;
        }

        if(itComma != end && itKey != end && itComma != itKey)
        {
            for(auto itCmt = itComma+1; itCmt != itKey; ++itCmt)
            {
                if((*itCmt).type == TokenType::TokenComment)
                {
                    comment += (*itCmt).token;
                }
            }
        }
        return itKey;
    };

    //
    // It is the second possiblity.
    //
    // No more next comma or next key,
    // so it is the last key or the only one key,
    // then get the comments to the end of "}".
    //
    auto findNextCommentWithRB = [&](std::string& comment)
    {
        comment.clear();
        if(it2 == end) return it2;

        auto itRB = it2;
        while(itRB != end && (*itRB).type != TokenType::TokenRB)
        {
            ++itRB;
        }

        if(itRB != end && (*itRB).type == TokenType::TokenRB && itRB != it2)
        {
            for(auto itCmt = it2+1; itCmt!=itRB; ++itCmt)
            {
                if((*itCmt).type == TokenType::TokenComment)
                {
                    comment += (*itCmt).token;
                }
            }
        }
        return end;
    };

    // Start get the all the keys comments here.
    while(it2!=end)
    {
        std::string comment;

        // Try the first possiblity.
        auto pos  = findNextCommentWithComma(comment);
        if(pos != end)
        {
            pEnumClass->comment.keyComments.push_back(comment);
        }
        else
        {
            // Try the second possiblity.
            findNextCommentWithRB(comment);
            pEnumClass->comment.keyComments.push_back(comment);
            break;
        }

        it2 = pos;
    }

    if(pEnumClass->keys.size() != pEnumClass->comment.keyComments.size())
    {
        print_info("CecEnumParser, parse code comment failed.\n");
        pEnumClass->comment.keyComments.clear();
    }
}

std::shared_ptr<EnumClass> CecEnumParser::parse(const std::string &stream)
{
    using TokenType = CecEnumLexer::TokenType;
    std::set<std::string> enumClassKeySet;
    CecEnumLexer::Context ctx;
    ctx.curser = reinterpret_cast<const uint8_t*>(stream.data());
    auto startCurser = ctx.curser;
    ctx.limit = reinterpret_cast<const uint8_t*>(stream.data()) + stream.size();
    TokenType tokenType = CecEnumLexer::TokenType::LexerError;
    int state = 0;
    bool good = false;
    std::shared_ptr<EnumClass> pEnumClass (new EnumClass);

    auto actionForState7or8 = [&]()
    {
        // In this state, it should meet more ",key" or "}"
        auto tokenType = CecEnumLexer::lex(ctx);
        bool good = false;
        if (tokenType == TokenType::TokenComma)
        {
            tokenType = CecEnumLexer::lex(ctx);
            if (tokenType == TokenType::TokenCName 
                && eatEnumClassKey(pEnumClass,ctx.token(),enumClassKeySet))
            {
                state = 8;
                good = true;
            }
        }
        else if (tokenType == TokenType::TokenRB)
        {
            state = 9;
            good = true;
        }
        return good;
    };
    
    auto dumpErrorState = [&]()
    {
        print_error("EnumParser dump begin--\n");
        print_error("  current state = ", state, "\n");
        print_error("  file position = ", ctx.curser-startCurser,
                    "  ## Please check source file at this position.\n");
        print_error("EnumParser dump end----\n");
    };


    while (true)
    {
        switch (state)
        {
        case 0: // beginning.
        {
            tokenType = CecEnumLexer::lex(ctx);
            good = false;
            if (tokenType == TokenType::TokenEnum) // "enum"
            {
                state = 1;
                good = eatEnumClassFullNamePart1(pEnumClass, ctx.token());
            }
            else if(tokenType == TokenType::TokenSemicolon)
            {
                state = 0;
                good = true;
            }
            if (!good)
            {
                dumpErrorState();
                return {};
            }

            break;
        }
        case 1: // "enum"
        {
            tokenType = CecEnumLexer::lex(ctx);
            good = false;
            if (tokenType == TokenType::TokenClass) // "struct" | "class"
            {
                state = 2;
                good = eatEnumClassFullNamePart2(pEnumClass, ctx.token());
            }
            else if (tokenType == TokenType::TokenCName)
            {
                state = 3;
                good = eatEnumClassName(pEnumClass,ctx.token());
                if(good)
                {
                    good = eatEnumClassFullNamePart3(pEnumClass, ctx.token());
                }
            }
            if (!good)
            {
                dumpErrorState();
                return {};
            }
                
            break;
        }
        case 2: // "class | struct"
        {
            tokenType = CecEnumLexer::lex(ctx);
            good = false;
            if (tokenType == TokenType::TokenCName)
            {
                state = 3;
                good = eatEnumClassName(pEnumClass,ctx.token());
                if(good)
                {
                    good = eatEnumClassFullNamePart3(pEnumClass, ctx.token());
                }
            }
            if (!good)
            {
                dumpErrorState();
                return {};
            }
                
            break;
        }
        case 3: // "name"
        {
            tokenType = CecEnumLexer::lex(ctx);
            good = false;
            if (tokenType == TokenType::TokenLB)
            {
                state = 5;
                good = true;
            }
            else if (tokenType == TokenType::TokenColon)
            {
                tokenType = CecEnumLexer::lex(ctx);
                if (tokenType == TokenType::TokenCName)
                {
                    state = 4;
                    good = eatEnumClassScalarType(pEnumClass,ctx.token());
                    // I think it may be redundant, 
                    // because the type will be collected serparately.
                    //
                    // if(good)
                    // {
                    //     good = eatEnumClassFullNamePart4(pEnumClass, ctx.token());
                    // }
                }
            }
            if (!good)
            {
                dumpErrorState();
                return {};
            }
                
            break;
        }
        case 4: // ":type"
        {
            tokenType = CecEnumLexer::lex(ctx);
            good = false;
            if (tokenType == TokenType::TokenLB)
            {
                state = 5;
                good = true;
            }
            if (!good)
            {
                dumpErrorState();
                return {};
            }
                
            break;
        }
        case 5: // "{"
        {
            tokenType = CecEnumLexer::lex(ctx);
            good = false;
            if (tokenType == TokenType::TokenCName && 
                eatEnumClassKey(pEnumClass,ctx.token(),enumClassKeySet))
            {
                state = 6;
                good = true;
            }
            if (!good)
            {
                dumpErrorState();
                return {};
            }
                
            break;
        }
        case 6: // "first key"
        {
            tokenType = CecEnumLexer::lex(ctx);
            good = false;
            if (tokenType == TokenType::TokenRB)
            {
                state = 9;
                good = true;
            }
            else if (tokenType == TokenType::TokenEq)
            {
                tokenType = CecEnumLexer::lex(ctx);
                if (tokenType == TokenType::TokenNumber 
                    && eatEnumClassNumber(pEnumClass,ctx.token()))
                {
                    // Eat first key's number success.
                    state = 7;
                    good = true;
                }
            }
            else if (tokenType == TokenType::TokenComma)
            {
                tokenType = CecEnumLexer::lex(ctx);
                if (tokenType == TokenType::TokenCName 
                    && eatEnumClassKey(pEnumClass,ctx.token(),enumClassKeySet))
                {
                    state = 8;
                    good = true;
                }
            }
            if (!good)
            {
                dumpErrorState();
                return {};
            }
                
            break;
        }
        case 7: // ":number"
        {
            if (!actionForState7or8())
            {
                dumpErrorState();
                return {};
            }
                
            break;
        }
        case 8: // ",key"
        {
            if (!actionForState7or8())
            {
                dumpErrorState();
                return {};
            }
                
            break;
        }
        case 9: // "}"
        {
            // Stop parsing here.
            handleEnumClassComment(pEnumClass, ctx);
            if(doPostWork(pEnumClass) == true)
            {
                return pEnumClass;
            }
            else
            {
                return {};
            }
        }
        default:
        {
            dumpErrorState();
            return {};
        }
        } // switch-end
    } // while-end

    return {};
}

bool CecEnumParser::eatEnumClassName(std::shared_ptr<EnumClass> pEnumClass,
                        const std::string& token)
{
    pEnumClass->name = token;
    return true;
}

bool CecEnumParser::eatEnumClassFullNamePart1(std::shared_ptr<EnumClass> pEnumClass,
                            const std::string &token)
{
    // "enum"
    pEnumClass->fullName = token;
    return true;
}
bool CecEnumParser::eatEnumClassFullNamePart2(std::shared_ptr<EnumClass> pEnumClass,
                            const std::string &token)
{
    // "class" | "struct"
    pEnumClass->fullName += " " + token;
    return true;
}
bool CecEnumParser::eatEnumClassFullNamePart3(std::shared_ptr<EnumClass> pEnumClass,
                            const std::string &token)
{
    // "name"
    pEnumClass->fullName += " " + token;
    return true;
}
bool CecEnumParser::eatEnumClassFullNamePart4(std::shared_ptr<EnumClass> pEnumClass,
                            const std::string &token)
{
    // "type"
    pEnumClass->fullName += ":" + token;
    return true;
}

bool CecEnumParser::eatEnumClassScalarType(std::shared_ptr<EnumClass> pEnumClass,
                        const std::string& token)
{
    pEnumClass->type = token;
    return true;
}

bool CecEnumParser::eatEnumClassKey(std::shared_ptr<EnumClass> pEnumClass,
                        const std::string& token,
                        std::set<std::string>& enumClassKeySet)
{
    auto result = enumClassKeySet.insert(token);
    if(result.second == true)
    {
        pEnumClass->keys.push_back(token);
    }
    else
    {
        print_error("find duplicated key = ", token, "\n");
    }
    return result.second;
}

bool CecEnumParser::eatEnumClassNumber(std::shared_ptr<EnumClass> pEnumClass,
                        const std::string& token)                    
{
    bool good = true;
    int64_t number = 0;

    if(CecEnumLexer::isValidDecNumber(reinterpret_cast<const uint8_t*>(token.c_str()), token.size()))
    {
        number = toNumber(token, 10, good);
    }
    else if(CecEnumLexer::isValidHexNumber(reinterpret_cast<const uint8_t*>(token.c_str()), token.size()))
    {
        number = toNumber(token, 16, good);
    }
    else
    {
        good = false;
    }

    if(good)
    {
        pEnumClass->number = number;
    }
    else
    {
        print_error("EnumParser: given enum number is out of range.\n");
    }

    return good;
}

bool CecEnumParser::doPostWork(std::shared_ptr<EnumClass> pEnumClas)
{
    if(pEnumClas->type.empty())
    {
        // The type's value range will be checked by CecEnumChecker.
        pEnumClas->type = config::EnumScalarTypeDefault;
    }

    return true;
}

int64_t CecEnumParser::toNumber(const std::string &token,
                                const int radix,
                                bool &result)
{
    auto number = strtoll(token.c_str(), nullptr, radix);
    if (errno == ERANGE)
    {
        result = false;
        number = 0;
    }

    result = true;
    return number;
}
