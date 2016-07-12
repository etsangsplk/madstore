#ifndef MAD_LISP_H
#define MAD_LISP_H

#include <string>
#include <vector>
#include <list>

struct SExpr {
  enum Type {Symbol, Number, List};

  Type type;
  std::string val;
  std::vector<SExpr> list;
  SExpr(Type type = Symbol): type(type) {}
  SExpr(Type type, const std::string & val): type(type), val(val) {}
};

class SExprParser {
  private:
    SExpr CreateExpr(const std::string & token);
    SExpr CreateExpr(std::list<std::string> & tokens);
    std::list<std::string> Tokenize(const std::string & str);

  public:
    SExpr Parse(const std::string& s);
};

#endif /* MAD_LISP_H */
