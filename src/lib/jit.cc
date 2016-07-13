#include "jit.h"

JitFunction::JitFunction(std::vector<std::string>& varNames, const SExpr &expr)
  :runtime(),assembler(&runtime),compiler(&assembler) {

  // Map operators to assembly instructions:
  functionMap["+"] = [&](const std::vector<asmjit::XmmVar> &args) -> asmjit::XmmVar {
    compiler.addsd(args[0], args[1]);
    return args[0];
  };

  functionMap["-"] = [&](const std::vector<asmjit::XmmVar> &args) -> asmjit::XmmVar {
    compiler.subsd(args[0], args[1]);
    return args[0];
  };

  functionMap["*"] = [&](const std::vector<asmjit::XmmVar> &args) -> asmjit::XmmVar {
    compiler.mulsd(args[0], args[1]);
    return args[0];
  };

  functionMap["/"] = [&](const std::vector<asmjit::XmmVar> &args) -> asmjit::XmmVar {
    compiler.divsd(args[0], args[1]);
    return args[0];
  };

  // Convert numbers into AsmJit vars:
  numberHandler = [&](const std::string &number) -> asmjit::XmmVar {
    double x = std::atof(number.c_str());
    asmjit::XmmVar xVar(compiler.newXmmVar(asmjit::kX86VarTypeXmm));
    SetXmmVar(xVar, x);
    return xVar;
  };

  std::map<std::string, int> varNameToIndex;
  for (auto it = varNames.begin(); it != varNames.end(); ++it) {
    varNameToIndex[*it] = it - varNames.begin();
  }

  // Resolve symbol to a function argument value:
  symbolHandler = [&](const std::string name) -> asmjit::XmmVar {
    int index = varNameToIndex.at(name);
    asmjit::XmmVar v(compiler.newXmm());
    compiler.movsd(v, asmjit::Mem(arg0, index * sizeof(double)));
    return v;
  };

  generatedFunction = Generate(expr);
}

void JitFunction::SetXmmVar(asmjit::XmmVar &v, double d) {
  // No immediates for SSE regs/doubles. So put into a general purpose reg
  // and then move into SSE - we could do better than this.
  asmjit::GpVar gpreg = compiler.newUInt64();
  uint64_t *i = reinterpret_cast<uint64_t*>(&d);
  compiler.mov(gpreg, *i); 
  compiler.movq(v, gpreg); 
  compiler.unuse(gpreg);
}

JitFunction::FuncPtrType JitFunction::Generate(const SExpr &c) {
  compiler.addFunc(asmjit::FuncBuilder1<double, const double *>());
  arg0 = compiler.newUInt64();
  compiler.setArg(0, arg0);
  asmjit::XmmVar retVar = Eval(c);
  compiler.ret(retVar);
  compiler.endFunc();
  compiler.finalize();
  return reinterpret_cast<FuncPtrType>(assembler.make());
}

double JitFunction::operator()(const std::vector<double> &args) const {
  return generatedFunction(args.data()); 
}

JitFunction::~JitFunction() {
  runtime.release((void*) generatedFunction);
}

