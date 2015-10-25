//
// Created by Maxym on 10/24/2015.
//

#ifndef CCOMPILER_PFRAGMENT_HPP
#define CCOMPILER_PFRAGMENT_HPP

#include <string>

class CommentsCleaner
{
public:
    explicit CommentsCleaner(const std::string& text);

    // Returns a copy of string, with comments omitted.
    const std::string& ToString();
private:
    enum Scope {
        SCOPE_MULTI_LINE_COMMENT,
        SCOPE_MULTI_LINE_COMMENT_STAR,
        SCOPE_BACKSLASH,
        SCOPE_GLOBAL,
        SCOPE_LINE_COMMENT,
        SCOPE_LINE_COMMENT_CONTINUE,
        SCOPE_QUOTED,
        SCOPE_DOUBLE_QUOTED
    };

private:
    std::string buffer_;

};


#endif //CCOMPILER_PFRAGMENT_HPP
