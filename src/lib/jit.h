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

    EvalReturn Eval(const SExpr &c) {
      switch(c.type) {
        case SExpr::Number:
        {
          return numberHandler(c.val.c_str());
        }

        case SExpr::List:
        {
          std::vector<EvalReturn> evalArgs(c.list.size() - 1);

          // eval each argument
          std::transform(c.list.begin() + 1, c.list.end(), evalArgs.begin(), [=](const SExpr &c) -> EvalReturn {
              return this->Eval(c);
          });

          if(functionMap.find(c.list[0].val) == functionMap.end()) {
            throw std::runtime_error("Could not handle procedure: " + c.list[0].val);
          }

          // call function specified by symbol map with evaled arguments
          return functionMap.at(c.list[0].val)(evalArgs);
        }

        case SExpr::Symbol:
        {
          if (symbolHandler) {
            return symbolHandler(c.val);
          }
          throw std::runtime_error("Cannot handle symbol: " + c.val);
        }
      }
      throw std::runtime_error("Unsupported S-expression type: " + c.type);
    }
};

// Expressions return AsmJit SSE "registers" / variables.
class JitFunction : public Visitor<asmjit::XmmVar> {
  private:
    asmjit::JitRuntime runtime;
    asmjit::X86Assembler assembler;
    asmjit::X86Compiler compiler;
    std::map<std::string, int> argNameToIndex;
    asmjit::GpVar arg0;

    typedef double (*FuncPtrType)(const double * args);
    FuncPtrType generatedFunction;

    void SetXmmVar(asmjit::XmmVar &v, double d);

  public:
    JitFunction(const std::vector<std::string> &names, const SExpr &cell);
    ~JitFunction();

    FuncPtrType Generate(const SExpr &c);
    double operator()(const std::vector<double> &args) const;
};

#endif /* MAD_JIT_H */
