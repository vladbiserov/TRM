/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : file.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __file_h_
#define __file_h_

#include <fstream>
#include <string>
#include <sstream>
#include <format>

namespace parser
{
  class file
  {
  private:
    static std::string CurBuf;

    file() {}
    ~file()
    {
      CurBuf.clear();
    }
  public:
    static void Print(const std::string& Data)
    {
      CurBuf += Data + "\n";
    }

    static void PrintFile(const std::string& InName, const std::string& OutName,
      const std::string& LgtBuf, const std::string& TexBuf)
    {
      std::ofstream FOut(OutName);
      std::ifstream FIn(InName);

      if (!FIn.is_open() || !FOut.is_open())
        throw std::exception("incorrect file!");

      std::string line;

      while (getline(FIn, line))
      {
        if (line == "SCENE")
          FOut << CurBuf;
        else if (line == "TEXTURE")
          FOut << TexBuf;
        else if (line == "LIGHT")
          FOut << LgtBuf;
        else
          FOut << line << std::endl;
      }

      FOut.close();
      FIn.close();
      CurBuf.clear();
    }

    static std::string ReadFile(const std::string& Name)
    {
      std::ifstream F(Name);
      std::string Buf;

      std::stringstream strStream;
      strStream << F.rdbuf();
      Buf = strStream.str();
      F.close();

      return Buf;
    }
  };
}

#endif

/* END OF 'file.h' FILE */