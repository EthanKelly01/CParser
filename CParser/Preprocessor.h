#pragma once
#include "Structs.h"

#include <map>

namespace Parser {
    typedef std::vector<std::pair<std::string, std::string>> define; //TODO: rename for clarity

    struct dependency {
        //TODO: find better graphing system to store these
        std::string filename;
        std::vector<dependency> dependencies;
        define defines;

        dependency() = default;
        dependency(std::string t_filename, std::vector<dependency> t_dependencies = {}, define t_defines = {}) : filename(t_filename), dependencies(t_dependencies), defines(t_defines) {};
    };

	void preprocessor(std::vector<std::string>& project);

	void constructDeps(std::vector<std::string>& project, std::map<std::string, dependency>& myTree, std::vector<std::string>& filesDone, int index = 0);

    void findCycles(dependency current, dependency match);

    define findDefines(dependency x);
}