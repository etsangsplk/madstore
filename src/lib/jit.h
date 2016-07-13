#ifndef MAD_JIT_H
#define MAD_JIT_H

#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <asmjit/asmjit.h>
#include "lisp.h"

template <typename EvalReturn>
class Visitor {
  public:
    typedef std::map<std::string, std::function<EvalReturn (const std::vector<EvalReturn> &)>> FunctionMap;
    typedef std::function<EvalReturn (const std::string &symbol)> SymbolHandler;
    typedef std::function<EvalReturn (const std::string &number)> NumberHandler;

  protected:
    FunctionMap functionMap;
    NumberHandler numberHandler;
    SymbolHandler symbolHandler;

  public:
    Visitor() {}

    EvalReturn Eval(const SExpr &expr) {
      switch(expr.type) {
        case SExpr::Number:
        {
          return numberHandler(expr.val.c_str());
        }

        case SExpr::List:
        {
          std::vector<EvalReturn> evalArgs(expr.list.size() - 1);

          // eval each argument
          std::transform(expr.list.begin() + 1, expr.list.end(), evalArgs.begin(), [=](const SExpr &c) -> EvalReturn {
              return this->Eval(c);
          });

          if(functionMap.find(expr.list[0].val) == functionMap.end()) {
            throw std::runtime_error("Could not handle procedure: " + expr.list[0].val);
          }

          // call function specified by symbol map with evaled arguments
          return functionMap.at(expr.list[0].val)(evalArgs);
        }

        case SExpr::Symbol:
        {
          if (symbolHandler) {
            return symbolHandler(expr.val);
          }
          throw std::runtime_error("Cannot handle symbol: " + expr.val);
        }
      }
      throw std::runtime_error("Unsupported S-expression type: " + std::to_string(expr.type));
    }
};

// Expressions return AsmJit SSE "registers" / variables.
class JitFunction : public Visitor<asmjit::XmmVar> {
  private:
    asmjit::JitRuntime runtime;
    asmjit::X86Assembler assembler;
    asmjit::X86Compiler compiler;
    asmjit::GpVar arg0;

    typedef double (*FuncPtrType)(const double * args);
    FuncPtrType generatedFunction;

    void SetXmmVar(asmjit::XmmVar &v, double d);

  public:
    JitFunction(std::vector<std::string>& varNames, const SExpr &expr);
    ~JitFunction();

    FuncPtrType Generate(const SExpr &expr);
    double operator()(const std::vector<double> &args) const;
};

#endif /* MAD_JIT_H */
