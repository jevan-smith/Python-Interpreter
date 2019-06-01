//
// Created by smith on 3/20/2019.
//

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include<string>

class Token {

public:
    Token();

    bool &eof()                           { return _eof; }
    bool &eol()                           { return _eol; }

    bool eof() const                      { return _eof; }
    bool eol() const                      { return _eol; }

    bool isOpenParen() const              { return _symbol == '('; }
    bool isCloseParen() const             { return _symbol == ')'; }

    void symbol(char c)                   { _symbol = c; }
    char symbol()                         { return _symbol; }

    void symbolString(std::string c)      { _symbolString = c; }
    std::string symbolString()            { return _symbolString; }

    void isIndent(bool c)                 { _indent = c; }
    void isDedent(bool c)                 { _dedent = c; }
    bool isIndent() const                 { return _indent; }
    bool isDedent() const                 { return _dedent; }
    bool &isBlank()                       { return _blank; }
    bool isBlank() const                  { return _blank; }

    bool isDef() const                    { return _symbolString == "def"; }
    bool isLen() const                    { return _symbolString == "len"; }
    bool isAppend() const                 { return _symbolString == "append"; }
    bool isPop() const                    { return _symbolString == "pop"; }
    bool isIf() const                     { return _symbolString == "if"; }
    bool isElse() const                   { return _symbolString == "else"; }
    bool isElseIf() const                 { return _symbolString == "elif"; }
    bool isOr() const                     { return _symbolString == "or"; }
    bool isAnd() const                    { return _symbolString == "and"; }
    bool isNot() const                    { return _symbolString == "not"; }
    bool isColon() const                  { return _symbol == ':'; }
    bool isComma() const                  { return _symbol == ','; }
    bool isQuotation() const              { return _symbol == '"'; }
    bool isSingleQuotation() const        { return _symbol == '\''; }
    bool isGreaterThanOrEqual() const     { return _symbolString == ">="; }
    bool isLessThanOrEqual() const        { return _symbolString == "<="; }
    bool isEqualTo() const                { return _symbolString == "=="; }
    bool isNotEqualTo() const             { return _symbolString == "!="; }
    bool isNotEqualTo2() const            { return _symbolString == "<>"; }
    bool isFloorDivision() const          { return _symbolString == "//"; }
    bool isSemiColon() const              { return _symbol == ';'; }
    bool isGreaterThan() const            { return _symbol == '>'; }
    bool isLessThan() const               { return _symbol == '<'; }
    bool isAssignmentOperator() const     { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isOpenBracket() const            { return _symbol == '{'; }
    bool isClosedBracket() const          { return _symbol == '}'; }
    bool isOpenBracket2() const           { return _symbol == '['; }
    bool isClosedBracket2() const         { return _symbol == ']'; }
    bool isDot() const                    { return _symbol == '.'; }

    bool isKeyword() const                { return _keyword.length() > 0; }
    std::string getKeyword() const        { return _keyword; }
    void setKeyword(std:: string n)       { _keyword = n; }

    bool isName() const                   { return _name.length() > 0; }
    std::string getName() const           { return _name; }
    void setName(std::string n)           { _name = n; }

    bool isString() const                 { return _string.length() > 0; }
    std::string getString() const         { return _string; }
    void setString(std::string n)         { _string = n; }

    bool &isWholeNumber()                 { return _isWholeNumber; }
    bool isWholeNumber() const            { return _isWholeNumber; }
    int getWholeNumber() const            { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

    bool &isDecimalNumber()               { return _isDecimalNumber; }
    bool isDecimalNumber() const          { return _isDecimalNumber; }
    double getDecimalNumber() const       { return _decimalNumber; }
    void setDecimalNumber(double n) {
        _decimalNumber = n;
        isDecimalNumber() = true;
    }

    void print() const;

private:
    std::string _name;
    std::string _keyword;
    std::string _string;
    bool _eof, _eol;
    bool _indent;
    bool _dedent;
    bool _blank;
    bool _isWholeNumber;
    bool _isDecimalNumber;
    char _symbol;
    std::string _symbolString;
    int _wholeNumber;
    double _decimalNumber;
};

#endif //EXPRINTER_TOKEN_HPP
