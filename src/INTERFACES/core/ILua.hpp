#ifndef _OPENMAP_ILUA_HPP
#define _OPENMAP_ILUA_HPP

#include <vector>
#include <any>

class ILua {

public:
        template<typename Func>
        void SetFunction(const char* function_name, Func body);

        virtual void ExecuteFile(const char* target_file_path) = 0;

        template<typename ...Args>
        std::vector<std::any> CallFunction(const char* function_name, Args... args);

        virtual ~ILua() = default;

};

#endif // _OPENMAP_ILUA_HPP