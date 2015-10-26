//
// Created by Maxym on 10/24/2015.
//

#include "CommentsCleaner.hpp"
#include <typeinfo>

CommentsCleaner::CommentsCleaner(const std::string &text)
{
    this->buffer_.assign(text);

    auto state = SCOPE_GLOBAL;
    auto length = this->buffer_.length();
    decltype(length) line = 0;
    for (decltype(length) i = 0; i < length; i++)
    {
        auto current_symbol = buffer_.at(i);
        if (current_symbol == '\n')
        {
            line++;
        }
        switch (state)
        {
            case SCOPE_GLOBAL:
                switch (current_symbol)
                {
                    case '/':
                        state = SCOPE_BACKSLASH;
                        break;
                    case '\'':
                        state = SCOPE_QUOTED;
                        break;
                    case '\"':
                        state = SCOPE_DOUBLE_QUOTED;
                        break;
                }
                break;
            case SCOPE_BACKSLASH:
            {
                switch (current_symbol) {
                    case '*':
                        this->buffer_[i] = ' ';
                        this->buffer_[i - 1] = ' ';
                        state = SCOPE_MULTI_LINE_COMMENT;
                        break;
                    case '/':
                        this->buffer_[i] = ' ';
                        this->buffer_[i - 1] = ' ';
                        state = SCOPE_LINE_COMMENT;
                        break;
                    default:
                        state = SCOPE_GLOBAL;
                }
                break;
            }
            case SCOPE_QUOTED:
                if (current_symbol == '\'')
                {
                    state = SCOPE_GLOBAL;
                }
                break;
            case SCOPE_DOUBLE_QUOTED:
                if (current_symbol == '\"')
                {
                    state = SCOPE_GLOBAL;
                }
                break;
            case SCOPE_MULTI_LINE_COMMENT_STAR:
                this->buffer_[i] = ' ';
                if (current_symbol == '/')
                {
                    state = SCOPE_GLOBAL;
                } else
                {
                    state = SCOPE_MULTI_LINE_COMMENT;
                }
                break;
            case SCOPE_LINE_COMMENT:
            {
                if (current_symbol != '\n')
                {
                    this->buffer_[i] = ' ';
                }
                switch (current_symbol){
                    case '\n':
                        state = SCOPE_GLOBAL;
                        break;
                    case '\\':
                        state = SCOPE_LINE_COMMENT_CONTINUE;
                        break;
                }
                break;
            }
            case SCOPE_LINE_COMMENT_CONTINUE:
            {
                if (current_symbol != '\n')
                {
                    this->buffer_[i] = ' ';
                }
                if (!std::isspace(current_symbol) or current_symbol == '\n')
                {
                    state = SCOPE_LINE_COMMENT;
                }

                break;
            }
            case SCOPE_MULTI_LINE_COMMENT:
            {
                this->buffer_[i] = ' ';
                if (current_symbol == '*')
                {
                    state = SCOPE_MULTI_LINE_COMMENT_STAR;
                }
                break;
            }
        }
    }
}

const std::string &CommentsCleaner::ToString()
{
    return this->buffer_;
}
