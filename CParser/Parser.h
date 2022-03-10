#pragma once

#include "Structs.h"
#include "Preprocessor.h"

namespace Parser {
	std::vector<datatypes*> parseFile(std::string filename);

    void removeComments(std::vector<std::string>& project);

	void parseProject(std::string directory);
}