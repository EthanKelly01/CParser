#include "Preprocessor.h"

#include <assert.h> //TODO: fix this system
#include <regex>

namespace Parser {

    void preprocessor(std::vector<std::string>& project) {

        std::vector<dependency> myTree;
        std::vector<std::string> filesDone = {};
        constructDeps(project, myTree, filesDone);
        for (dependency x : myTree) findCycles(x, x);

        for (std::string &file : project) {
            dependency current;
            for (dependency x : myTree) if (x.filename == file.substr(10, file.find_first_of("\n"))) current = x; //find matching dependency struct //TODO: improve this
            define myDefines = findDefines(file);
            //handle defines
            for (std::pair<std::string, std::string> x : myDefines) {
                std::smatch m;
                std::regex r("^([^#]*)\s" + x.first + "\s"); //exclude defines and strings
                while (std::regex_search(file, m, r)) if (std::count(m[1].str().begin(), m[1].str().end(), '\"') % 2 == 0) file.replace(m.position(), m.length(), x.second);
            }
        }
    }

    void constructDeps(std::vector<std::string>& project, std::vector<dependency>& myTree, std::vector<std::string>& filesDone) {
        std::smatch m;
        std::regex r("^[ \t]*#[ \t]*include[ \t]+\"[[:graph:]]+\""); //find all valid includes in a file

        bool fileSkipped = 0;
        for (std::string file : project) {
            std::string filename = file.substr(10, file.find_first_of("\n"));
            for (dependency x : myTree) if (x.filename == filename) goto outerLoop; //file is already counted

            {
                std::vector<dependency> dependencies;

                //find all dependencies
                r = "^[ \t]*#[ \t]*include[ \t]+\"([[:graph:]]+)\""; //find all valid includes in a file
                while (std::regex_search(file, m, r)) {
                    if (std::find(filesDone.begin(), filesDone.end(), m[1]) == filesDone.end()) {
                        fileSkipped = true;
                        goto outerLoop; //TODO: find better system to check for dupes
                    }

                    for (dependency x : myTree) if (x.filename == m[1]) {
                        dependencies.push_back(x);
                        break;
                    }
                }

                define defines;
                //add all defines
                r = "^[ \t]*#[ \t]*define[ \t]+([[:graph:]]+)[ \t]+([[:print:]]+$)";
                while (std::regex_search(file, m, r)) defines.push_back({ m[1], m[2] }); //add each valid define in the file

                myTree.push_back({ filename, dependencies, defines });
                filesDone.push_back(filename);
            }

        outerLoop:
            int temp = 0; //TODO: fix this
        }
        if (fileSkipped) constructDeps(project, myTree, filesDone);
    }

    void findCycles(dependency current, dependency match) {
        for (dependency x : current.dependencies) {
            assert(x.filename != match.filename); //TODO: find a replacement
            findCycles(x, match);
        }
    }

    define findDefines(dependency x) {
        define defines = x.defines;
        for (dependency y : x.dependencies) {
            define newDefines = findDefines(y); //TODO: Optimize this
            defines.insert(defines.end(), newDefines.begin(), newDefines.end());
        }
        return defines;
    }
}