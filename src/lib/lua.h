#ifndef _MAD_LUA_H_
#define _MAD_LUA_H_

#include <memory>
#include <vector>
#include <map>
#include <lua.hpp>

struct LuaDeleter {
  void operator()(lua_State *L) {
    lua_close(L);
  }
};

class LuaFunction {
  private:
    std::vector<std::string> arg_names;
    std::unique_ptr<lua_State, LuaDeleter> state;
    void CheckResult(int result) const;

  public:
    LuaFunction(std::string function, std::vector<std::string> arg_names);
    LuaFunction(const LuaFunction& other) = delete;
    LuaFunction(LuaFunction&& other);

    std::string operator()(const std::vector<std::string>& args) const;
};

class Lua {
  private:
    std::map<uint32_t, std::unique_ptr<LuaFunction>> functions;

  public:
    Lua() {}
    Lua(const Lua& other) = delete;
    ~Lua() {}

    const LuaFunction& Compile(std::string function, std::vector<std::string> arg_names);
};

#endif
