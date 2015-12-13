//
// Created by Maxym on 12/13/2015.
//

#ifndef CCOMPILER_ASTPROXY_HPP
#define CCOMPILER_ASTPROXY_HPP

#include <string>
#include "tree_syntax/tree_syntax.hpp"

class AstProxy
{
public:
    int ProcessFile(const std::string& file_name);

    const TreeSyntaxShared& root() const noexcept { return this->root_; }

private:
    static std::string RemoveFilenameExtension(const std::string& file_name);

    TreeSyntaxShared root_;
};

#endif //CCOMPILER_ASTPROXY_HPP
