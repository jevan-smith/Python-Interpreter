//
// Created by smith on 3/20/2019.
//

#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include "Token.hpp"

class Tokenizer {

public:
    Tokenizer(std::ifstream &inStream);
    Token getToken();
    void ungetToken();
    void printProcessedTokens();

private:
    Token lastToken;
    bool ungottenToken;
    std::ifstream &inStream;
    std::vector<Token> _tokens;

private:
    std::string readName();
    std::string readString();
    std::string readComment();
    std::string readNumber();
    bool readIndent();
    bool readDedent();
};

#endif //EXPRINTER_TOKENIZER_HPP
