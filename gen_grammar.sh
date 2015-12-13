#!/bin/sh
yacc -d grammar/myc/parser.y -o grammar/parser.cxx

lex -t grammar/myc/lexer.l > grammar/lexer.cxx