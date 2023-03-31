/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : expr.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __expr_h_
#define __expr_h_

#include <format>
#include <optional>
#include <math.h>

#include "variable.h"
#include "file.h"

namespace parser
{
  class expr
  {
  public:
    std::string Text;
    virtual double Eval(void) = 0;

    virtual ~expr() {}
  };

  class num_expr : public expr
  {
  private:
    double Val;
  public:
    num_expr(double NewVal) : Val(NewVal)
    {}

    double Eval(void) override
    {
      Text = std::to_string(Val);
      return Val;
    }
    ~num_expr()
    {}
  };

  class bin_expr : public expr
  {
  private:
    expr* E1, * E2;
    char Oper;
  public:
    bin_expr(char O, expr* E1, expr* E2)
    {
      this->E1 = E1;
      this->E2 = E2;
      this->Oper = O;
    }

    ~bin_expr()
    {
      delete E1;
      delete E2;
    }

    double Eval(void) override
    {
      double res;
      switch (Oper)
      {
      case '+':
        res = E1->Eval() + E2->Eval();
        Text = "(" + E1->Text + "+" + E2->Text + ")";
        return res;
      case '-':
        res = E1->Eval() - E2->Eval();
        Text = "(" + E1->Text + "-" + E2->Text + ")";
        return res;
      case '*':
        res = E1->Eval() * E2->Eval();
        Text = "(" + E1->Text + "*" + E2->Text + ")";
        return res;
      case '/':
        res = E1->Eval() / E2->Eval();
        Text = "(" + E1->Text + "/" + E2->Text + ")";
        return res;
      default:
        return 0;
      }
    }
  };

  class cond_expr : public expr
  {
  private:
    expr* E1, * E2;
  public:
    enum oper_type
    {
      eEQ,
      eNOTEQ,

      eLT,
      eLTEQ,
      eGT,
      eGTEQ,

      eAND,
      eOR
    } Type;

    cond_expr(oper_type O, expr* E1, expr* E2)
    {
      this->E1 = E1;
      this->E2 = E2;
      this->Type = O;
    }

    ~cond_expr()
    {
      delete E1;
      delete E2;
    }

    double Eval(void) override
    {
      double res;
      switch (Type)
      {
      case oper_type::eLT:
        res = E1->Eval() < E2->Eval();
        Text = "(" + E1->Text + "<" + E2->Text + ")";
        return res;
      case oper_type::eGT:
        res = E1->Eval() > E2->Eval();
        Text = "(" + E1->Text + ">" + E2->Text + ")";
        return res;
      case oper_type::eLTEQ:
        res = E1->Eval() <= E2->Eval();
        Text = "(" + E1->Text + "<=" + E2->Text + ")";
        return res;
      case oper_type::eGTEQ:
        res = E1->Eval() >= E2->Eval();
        Text = "(" + E1->Text + ">=" + E2->Text + ")";
        return res;
      case oper_type::eEQ:
        res = E1->Eval() == E2->Eval();
        Text = "(" + E1->Text + "==" + E2->Text + ")";
        return res;
      case oper_type::eNOTEQ:
        res = E1->Eval() != E2->Eval();
        Text = "(" + E1->Text + "!=" + E2->Text + ")";
        return res;
      case oper_type::eAND:
        res = E1->Eval() && E2->Eval();
        Text = "(" + E1->Text + "&&" + E2->Text + ")";
        return res;
      case oper_type::eOR:
        res = E1->Eval() || E2->Eval();
        Text = "(" + E1->Text + "||" + E2->Text + ")";
        return res;
      default:
        return 0;
      }
    }
  };


  class un_expr : public expr
  {
  private:
    expr* E;
    char Oper;
  public:
    un_expr(char O, expr* E)
    {
      this->E = E;
      this->Oper = O;
    }

    ~un_expr()
    {
      delete E;
    }

    double Eval(void) override
    {
      double res;
      switch (Oper)
      {
      case '-':
        res = -E->Eval();
        Text = "-" + E->Text;
        return res;
      case '+':
        return E->Eval();
      default:
        return 0;
      }
    }
  };

  class const_expr : public expr
  {
  private:
    std::string Name;
  public:
    const_expr(std::string Name) : Name(Name)
    {
    }

    ~const_expr()
    {
      Name.clear();
    }

    double Eval(void) override
    {
      Text = Name;
      return variables::Get(Name).Val;
    }
  };

  class shape_expr : public expr
  {
  private:
    std::vector<std::string> Params;
    obj::shape::type Type;
    std::string Var;
    bool IsTex;

  public:
    shape_expr(std::string VarName, obj::shape::type Type, std::vector<std::string> Args, bool IsTex) :
      Var(VarName), Type(Type), IsTex(IsTex)
    {
      Params = std::move(Args);
    }

    ~shape_expr()
    {
    }

    double Eval(void) override
    {
      std::string tmp = std::format("{}", obj::shape::ToStr.at(Type)(Var, Params, IsTex));

      file::Print(std::format("// apply SDF function to '{}'", Var));
      file::Print(tmp);
      variables::SetShape(Var, tmp);

      return 0;
    }
  };

  class mod_expr : public expr
  {
  private:
    std::vector<std::string> Params;
    obj::mod::type Type;
    std::string Var;

  public:
    mod_expr(std::string VarName, obj::mod::type Type, std::vector<std::string> Args) :
      Var(VarName), Type(Type)
    {
      Params = std::move(Args);
    }

    ~mod_expr()
    {}

    double Eval(void) override
    {
      file::Print(std::format("// apply modification function to '{}'", Var));
      file::Print(std::format("{}", obj::mod::ToStr.at(Type)(Var, Params)));
      return 0;
    }
  };

  class oper_expr : public expr
  {
  private:
    obj::oper::type Type;
    std::string Var, P1, P2;
    std::optional<std::string> K;

  public:
    oper_expr(std::string VarName, obj::oper::type Type, std::vector<std::string> Args) :
      Var(VarName), Type(Type), P1(P1), P2(P2), K(K)
    {
      int s = (int)Args.size();

      if (s < 2 || s > 3)
        throw std::exception("incorrect count of parameters!");
      P1 = Args[0];
      P2 = Args[1];

      if (s == 3)
        K = Args[2];

      if (!variables::IsExists(Var) && (Var == P1 || Var == P2))
        throw std::exception("incorrect parameters!");
    }

    ~oper_expr()
    {}

    double Eval(void) override
    {
      file::Print(std::format("// apply operation to '{}'", Var));
      file::Print(std::format("{}", obj::oper::ToStr.at(Type)(Var, P1, P2, K)));
      return 0;
    }
  };

  class light_expr : public expr
  {
  private:
    std::vector<std::string> Params;
    obj::light::type Type;
    std::string Var;

  public:
    light_expr(std::string VarName, obj::light::type Type, std::vector<std::string> Args) :
      Var(VarName), Type(Type)
    {
      Params = std::move(Args);
    }

    ~light_expr()
    {}

    double Eval(void) override
    {
      obj::light::Add.at(Type)(Var, Params);
      return 0;
    }
  };

  class sin_expr : public expr
  {
  private:
    expr* E;

  public:
    sin_expr(expr* E) : E(E)
    {
    }

    ~sin_expr()
    {
      delete E;
    }

    double Eval(void) override
    {
      double val = E->Eval();
      Text = std::format("sin({0})", E->Text);
      return sin(val);
    }
  };

  class cos_expr : public expr
  {
  private:
    expr* E;

  public:
    cos_expr(expr* E) : E(E)
    {
    }

    ~cos_expr()
    {
      delete E;
    }

    double Eval(void) override
    {
      double val = E->Eval();
      Text = std::format("cos({0})", E->Text);
      return cos(val);
    }
  };

  class abs_expr : public expr
  {
  private:
    expr* E;

  public:
    abs_expr(expr* E) : E(E)
    {
    }

    ~abs_expr()
    {
      delete E;
    }

    double Eval(void) override
    {
      double val = E->Eval();
      Text = std::format("abs({0})", E->Text);
      return fabs(val);
    }
  };

  class fmod_expr : public expr
  {
  private:
    expr* E1, * E2;

  public:
    fmod_expr(expr* E1, expr* E2) : E1(E1), E2(E2)
    {
    }

    ~fmod_expr()
    {
      delete E1;
      delete E2;
    }

    double Eval(void) override
    {
      double
        val1 = E1->Eval(),
        val2 = E2->Eval();
      Text = std::format("mod({0}, {1})", E1->Text, E2->Text);
      return fmod(val1, val2);
    }
  };

  class vec_expr : public expr
  {
  private:
    expr* A, * B, * C;
    int s;

  public:
    vec_expr(std::vector<expr*> Args)
    {
      s = (int)Args.size();

      if (s == 1)
        A = B = C = Args[0];
      else if (s == 3)
      {
        A = Args[0];
        B = Args[1];
        C = Args[2];
      }
      else
        throw std::exception("incorrect count of parameters!");
    }
    ~vec_expr()
    {
      if (s == 1)
        delete A;
      else if (s == 3)
      {
        delete A;
        delete B;
        delete C;
      }
      
    }

    double Eval(void) override
    {
      double
        a = A->Eval(),
        b = B->Eval(),
        c = C->Eval();
      Text = std::format("vec3({0}, {1}, {2})", A->Text, B->Text, C->Text);
      return 0;
    }
  };
}

#endif

/* END OF 'expr.h' FILE */