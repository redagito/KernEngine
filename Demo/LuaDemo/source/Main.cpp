#include <iostream>
#include <lua.hpp>
#include <string>

int println(lua_State *s)
{
    std::cout << lua_tostring(s, 1) << std::endl;
    return 0;
}

class LuaState
{
   public:
    LuaState()
    {
        state = luaL_newstate();
        luaL_openlibs(state);
        lua_pushcfunction(state, println);
        lua_setglobal(state, "myprintln");
    }

    ~LuaState() { lua_close(state); }

    lua_State *state = nullptr;
};

void onError(lua_State *state)
{
    std::cout << "Error: " << lua_tostring(state, -1) << std::endl;
    lua_pop(state, 1);
    throw std::runtime_error("Error");
}

void eval(const std::string &line)
{
    LuaState state;

    int err = luaL_loadstring(state.state, line.c_str());
    std::cout << "Eval string: " << line << std::endl;
    if (err)
        onError(state.state);

    err = lua_pcall(state.state, 0, 0, 0);
    if (err)
        onError(state.state);
}

int main(int argc, char **argv) {
    std::string line;
    std::getline(std::cin, line);
    eval(line);
    
    return 0; 

}