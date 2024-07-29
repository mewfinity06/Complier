#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "src/lexer.hpp"

std::ostream &operator<<(std::ostream& strm, TokenKind tt) {
    const std::string kind[] = {
        "EOF", "Identifier", "Number", "QuotedString",
        "RightArrow", "LeftArrow", "Assign", 
        "GreaterThan", "LessThan", "GreaterEquals", "LessEquals",
        "And", "Or", "PlusPlus", "MinusMinus", "DoubleEquals",
        "FatArrow", "OpenParen", "CloseParen", "OpenBrack", 
        "CloseBrack", "OpenCurly", "CloseCurly", "Tilda", 
        "Bang", "Question", "Pipe", "Semicolon", "Colon", 
        "BinaryOperator", "Let", "Const", "Func", "For", "While", 
        "Int", "String",
    };
    return strm << kind[tt];
}

int main(int argc, char* argv[]) {
    // Check if a filename was provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        exit(1);
    }

    std::string filename = argv[1];
    std::ifstream file(filename);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string contents = buffer.str();
    contents += " ";

    // Close the file
    file.close();

    Lexer lex = Lexer(contents);
    auto tokens = lex.tokenize();

    for (Token token : tokens) {
        std::cout << token.value << " -> " << token.kind << std::endl;
    }


    return 0;
}