# CParser
 A simple C++ syntax parser library.
 
 The project is designed to read the entire directory of a c++ project and return a compilation of the objects in the project.<br>
 **NOTE:** this project is meant to be part of a later project for c++ runtime reflection.

<h1 align="center">Design</h1>

This library includes a custom datatype aptly named `datatypes` which includes different implementations for each type of object in c++ but all include a `toString()` function to output the contents.

The parser uses a static function system to read in, process, and output data from a c++ project. While this can be achieved with a single function `parseProject(directoryName)`, each step is available individually under separate functions.<br>

The `preprocessor(&vector<string>)` function builds a file depency system to act upon preprocessor directives such as #define's. This function edits the program's copy of all files. Once all preprocessor directives in the project can be handled, each file can be individually parsed for its data.<br>

`parseFile(filename)` can be called on a single file to parse each of the objects out of it. It will use a helper function, `recursiveParse` to fully parse all data.<br>

There are two supported ways to output data for use. The list of objects can be passed directly to the caller as a `std::vector<datatypes>` or it can be serialized into a txt file to be read later. The *current* plan to implement this is to overload the `parseProject(directory)` function later, however there is currently an `outputFile()` function to later be implemented.
