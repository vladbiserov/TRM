/*************************************************************
 * Copyright (C) 2022-2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : parser.h
  * PURPOSE     : Ray marching project.
  *               Parser module.
  * PROGRAMMER  : Vladislav Biserov.
  * LAST UPDATE : 30.03.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __parser_h_
#define __parser_h_

#include "lexer.h"
#include "statement.h"

namespace parser
{
  class parser
  {
  private:
    token END = token("", token_type::eEOF);
    std::vector<token> Tokens;
    int CurPos, Size;

    token Get(int RelPos)
    {
      int pos = CurPos + RelPos;

      if (pos >= Size)
        return END;

      return Tokens[pos];
    }

    bool Match(token_type Type)
    {
      token cur = Get(0);

      if (cur.Type != Type)
        return false;
      CurPos++;
      return true;
    }

    token Consume(token_type Type)
    {
      token cur = Get(0);
      if (Type != cur.Type)
        throw std::exception("token  doesn't match type!");
      CurPos++;
      return cur;
    }

    token Next(void)
    {
      token cur = Get(0);
      CurPos++;
      return cur;
    }

    void TexExpr(void)
    {
      Consume(token_type::eCav);  // "
      Consume(token_type::eWord); // name
      token ext = Get(0);
      Consume(token_type::eWord); // ext
      if (ext.Text != "png" && ext.Text != "jpg" && ext.Text != "tga" && ext.Text != "bmp")
        throw std::exception("incorrect texture extension!");
      Consume(token_type::eCav);  // "
    }

    void MatExpr(void)
    {
      token cur = Get(0);

      if (cur.Text != "MtlLib")
        throw std::exception("incorrect material");

      Consume(token_type::eWord);       // MtlLib
      Consume(token_type::eLBracket);   // [
      expr *e = Expr();                           // num
      Consume(token_type::eRBracket);   // ]

      delete e;
    }

    void ShpExpr(void)
    {
      token cur = Get(0);

      if (!variables::IsExists(cur.Text) || variables::Get(cur.Text).Type != var_type::eShape)
        throw std::exception("incorrect type of variable!");

      Consume(token_type::eWord);
    }

    expr* Func(std::string VarName)
    {
      token cur = Get(0);
      std::vector<std::string> par;
      std::vector<param::type> types;
      std::string name;
      bool isTex = true;

      enum f_type
      {
        eShape,
        eMod,
        eOper,
        eLight,
      } ftype;

      if (obj::shape::Table.find(cur.Text) != obj::shape::Table.end())
      {
        ftype = f_type::eShape;
        types = obj::shape::Types.at(obj::shape::Table.at(cur.Text));
      }
      else if (obj::mod::Table.find(cur.Text) != obj::mod::Table.end())
      {
        ftype = f_type::eMod;
        types = obj::mod::Types.at(obj::mod::Table.at(cur.Text));
      }
      else if (obj::oper::Table.find(cur.Text) != obj::oper::Table.end())
      {
        ftype = f_type::eOper;
        types = obj::oper::Types.at(obj::oper::Table.at(cur.Text));
      }
      else if (obj::light::Table.find(cur.Text) != obj::light::Table.end())
      {
        ftype = f_type::eLight;
        types = obj::light::Types.at(obj::light::Table.at(cur.Text));
      }
      else
        throw std::exception("no such function!");

      name = cur.Text;

      Consume(token_type::eFunc);   // func
      Consume(token_type::eLParen); // (

      int
        ind = 0,
        size = (int)types.size(); // Current argument index

      while (!Match(token_type::eRParen))
      {
        if (ind >= size)
          throw std::exception("incorrect count of parameters!");

        int p = CurPos;
        std::string tmp = "";
        expr *e = nullptr;

        switch (types[ind])
        {
        case param::type::eNum:
          e = Expr();
          delete e;
          break;
        case param::type::eTex:
          TexExpr();
          break;
        case param::type::eVec:
          e = VecExpr();
          delete e;
          break;
        case param::type::eMat:
          MatExpr();
          break;
        case param::type::eShp:
          ShpExpr();
          break;
        default:
          throw std::exception("incorrect parameter type!");
        }

        for (int i = p; i < CurPos; i++)
          tmp += Tokens[i].Text;

        par.emplace_back(tmp);

        Match(token_type::eSemicolon);
        ind++;
      }

      if (ftype == f_type::eShape && ind == size - 1)
        isTex = false;
      else if (ind != size)
        throw std::exception("incorrect count of parameters!");

      if (ftype == f_type::eShape)
        return new shape_expr(VarName, obj::shape::Table.find(name)->second, par, isTex);
      if (ftype == f_type::eMod)
        return new mod_expr(VarName, obj::mod::Table.find(name)->second, par);
      if (ftype == f_type::eOper)
        return new oper_expr(VarName, obj::oper::Table.find(name)->second, par);
      if (ftype == f_type::eLight)
        return new light_expr(VarName, obj::light::Table.find(name)->second, par);

      throw std::exception("incorrect function!");
    }

    expr* VecExpr(void)
    {
      return VecAddExpr();
    }

    expr* VecAddExpr(void)
    {
      expr* exp = VecMultiExpr();

      while (true)
      {
        if (Match(token_type::ePlus))
        {
          exp = new bin_expr('+', exp, VecMultiExpr());
          continue;
        }
        if (Match(token_type::eMinus))
        {
          exp = new bin_expr('-', exp, VecMultiExpr());
          continue;
        }
        break;
      }
      return exp;
    }

    expr* VecMultiExpr(void)
    {
      expr* exp = VecPrimExpr();

      while (true)
      {
        if (Match(token_type::eStar))
        {
          exp = new bin_expr('*', exp, Unary());
          continue;
        }
        if (Match(token_type::eSlash))
        {
          exp = new bin_expr('/', exp, Unary());
          continue;
        }
        break;
      }
      return exp;
    }
    expr* VecPrimExpr(void)
    {
      token cur = Get(0);

      if (cur.Text == "vec3")
      {
        Consume(token_type::eType);
        Consume(token_type::eLParen);
        std::vector<expr*> Ex;

        while (!Match(token_type::eRParen))
        {
          Ex.emplace_back(Expr());
          Match(token_type::eSemicolon);
        }
        return new vec_expr(Ex);
      }
      else if (Match(token_type::eWord))
      {
        auto a = variables::Get(cur.Text);

        if (a.Type == var_type::eVec)
          return new const_expr(cur.Text);

        throw std::exception("it isn't vector!");
      }
      else if (Match(token_type::eLParen))
      {
        expr* res = VecExpr();
        Match(token_type::eRParen);
        // res->Text = std::format("({})", res->Text);
        return res;
      }

      throw std::exception("incorrect vector input data!");
    }

    expr* Expr(void)
    {
      return LogOr();
    }

    expr* LogOr(void)
    {
      expr* res = LogAnd();

      while (true)
      {
        if (Match(token_type::eBARBAR))
        {
          res = new cond_expr(cond_expr::eOR, res, LogAnd());
          continue;
        }
        break;
      }

      return res;
    }

    expr* LogAnd(void)
    {
      expr* res = LogEq();

      while (true)
      {
        if (Match(token_type::eAMPAMP))
        {
          res = new cond_expr(cond_expr::eAND, res, LogEq());
          continue;
        }
        break;
      }

      return res;
    }

    expr* LogEq(void)
    {
      expr* res = Conditional();

      while (true)
      {
        if (Match(token_type::eEQEQ))
        {
          res = new cond_expr(cond_expr::eEQ, res, Conditional());
          continue;
        }
        else if (Match(token_type::eEXCLEQ))
        {
          res = new cond_expr(cond_expr::eNOTEQ, res, Conditional());
          continue;
        }
        break;
      }

      return res;
    }

    expr* Conditional(void)
    {
      expr* exp = Additive();

      while (true)
      {
        if (Match(token_type::eGT))
        {
          exp = new cond_expr(cond_expr::oper_type::eGT, exp, Additive());
          continue;
        }
        else if (Match(token_type::eLT))
        {
          exp = new cond_expr(cond_expr::oper_type::eLT, exp, Additive());
          continue;
        }
        else if (Match(token_type::eGTEQ))
        {
          exp = new cond_expr(cond_expr::oper_type::eGTEQ, exp, Additive());
          continue;
        }
        else if (Match(token_type::eLTEQ))
        {
          exp = new cond_expr(cond_expr::oper_type::eLTEQ, exp, Additive());
          continue;
        }
        break;
      }
      return exp;
    }

    expr* Additive(void)
    {
      expr* exp = Multi();

      while (true)
      {
        if (Match(token_type::ePlus))
        {
          exp = new bin_expr('+', exp, Multi());
          continue;
        }
        if (Match(token_type::eMinus))
        {
          exp = new bin_expr('-', exp, Multi());
          continue;
        }
        break;
      }
      return exp;
    }

    expr* Multi(void)
    {
      expr* exp = Unary();

      while (true)
      {
        if (Match(token_type::eStar))
        {
          exp = new bin_expr('*', exp, Unary());
          continue;
        }
        if (Match(token_type::eSlash))
        {
          exp = new bin_expr('/', exp, Unary());
          continue;
        }
        break;
      }
      return exp;
    }
    expr* Unary(void)
    {
      if (Match(token_type::eMinus))
      {
        return new un_expr('-', Primary());
      }

      return Primary();
    }
    expr* Primary(void)
    {
      token cur = Get(0);

      if (Match(token_type::eNumber))
      {
        double num = std::stod(cur.Text);
        return new num_expr(num);
      }
      if (Match(token_type::eWord))
      {
        /* Function */
        if (Get(0).Type == token_type::eLParen)
        {
          Consume(token_type::eLParen);

          if (cur.Text == "sin")
          {
            expr* res = Expr();
            Consume(token_type::eRParen);
            return new sin_expr(res);
          }
          if (cur.Text == "cos")
          {
            expr* res = Expr();
            Consume(token_type::eRParen);
            return new cos_expr(res);
          }
          if (cur.Text == "abs")
          {
            expr* res = Expr();
            Consume(token_type::eRParen);
            return new abs_expr(res);
          }
          if (cur.Text == "mod")
          {
            expr* e1 = Expr();
            Consume(token_type::eSemicolon);
            expr* e2 = Expr();
            Consume(token_type::eRParen);
            return new fmod_expr(e1, e2);
          }
        }
        /* Variable */
        else
        {
          auto a = variables::Get(cur.Text);

          if (a.Type == var_type::eInt || a.Type == var_type::eFloat)
            return new const_expr(cur.Text);

          throw std::exception("it isn't number!");
        }
      }
      if (Match(token_type::eLParen))
      {
        expr* res = Expr();
        Match(token_type::eRParen);
        return res;
      }

      throw std::exception("incorrect input data!");
    }

    statement* BlockOrStatement(void)
    {
      if (Get(0).Type == token_type::eLBRACE)
        return Block();
      return Statement();
    }

    statement* Block(void)
    {
      block_statement* block = new block_statement();

      Consume(token_type::eLBRACE);
      while (!Match(token_type::eRBRACE))
      {
        block->Add(Statement());
      }
      return block;
    }

    statement* Statement(void)
    {
      if (Match(token_type::eIf))
        return IfStatement();
      if (Match(token_type::eWhile))
        return WhileStatement();
      if (Match(token_type::eFor))
        return ForStatement();
      if (Match(token_type::eAdd))
        return AddStatement();
      return AssignStatement();
    }

    statement* AddStatement(void)
    {
      token cur = Consume(token_type::eLParen);
      add_statement* s = new add_statement();

      while (cur.Type != token_type::eRParen)
      {
        cur = Next();

        if (Get(0).Type == token_type::eSemicolon || Get(0).Type == token_type::eRParen)
        {
          if (!variables::IsExists(cur.Text))
            throw std::exception("no such variable exists!");
          s->Add(cur.Text);
        }
      }
      return s;
    }

    statement* AssignStatement(void)
    {
      token
        cur = Get(0),
        next = Get(1);

      if (cur.Type == token_type::eType && next.Type == token_type::eWord && Get(2).Type == token_type::eEQ)
      {
        Match(token_type::eType);
        Match(token_type::eWord);
        Match(token_type::eEQ);

        if (variables::IsExists(next.Text))
          throw std::exception("such varibale is already exists!");

        var_type type = TYPES[cur.Text];
        if (type == var_type::eInt || type == var_type::eFloat)
        {
          int p1 = CurPos;
          expr* e = Expr();
          std::string tmp = "";

          for (int i = p1; i < CurPos; i++)
            tmp += Tokens[i].Text;

          return new assign_statement(TYPES[cur.Text], next.Text, e, tmp);
        }
        else if (type == var_type::eVec)
        {
          int p1 = CurPos;
          expr* e = VecExpr();
          std::string tmp = "";

          for (int i = p1; i < CurPos; i++)
            tmp += Tokens[i].Text;

          return new assign_statement(TYPES[cur.Text], next.Text, e, tmp);
        }
        // NOT NUMBERS
        return new assign_statement(TYPES[cur.Text], next.Text, Func(next.Text), "");
      }
      if (cur.Type == token_type::eWord && next.Type == token_type::eEQ)
      {
        Match(token_type::eWord);
        Match(token_type::eEQ);

        var_type type;
        if (!variables::GetType(cur.Text, &type))
          throw std::exception("no such varibale!");

        if (type == var_type::eInt || type == var_type::eFloat)
        {
          int p1 = CurPos;
          expr* e = Expr();
          std::string tmp = "";

          for (int i = p1; i < CurPos; i++)
            tmp += Tokens[i].Text;

          return new assign_statement(type, cur.Text, e, tmp);
        }
        else if (type == var_type::eVec)
        {
          int p1 = CurPos;
          expr* e = VecExpr();
          std::string tmp = "";

          for (int i = p1; i < CurPos; i++)
            tmp += Tokens[i].Text;

          return new assign_statement(type, cur.Text, e, tmp);
        }

        // NOT NUMBERS
        return new assign_statement(type, cur.Text, Func(cur.Text), "");
      }

      throw std::exception("Unknown statement");
    }

    statement* IfStatement(void)
    {
      expr* cond = Expr();
      statement
        * ifst = BlockOrStatement(),
        * elsest = nullptr;

      if (Match(token_type::eElse))
        elsest = BlockOrStatement();

      return new if_statement(cond, ifst, elsest);
    }

    statement* WhileStatement(void)
    {
      expr* cond = Expr();
      statement* st = BlockOrStatement();

      return new while_statement(cond, st);
    }

    statement* ForStatement(void)
    {
      Consume(token_type::eLParen);
      statement* init = AssignStatement();
      Consume(token_type::eSemicolon);
      expr* term = Expr();
      Consume(token_type::eSemicolon);
      statement* incr = AssignStatement();
      Consume(token_type::eRParen);
      statement* block = BlockOrStatement();

      return new for_statement(init, term, incr, block);
    }

  public:
    parser(std::vector<token> NewTokens) : Tokens(NewTokens)
    {
      CurPos = 0;
      Size = (int)Tokens.size();
    }

    statement* Parse(void)
    {
      block_statement* result = new block_statement();

      while (!Match(token_type::eEOF))
        result->Add(Statement());

      return result;
    }
  };

  void Parse(const std::string &Scene, const std::string &ShIn, const std::string &ShOut )
  {
    std::string F = file::ReadFile(Scene);

    lexer L(F);

    std::vector<token> lt = L.Tokenize();

    parser P(lt);

    statement* state = P.Parse();

    state->Execute();

    delete state;

    file::PrintFile(ShIn, ShOut, obj::light::GetStr(), obj::shape::GetTexStr());
    variables::Clear();
  }
}

#endif

/* END OF 'parser.h' FILE */