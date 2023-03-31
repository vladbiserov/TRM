/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : token.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __token_h_
#define __token_h_

#include <string>
#include <vector>
#include <exception>
#include <map>
#include <functional>
#include <format>
#include <optional>

namespace parser
{
  enum class var_type
  {
    eInt,
    eFloat,
    eVec,
    eShape,
    eLight
  };

  extern std::map<std::string, var_type> TYPES;

  enum class token_type
  {
    eNumber,
    eWord,
    eType,
    eFunc,

    ePrint,
    eIf,
    eElse,
    eWhile,
    eFor,
    eSemicolon,
    eAdd,

    ePlus,
    eMinus,
    eStar,
    eSlash,
    eEQ,
    eEQEQ,
    eEXCL,
    eEXCLEQ,
    eLT,
    eLTEQ,
    eGT,
    eGTEQ,

    eBAR,
    eBARBAR,
    eAMP,
    eAMPAMP,

    eLParen,
    eRParen,
    eLBracket,
    eRBracket,
    eLBRACE,
    eRBRACE,
    eCav,

    eEOF
  };

  class token
  {
  public:
    std::string Text;
    token_type Type;

    token(std::string NewText, token_type NewType) : Text(NewText), Type(NewType)
    {
    }
  };
}

#endif

/* END OF 'token.h' FILE */