#pragma once

#include <iostream>
#include <vector>

#include "lexer.hpp"

class Expr {

};

class Program {
    public:
    std::vector<Expr> exprs;
};

class Parser {
    public:
    Lexer lexer;
    Program program;

    Parser(Lexer lexer) : lexer(lexer) {}

    Program parse_file() {

    }

    private:

};