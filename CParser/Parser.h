#pragma once

#include "Structs.h"
#include "Preprocessor.h"

namespace Parser {

    enum parserState {
        VOID,
        NAMESPACE,
        CLASS,
        FUNCTION,
        ENUM,
        VARIABLE
    };

	inline std::vector<datatypes> parseFile(std::string filename);

	std::vector<datatypes> recursiveParse(std::vector<std::string> file, int& index, int state = VOID);

    void removeComments(std::vector<std::string>& project);

	void parseProject(std::string directory);
}