/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : lexer.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __lexer_h_
#define __lexer_h_

#include <algorithm>
#include <map>

#include "obj/obj.h"

#include "token.h"

namespace parser
{
  class lexer
  {
  private:
    /* Some constants */
    std::map<std::string, token_type> OPERS =
    {
      {"+", token_type::ePlus},
      {"-", token_type::eMinus},
      {"*", token_type::eStar},
      {"/", token_type::eSlash},
      {"(", token_type::eLParen},
      {")", token_type::eRParen},
      {"=", token_type::eEQ},
      {"<", token_type::eLT},
      {">", token_type::eGT},
      {"{", token_type::eLBRACE},
      {"}", token_type::eRBRACE},
      {"[", token_type::eLBracket},
      {"]", token_type::eRBracket},
      {",", token_type::eSemicolon},
      {"\"", token_type::eCav},

      {"!", token_type::eEXCL},
      {"|", token_type::eBAR},
      {"&", token_type::eAMP},

      {"==", token_type::eEQEQ},
      {"!=", token_type::eEXCLEQ},
      {"<=", token_type::eLTEQ},
      {">=", token_type::eGTEQ},

      {"&&", token_type::eAMPAMP},
      {"||", token_type::eBARBAR},
    };

    std::string Input;
    std::vector<token> Tokens;
    int CurPos, Len;

    void AddToken(std::string Text, token_type Type)
    {
      Tokens.push_back(token(Text, Type));
    }

    char Peek(int RelPos)
    {
      int pos = CurPos + RelPos;

      if (pos >= Len)
        return '\0';

      return Input[pos];
    }

    char Next(void)
    {
      CurPos++;
      return Peek(0);
    }

    void TokenizeNumber(void)
    {
      std::string res;
      char cur = Peek(0);

      while (true)
      {
        if (cur == '.')
        {
          if (res.find('.') != std::string::npos)
            throw std::exception("incorrect number");
        }
        else if (!isdigit(cur))
          break;
        res += cur;
        cur = Next();
      }

      AddToken(res, token_type::eNumber);
    }

    void TokenizeWord(void)
    {
      std::string res;
      char cur = Peek(0);

      while (true)
      {
        if (!(isalpha(cur) || isdigit(cur) || cur == '_'))
          break;
        res += cur;
        cur = Next();
      }

      if (TYPES.find(res) != TYPES.end())
        AddToken(res, token_type::eType);
      else if (obj::shape::Table.find(res) != obj::shape::Table.end())
        AddToken(res, token_type::eFunc);
      else if (obj::mod::Table.find(res) != obj::mod::Table.end())
        AddToken(res, token_type::eFunc);
      else if (obj::light::Table.find(res) != obj::light::Table.end())
        AddToken(res, token_type::eFunc);
      else if (obj::oper::Table.find(res) != obj::oper::Table.end())
        AddToken(res, token_type::eFunc);
      else if (res == "print")
        AddToken("", token_type::ePrint);
      else if (res == "if")
        AddToken("", token_type::eIf);
      else if (res == "else")
        AddToken("", token_type::eElse);
      else if (res == "while")
        AddToken("", token_type::eWhile);
      else if (res == "add")
        AddToken("", token_type::eAdd);
      else if (res == "for")
        AddToken("", token_type::eFor);
      else
        AddToken(res, token_type::eWord);
    }

    void TokenizeOperator(void)
    {
      char cur = Peek(0);

      if (cur == '/')
      {
        if (Peek(1) == '/')
        {
          Next();
          Next();
          TokenizeComment();
          return;
        }
        else if (Peek(1) == '*')
        {
          Next();
          Next();
          TokenizeMultiComment();
          return;
        }
      }
      std::string cur_str;

      while (true)
      {
        if (OPERS.find(cur_str + cur) == OPERS.end() && !cur_str.empty())
        {
          AddToken(cur_str, OPERS.find(cur_str)->second);
          return;
        }
        cur_str += cur;
        cur = Next();
      }
    }

    void TokenizeComment(void)
    {
      char cur = Peek(0);

      while (cur != '\n' && cur != '\r' && cur != '\0')
        cur = Next();
    }

    void TokenizeMultiComment(void)
    {
      char cur = Peek(0);

      while (true)
      {
        if (cur == '\0')
          throw std::exception("missing close tag!");
        if (cur == '*' && Peek(1) == '/')
          break;
        cur = Next();
      }
      Next();
      Next();
    }

    bool Check(void)
    {
      int lp, rp, lb, rb;
      lp = rp = lb = rb = 0;

      for (auto& t : Tokens)
      {
        if (t.Type == token_type::eLParen)
          lp++;
        if (t.Type == token_type::eRParen)
          rp++;
        if (t.Type == token_type::eLBRACE)
          lb++;
        if (t.Type == token_type::eRBRACE)
          rb++;
      }

      return lp == rp && lb == rb;
    }

  public:
    lexer(std::string NewInput) : Input(NewInput), CurPos(0), Len((int)Input.size())
    {
    }

    std::vector<token> Tokenize(void)
    {
      while (CurPos < Len)
      {
        char cur = Peek(0);
        std::string cur_str;
        cur_str += cur;

        if (isdigit(cur))
          TokenizeNumber();
        else if (isalpha(cur))
          TokenizeWord();
        else if (OPERS.find(cur_str) != OPERS.end())
          TokenizeOperator();
        else
          Next();
      }

      if (!Check())
        throw std::exception("incorrect count of tags");

      return Tokens;
    }
  };
}

#endif

/* END OF 'lexer.h' FILE */