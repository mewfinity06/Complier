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

    GreaterThan,
    LessThan,

    And,
    Or,
    PlusPlus,
    MinusMinus,
    DoubleEquals,

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
    std::string source;
    int index;

    Lexer(std::string source) : source(source) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string buf;

        auto keywords = create_keywords();

        while (source[index] != '\0') {
            // Handle multi char tokens
            // TODO: Expand to recognizing
            if (source[index] == ' ' || source[index] == '\n' || source[index] == '\t') {
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
                index++;
                continue;
            }

            if (isalnum(source[index])) {
                buf += source[index];
            }

            if (source[index] == '/' && source[index + 1] == '/') {
                while (source[index] != '\n') {
                    index++;
                }
            }

            else if (source[index] == '/' && source[index + 1 == '*']) {
                index += 2;
                while (source[index] != '*' && source[index + 1] != '/') {
                    index++;
                }
                index += 2;
            }

            if (source[index] == '"') {
                std::string comment_buf;
                do {
                    comment_buf += source[index];
                    index++;
                } while (source[index] != '"');
                comment_buf += '"';
                tokens.push_back(Token(comment_buf, QuotedString));
                index++;
            }
            else if (source[index] == '\'') {
                std::string comment_buf;
                do {
                    comment_buf += source[index];
                    index++;
                } while (source[index] != '\'');
                comment_buf += '\'';
                tokens.push_back(Token(comment_buf, QuotedString));
                index++;
            }

            if (source[index] == '-' && source[index + 1] == '>') {
                tokens.push_back(Token("->", RightArrow));
                index += 1;
            }
            else if (source[index] == '<' && source[index + 1] == '-') {
                tokens.push_back(Token("<-", LeftArrow));
                index += 2;
            }

            if (source[index] == '=' && source[index + 1] == '=') {
                tokens.push_back(Token("==", DoubleEquals));
                index += 1;
            }
            else if (source[index] == '+' && source[index + 1] == '+') {
                tokens.push_back(Token("++", PlusPlus));
                index += 2;
            }
            else if (source[index] == '-' && source[index + 1] == '-') {
                tokens.push_back(Token("--", MinusMinus));
                index += 2;
            }

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
                    tokens.push_back(Token(source[index], BinOperator));
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
            }
            index++;
        }

        tokens.push_back(Token("EOF", Eof));

        return tokens;
    }

    private:

    std::map<std::string, TokenKind> create_keywords() {
        std::map<std::string, TokenKind> keywords;

        keywords["let"] = Let;
        keywords["const"] = Const;
        keywords["func"] = Func;
        keywords["for"] = For;
        keywords["while"] = While;
        keywords["int"] = Int;
        keywords["string"] = String;

        return keywords;
    }
};