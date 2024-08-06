#include <iostream>
#include <string>
#include <vector>
#include <map>

enum TokenKind {

    Eof,

    // Multi char tokens
    Ident,
    Number,
    QuotedString,

    RightArrow,
    LeftArrow,
    Assign,

    GreaterThan,
    LessThan,
    GreaterEquals,
    LessEquals,

    And,
    Or,
    PlusPlus,
    MinusMinus,
    DoubleEquals,
    FatArrow,

    // Single char tokens
    OpenParen,
    CloseParen,
    OpenBrack,
    CloseBrack,
    OpenCurly,
    CloseCurly,

    Tilda,
    Bang,
    Question,
    Pipe,
    Semicolon,
    Colon,

    BinOperator,

    // Keywords
    Let,
    Const,
    Func,
    For,
    If,
    Else,
    Elif,
    While,
    Int,
    String,

};

class Token {
    public:
    std::string value;
    TokenKind kind;

    Token(std::string v, TokenKind k) : value(v), kind(k) {}
    Token(char v, TokenKind k) : value(std::string(1, v)), kind(k) {}
};

class Lexer {
    public:
    std::string &source;
    int index;
    std::map<std::string, TokenKind> keywords;
    std::vector<Token> tokens;

    Lexer(std::string &source) : source(source), index(0) {}

    std::vector<Token> tokenize() {
        std::string buf;

        keywords = create_keywords();

        while (source[index] != '\0') {
            // If is an alpha or numberic, add to the buffer to be processed above
            if (isalnum(source[index])) {
                buf += source[index];
            }

            tokenize_multi(buf);

            // Handle single line comments
            if (source[index] == '/' && source[index + 1] == '/') {
                advance(2);
                while (source[index] != '\n') {
                    advance();
                }
            }

            // Handle multi line comments
            else if (source[index] == '/' && source[index + 1 == '*']) {
                advance(2);
                while (source[index] != '*' && source[index + 1] != '/') {
                    advance();
                }
                advance(2);
            }

            // Hangle QuotedStrings
            if (source[index] == '"') {
                std::string comment_buf;
                do {
                    comment_buf += source[index];
                    advance();
                } while (source[index] != '"');
                comment_buf += '"';
                tokens.push_back(Token(comment_buf, QuotedString));
                advance();
            }
            else if (source[index] == '\'') {
                std::string comment_buf;
                do {
                    comment_buf += source[index];
                    advance();
                } while (source[index] != '\'');
                comment_buf += '\'';
                tokens.push_back(Token(comment_buf, QuotedString));
                advance();
            }
            
            // Handle ->, <-, ==, ++, --, etc
            if (source[index] == '-' && source[index + 1] == '>') {
                tokens.push_back(Token("->", RightArrow));
                advance(2);
            }
            else if (source[index] == '<' && source[index + 1] == '-') {
                tokens.push_back(Token("<-", LeftArrow));
                advance(2);
            }

            if (source[index] == '=' && source[index + 1] == '=') {
                tokens.push_back(Token("==", DoubleEquals));
                advance(2);
            }
            else if (source[index] == '+' && source[index + 1] == '+') {
                tokens.push_back(Token("++", PlusPlus));
                advance(2);
            }
            else if (source[index] == '-' && source[index + 1] == '-') {
                tokens.push_back(Token("--", MinusMinus));
                advance(2);
            }
            else if (source[index] == '>' && source[index + 1] == '=') {
                tokens.push_back(Token(">=", GreaterEquals));
                advance(2);
            }
            else if (source[index] == '<' && source[index + 1] == '=') {
                tokens.push_back(Token("<=", LessEquals));
                advance(2);
            }
            else if (source[index] == '=' && source[index + 1] == '>') {
                tokens.push_back(Token("=>", FatArrow));
                advance(2);
            }

            // Handle single char tokens
            switch (source[index]) {
                case '{':
                    tokens.push_back(Token(source[index], OpenCurly));
                    break;
                case '}':
                    tokens.push_back(Token(source[index], CloseCurly));
                    break;
                case '[':
                    tokens.push_back(Token(source[index], OpenBrack));
                    break;
                case ']':
                    tokens.push_back(Token(source[index], CloseBrack));
                    break;
                case '(':
                    tokens.push_back(Token(source[index], OpenParen));
                    break;
                case ')':
                    tokens.push_back(Token(source[index], CloseParen));
                    break;
                case '+':
                case '-':
                case '*':
                case '/':
                    tokenize_multi(buf);
                    tokens.push_back(Token(source[index], BinOperator));
                    break;
                case '=':
                    tokenize_multi(buf);
                    tokens.push_back(Token(source[index], Assign));
                    break;
                case ';':
                    tokens.push_back(Token(source[index], Semicolon));
                    break;
                case ':':
                    tokens.push_back(Token(source[index], Colon));
                    break;
                case '~':
                    tokens.push_back(Token(source[index], Tilda));
                    break;
                case '!':
                    tokens.push_back(Token(source[index], Bang));
                    break;
                case '?':
                    tokens.push_back(Token(source[index], Question));
                    break;
                case '|':
                    tokens.push_back(Token(source[index], Pipe));
                    break;
                case '>':
                    tokens.push_back(Token(source[index], GreaterThan));
                    break;
                case '<':
                    tokens.push_back(Token(source[index], LessThan));
                    break;
            }
            advance();
        }

        tokens.push_back(Token("EOF", Eof));

        return tokens;
    }

    private:

    void advance(int offset = 1) {
        index += offset;
    }

    std::map<std::string, TokenKind> create_keywords() {
        std::map<std::string, TokenKind> keywords;

        keywords["let"] = Let;
        keywords["const"] = Const;
        keywords["func"] = Func;
        keywords["for"] = For;
        keywords["while"] = While;
        keywords["int"] = Int;
        keywords["string"] = String;
        keywords["if"] = If;
        keywords["else"] = Else;
        keywords["elif"] = Elif;

        return keywords;
    }

    void tokenize_multi(std::string &buf) {
        if (source[index] == ' ' || source[index] == '\n' || source[index] == '\t' || !isalnum(source[index])) {
                if (isalpha(buf[0])) {
                    auto it = keywords.find(buf);
                    if (it != keywords.end()) {
                        tokens.push_back(Token(buf, keywords[buf]));
                    }
                    else {
                        tokens.push_back(Token(buf, Ident));
                    }
                }
                if (isdigit(buf[0])) {
                    int dot_found = 0;
                    for (char c : buf) {
                        if (!isdigit(c) || dot_found > 1) {
                            std::cerr << "Cannot have alpha characters in number : " << buf << std::endl;
                            exit(1);
                        }
                    }
                    tokens.push_back(Token(buf, Number));
                }
                buf = "";
            }
    }
};