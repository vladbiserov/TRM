/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : statement.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __statement_h_
#define __statement_h_

#include <format>
#include "expr.h"

namespace parser
{
  class statement
  {
  public:
    virtual void Execute(void) = 0;

    virtual ~statement() {}
  };

  class assign_statement : public statement
  {
  private:
    std::string Var;
    var_type Type;
    expr* Expr;

  public:
    assign_statement(var_type Type, std::string Var, expr* Expr, std::string Text) : Type(Type), Var(Var), Expr(Expr)
    {
      if (!variables::IsExists(Var))
        switch (Type)
        {
        case var_type::eInt:
          file::Print(std::format("// add int '{0}'\n"
            "int {0};\n", Var));
          break;
        case var_type::eFloat:
          file::Print(std::format("// add float '{0}'\n"
            "float {0};\n", Var));
          break;
        case var_type::eVec:
          file::Print(std::format("// add vec3 '{0}'\n"
            "vec3 {0};\n", Var));
          break;
        case var_type::eMtl:
          file::Print(std::format("// add mtl '{0}'\n"
            "mtl {0};\n", Var));
          break;
        case var_type::eShape:
          file::Print(std::format("// add shape '{0}'\n"
            "float {0};\n"
            "mtl mtl_{0};\n"
            "vec3 mod_{0} = point;\n"
            "vec2 tex_{0};\n", Var));
          break;
        default:
          break;
        }
      variables::Set(Var, { Type, 0, Text });
    }

    ~assign_statement()
    {
      delete Expr;
    }

    void Execute(void) override
    {
      double val = Expr->Eval();

      if (Type == var_type::eInt)
        val = (int)val;

      variables::Set(Var, { Type, val, Expr->Text });

      if (Type == var_type::eFloat)
        file::Print(std::format("// set float value to '{0}'\n{0} = {1};\n", Var, variables::Get(Var).Text));
      else if (Type == var_type::eInt)
        file::Print(std::format("// set int value to '{0}'\n{0} = int({1});\n", Var, variables::Get(Var).Text));
      else if (Type == var_type::eVec)
        file::Print(std::format("// set vec value to '{0}'\n{0} = {1};\n", Var, variables::Get(Var).Text));
      else if (Type == var_type::eMtl)
        file::Print(std::format("// set mtl value to '{0}'\n{0} = {1};\n", Var, variables::Get(Var).Text));
    }
  };

  class if_statement : public statement
  {
  private:
    expr* Expr;
    statement* If, * Else;

  public:
    if_statement(expr* Expr, statement* A, statement* B) : Expr(Expr), If(A), Else(B)
    {
    }

    ~if_statement()
    {
      delete Expr;
      delete If;
      delete Else;
    }

    void Execute(void) override
    {
      double val = Expr->Eval();

      if (val)
        If->Execute();
      else if (Else != nullptr)
        Else->Execute();
    }
  };

  class block_statement : public statement
  {
  private:
    std::vector<statement*> St;

  public:
    block_statement()
    {
    }

    ~block_statement()
    {
      for (auto &s : St)
        delete s;
    }

    void Add(statement* S)
    {
      St.emplace_back(S);
    }

    void Execute(void) override
    {
      for (auto& s : St)
        s->Execute();
    }
  };

  class add_statement : public statement
  {
  private:
    std::map<std::string, var_type> St;

  public:
    add_statement()
    {
    }

    ~add_statement()
    {
    }

    void Add(std::string S)
    {
      auto a = variables::Get(S);
      if (a.Type != var_type::eLight && a.Type != var_type::eShape)
        throw std::exception("can't add object this type to scene!");
      St[S] = a.Type;
    }

    void Execute(void) override
    {
      for (auto& s : St)
      {
        if (s.second == var_type::eLight)
        {
          obj::light::Enable(s.first);
          continue;
        }
        if (variables::IsFirst)
        {
          file::Print(std::format("// add to scene '{0}' var\n"
            "res = {0};\n"
            "Mtl = mtl_{0};\n", s.first));
          variables::IsFirst = false;
        }
        else
          file::Print(std::format("// add to scene '{0}' var\n"
            "tmp = res;\n"
            "tmp_mtl = Mtl;\n"
            "res = SDFUnion(tmp, {0});\n"
            "if (res == {0})\n"
            "  Mtl = mtl_{0};\n"
            "else if (res == tmp)\n"
            "  Mtl = tmp_mtl;\n"
            "else\n"
            "  Mtl = SDFSurfaceSmoothUnion(tmp, tmp_mtl, {0}, mtl_{0}, 0.5);\n", s.first));
      }
    }
  };

  class while_statement : public statement
  {
  private:
    statement* Statement;
    expr* Condition;

  public:
    while_statement(expr* Ex, statement* St) : Statement(St), Condition(Ex)
    {
    }

    ~while_statement()
    {
      delete Statement;
      delete Condition;
    }

    void Execute(void) override
    {
      while (Condition->Eval() != 0)
        Statement->Execute();
    }
  };

  class for_statement : public statement
  {
  private:
    statement* Init;
    expr* Term;

    statement* Incr, * Block;

  public:
    for_statement(statement* In, expr* Ter, statement* Incr, statement* B) : Init(In), Term(Ter), Incr(Incr), Block(B)
    {
    }

    ~for_statement()
    {
      delete Init;
      delete Term;
      delete Incr;
      delete Block;
    }

    void Execute(void) override
    {
      for (Init->Execute(); Term->Eval() != 0; Incr->Execute())
        Block->Execute();
    }
  };

  class state_statement : public statement
  {
  private:
    state_type Type;
    bool Val;

  public:
    state_statement(state_type Type, bool Val) : Type(Type), Val(Val)
    {
    }

    ~state_statement()
    {
    }

    void Execute(void) override
    {
      variables::Flags[Type] = Val;
    }
  };
}

#endif

/* END OF 'statement.h' FILE */