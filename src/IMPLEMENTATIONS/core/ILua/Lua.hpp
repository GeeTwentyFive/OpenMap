#ifndef _OPENMAP_LUA_HPP
#define _OPENMAP_LUA_HPP

#include <src/INTERFACES/core/ILua.hpp>

#include <string>
#include <functional>
#include <libs/lua/include/lua.hpp>
#include <libs/LuaBridge3/LuaBridge.hpp>


class Lua: public ILua {

private:
        lua_State* L;


        std::any LuaTableToAny(const luabridge::LuaRef& table) {
                // For now only supports array-like tables // TODO?
                std::vector<std::any> result;
                for (
                        int i = 1;
                        i <= table.length();
                        i++
                ) result.push_back(LuaValueToAny(table[i]));
                return result;
        }

        std::any LuaValueToAny(const luabridge::LuaRef& val) {
                if (val.isNil()) return std::any();
                if (val.isBool()) return std::any(val.cast<bool>());
                if (val.isNumber()) return std::any(val.cast<double>());
                if (val.isString()) return std::any(val.cast<std::string>());
                if (val.isTable()) return LuaTableToAny(val);
                throw std::runtime_error("ERROR: Unsupported Lua value conversion attempt");
        }


public:
        Lua() {
                L = luaL_newstate();
                luaL_openlibs(L);
        }

        template<typename Func>
        void SetFunction(const char* function_name, Func body) {
                luabridge::getGlobalNamespace(L)
                        .addFunction(function_name, body);
        }

        inline void ExecuteFile(const char* target_file_path) {
                if (luaL_loadfile(L, target_file_path)) {
                        throw std::runtime_error(
                                std::string("ERROR: Failed to load Lua script in path: ") +
                                '"' + target_file_path + '"' +
                                "\nLua's error message: " +
                                '"' + lua_tostring(L, -1) + '"'
                        );
                }
                if (lua_pcall(L, 0, 0, 0)) {
                        throw std::runtime_error(
                                std::string("ERROR: Failed to execute Lua script: ") +
                                '"' + target_file_path + '"' +
                                "\nLua's error message: " +
                                '"' + lua_tostring(L, -1) + '"'
                        );
                }
        }

        template<typename ...Args>
        std::vector<std::any> CallFunction(const char* function_name, Args... args) {
                luabridge::LuaRef f = luabridge::getGlobal(L, function_name);
                if (f.isNil()) {
                        throw std::runtime_error(
                                std::string("ERROR: Failed to find function in Lua: ") +
                                '"' + function_name + '"'
                        );
                }

                luabridge::LuaResult result = luabridge::call(function_name, args...);
                if (result.hasFailed()) {
                        throw std::runtime_error(
                                std::string("ERROR: Failed calling Lua function: ") +
                                '"' + function_name + '"' +
                                "\nLua's error message: " +
                                '"' + result.errorMessage() + '"'
                        );
                }

                std::vector<std::any> out;
                for (
                        std::size_t val_index = 0;
                        val_index < result.size();
                        val_index++
                ) {
                        try {
                                out.push_back(LuaValueToAny(result[val_index]));
                        }
                        catch (const std::runtime_error&) {
                                throw std::runtime_error(
                                        std::string("ERROR: Unsupported return type from function: ") +
                                        '"' + function_name + '"' +
                                        ", at index: " + std::to_string(val_index)
                                );
                        }
                }
                return out;
        }

        ~Lua() {
                lua_close(L);
        }

};

#endif // _OPENMAP_LUA_HPP