#include "lua.h"
#include "MurmurHash3.h"

LuaFunction::LuaFunction(std::string function, std::vector<std::string> arg_names)
  :arg_names(std::move(arg_names)),state(std::unique_ptr<lua_State, LuaDeleter>(luaL_newstate())) {

  auto L = state.get();
  luaL_openlibs(L);
  CheckResult(luaL_loadstring(L, ("return " + function).c_str()));
  lua_setglobal(L, "f");
}

LuaFunction::LuaFunction(LuaFunction&& other):state(std::move(other.state)) {}

void LuaFunction::CheckResult(int result) const {
  if (result != 0) {
    auto L = state.get();
    std::string message(lua_tostring(L, -1));
    lua_pop(L, 1);
    throw std::runtime_error(std::move(message));
  }
}

std::string LuaFunction::operator() (const std::vector<std::string>& args) const {
  auto L = state.get();
  lua_getglobal(L, "f");

  for (int i = 0; i < args.size(); ++i) {
    lua_pushlstring(L, args[i].c_str(), args[i].size());
    lua_setglobal(L, arg_names[i].c_str());
  }

  CheckResult(lua_pcall(L, 0, 1, 0));
  std::string result(lua_tostring(L, -1));
  lua_pop(L, 1);
  return std::move(result);
}

const LuaFunction& Lua::Compile(std::string function, std::vector<std::string> arg_names) {
  uint32_t hash;
  MurmurHash3_x86_32(function.c_str(), function.size(), 0x5d983e09, &hash);

  auto existing = functions.find(hash);
  if (existing != functions.end()) {
    return *existing->second.get();
  }

  functions[hash] = std::make_unique<LuaFunction>(function, arg_names);
  return *functions[hash].get();
}
