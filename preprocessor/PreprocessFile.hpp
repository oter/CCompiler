//
// Created by Maxym on 10/24/2015.
//

#ifndef CCOMPILER_PREPROCESSOROBJECT_HPP
#define CCOMPILER_PREPROCESSOROBJECT_HPP

#include <string>
#include <vector>

class PreprocessFile {
public:
    PreprocessFile(const std::string& fileName);

private:
    std::string fileName;
    std::vector<std::string> s;
};


#endif //CCOMPILER_PREPROCESSOROBJECT_HPP
