//
// Created by smith on 3/20/2019.
//

#include <iostream>
#include <string>
#include <vector>
#include "Tokenizer.hpp"

static int indentPosition = 0;
static bool newLineHappened = true;
static std::vector<int> savedIndents;
static int position = 0;
static bool quit = false;
static bool skipSpaces = false;
static int dedentCounter = 0;
static bool skipNewline = false;
static bool lockNextCharacter = false;
static bool isDecimal = false;

std::string Tokenizer::readName() {

    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }
    if(inStream.good())
        inStream.putback(c);
    return name;
}

std::string Tokenizer::readString() {

    std::string result;
    char c;

    inStream.get(c);

    if(c == '\"') {
        while( inStream.get(c) && c != '"')  {
            result += c;
        }
    }
    else {
        while( inStream.get(c) && c != '\'')  {
            result += c;
        }
    }
    if(inStream.good())
        inStream.putback(c);
    return result;
}

std::string Tokenizer::readComment() {

    std::string result;
    char c;
    while( inStream.get(c) && c != '\n' ) {
        result += c;
    }
    if(inStream.good())
        inStream.putback(c);
    return result;
}

std::string Tokenizer::readNumber() {

    int numberOfTimesDecimalHappen = 0;
    char lastValue = ' ';
    std::string stringValue;
    char c;
    while( (inStream.get(c) && isdigit(c)) || ( c=='.')) {
        if( c == '.') {
            numberOfTimesDecimalHappen += 1;
            isDecimal = true;
        }
        stringValue += c;
        lastValue = c;
    }

    if (lastValue == '.') {
        stringValue.pop_back();
        isDecimal = false;
    }

    if(numberOfTimesDecimalHappen > 1) {
        std::cout << "Tokenizer::readNumber Error! Cannot have multiple decimals in double!" << std::endl;
        exit(455);
    }

    if(inStream.good())
        inStream.putback(c);
    return stringValue;
}

bool Tokenizer::readIndent() {

    char c;
    position = 0;

    while( inStream.get(c) && isspace(c) && c != '\n') {
        position++;
    }

    if( c == '\n' || isspace(c) || c == '#') {
        quit = true;
    }

    if( quit ) {
        inStream.putback(c);
        return false;
    }

    if(position > indentPosition && position != 0) {
        indentPosition = position;
        savedIndents.push_back(indentPosition);
        inStream.putback(c);
        return true;
    }

    inStream.putback(c);
    return false;
}

bool Tokenizer::readDedent() {

    bool flag = false;
    dedentCounter = 0;

    if( quit ) {
        quit = false;
        return false;
    }

    if(position < indentPosition) {
        flag = true;
        for(int i = savedIndents.size(); i > 0; i--) {
            dedentCounter += 1;
            if (savedIndents[i-1] == position) {
                dedentCounter -= 1;
                indentPosition = savedIndents[savedIndents.size()-1];
                return true;

            }
        }
    }

    if(flag) {
        std::cout << "Dedent Error!" << std::endl;
        exit(343);
    }

    return false;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{} {}

Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    if(savedIndents.empty()) {
        savedIndents.push_back(0);
    }

    skip:
    char c = ' ';

    Token token;

    if( dedentCounter <= 0 && !lockNextCharacter) {
        if(skipSpaces) {
            while (inStream.get(c) && isspace(c) && c != '\n')  // Skip spaces but not new-line chars.
                ;
        }
        else {
            inStream.get(c);
        }
    }
    if(skipNewline)
        if(c != '\n')
            skipNewline = false;

    if ( newLineHappened ) {
        inStream.putback(c);
        newLineHappened = false;
        token.isIndent( readIndent() );
        bool temp = readDedent();

        if( !token.isIndent() && !temp) {
            inStream.get(c);
        }
        if( temp && dedentCounter == 0) {
            inStream.get(c);
        }
        skipSpaces = true;
        skipNewline = true;
    }
    if ( dedentCounter > 0 ) {
        dedentCounter -= 1;
        indentPosition = savedIndents[savedIndents.size()-2];
        savedIndents.pop_back();
        token.isDedent(true);
    }
    else if( inStream.eof() ) {
        if (savedIndents.size() != 1) {
            savedIndents.pop_back();
            token.isDedent(true);
            lockNextCharacter = true;
        }
        else {
            token.eof() = true;
        }
    }
    else if ( isspace(c) && (c != '\n') ) { // MAKES SURE [BLANKS] DONT GET PUSHED
        if( token.isIndent() || token.isDedent()
            || token.eof() || token.eol() ) {
            // Do nothing
        }
        else {
            if (savedIndents.size() != 1) {
                savedIndents.pop_back();
                token.isDedent(true);
                lockNextCharacter = true;
            }
            else {
                token.eof() = true;
            }
        }
    }
    else if( c == '\n' ) {
        if(skipNewline) {
            newLineHappened = true;
            skipSpaces = false;
            goto skip;
        }
        else {

            token.eol() = true;
            newLineHappened = true;
            skipSpaces = false;

        }
    }
    else if( isdigit(c) || c == '.') {

        inStream.putback(c);
        std::string value = readNumber();
        if(!isDecimal) {
            if(value.empty()) {
                token.symbol('.');
            }
            else {
                int intValue = stoi(value);
                token.setWholeNumber( intValue );
            }
        }
        else {
            isDecimal = false;
            double doubleValue = stod(value);
            token.setDecimalNumber( doubleValue );
        }

    }
    else if ( c == '#') {
        readComment();
        goto skip; //skip entire comment line
    }
    else if( c == ',') {
        token.symbol(c);
    }
    else if( c == ':') {
        token.symbol(c);
    }
    else if( c == '"' || c == '\'') {
        inStream.putback(c);
        std::string temp;
        temp = readString();
        token.setString( temp );
        inStream.get(c);
    }
    else if( c == '>' || c == '<') {
        char original = c;
        std::string temp;
        temp += c;
        inStream.get(c);
        if( original == '<' && c == '>') {
            temp += c;
            token.symbolString(temp);
        }
        else if( c == '=' ) {
            temp += c;
            token.symbolString(temp);
        }
        else {
            token.symbol(original);
            inStream.putback(c);
        }
    }
    else if( c == '=' ) {
        char original = c;
        std::string temp;
        temp += c;
        inStream.get(c);
        if( c == '=') {
            temp += c;
            token.symbolString(temp);
        }
        else {
            token.symbol(original);
            inStream.putback(c);
        }
    }
    else if( c == '!' ) {
        std::string temp;
        temp += c;
        inStream.get(c);
        if( c == '=') {
            temp += c;
            token.symbolString(temp);
        }
        else {
            std::cout << "Not Possible ->" << c << "<-" << std::endl;
            exit(1);
        }
    }
    else if( c == '+' || c == '-' || c == '*' || c == '%')
        token.symbol(c);
    else if( c == '/') {
        char original = c;
        inStream.get(c);
        if( c == '/')
            token.symbolString("//");
        else {
            inStream.putback(c);
            token.symbol(original);
        }
    }
    else if( c == ';' )
        token.symbol(c);
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if( c == '{' || c == '}')
        token.symbol(c);
    else if( c == '[' || c == ']')
        token.symbol(c);
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        std::string temp;
        temp = readName();
        if (temp == "for")
            token.setKeyword( temp );
        else if (temp == "print")
            token.setKeyword( temp );
        else if (temp == "in")
            token.setKeyword( temp );
        else if (temp == "range")
            token.setKeyword( temp );
        else if (temp == "or")
            token.symbolString( temp );
        else if (temp == "and")
            token.symbolString( temp );
        else if (temp == "not")
            token.symbolString(temp);
        else if (temp == "if")
            token.symbolString(temp);
        else if (temp == "else")
            token.symbolString(temp);
        else if (temp == "elif")
            token.symbolString(temp);
        else if (temp == "def")
            token.setKeyword(temp);
        else if (temp == "return")
            token.setKeyword(temp);
        else if (temp == "len")
            token.symbolString(temp);
        else if (temp == "append")
            token.symbolString(temp);
        else if (temp == "pop")
            token.symbolString(temp);
        else
            token.setName( temp );
    }
    else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }

    _tokens.push_back(token);
    return lastToken = token;

}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        if(iter->eol())
            std::cout << std::endl;
    }
}
