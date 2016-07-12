#include <cctype>
#include <list>
#include "lisp.h"

std::list<std::string> SExprParser::Tokenize(const std::string & str) {
  std::list<std::string> tokens;
  const char * s = str.c_str();
  while (*s) {
    while (*s == ' ')
      ++s;
    if (*s == '(' || *s == ')')
      tokens.push_back(*s++ == '(' ? "(" : ")");
    else {
      const char * t = s;
      while (*t && *t != ' ' && *t != '(' && *t != ')')
        ++t;
      tokens.push_back(std::string(s, t));
      s = t;
    }
  }
  return tokens;
}

SExpr SExprParser::CreateExpr(const std::string & token) {
  if (std::isdigit(token[0]) || (token[0] == '-' && std::isdigit(token[1]))) {
    return SExpr(SExpr::Number, token);
  }
  return SExpr(SExpr::Symbol, token);
}

SExpr SExprParser::CreateExpr(std::list<std::string> & tokens) {
  const std::string token(tokens.front());
  tokens.pop_front();
  if (token == "(") {
    SExpr c(SExpr::List);
    while (tokens.front() != ")") {
      c.list.push_back(CreateExpr(tokens));
    }
    tokens.pop_front();
    return c;
  }
  return CreateExpr(token);
}

SExpr SExprParser::Parse(const std::string & s) {
  std::list<std::string> tokens(Tokenize(s));
  return CreateExpr(tokens);
}

