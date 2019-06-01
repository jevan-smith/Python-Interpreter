//
// Created by smith on 3/20/2019.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _string{""}, _name{""}, _keyword{""}, _eof{false}, _eol{false},
                _symbol{'\0'}, _isWholeNumber{false}, _isDecimalNumber{false},
                _symbolString{""}, _indent{false},
                _dedent{false}, _blank{false} {}

void Token::print() const {
    if( eol() )                                 std::cout << " [NEWLINE] " ;
    else if( eof() )                            std::cout << " [ENDMARKER] " ;
    else if( isIndent() )                       std::cout << " [INDENT] " ;
    else if( isDedent() )                       std::cout << " [DEDENT] " ;
    else if( isBlank() )                        std::cout << " [BLANK] " ;
    else if( isDef() )                          std::cout << "def ";
    else if ( isLen() )                         std::cout << "len" ;
    else if ( isAppend() )                      std::cout << "append" ;
    else if ( isPop() )                         std::cout << "pop" ;
    else if ( isIf() )                          std::cout << "if " ;
    else if ( isElse() )                        std::cout << "else ";
    else if ( isElseIf() )                      std::cout << "elif ";
    else if ( isOr() )                          std::cout << " or " ;
    else if ( isAnd() )                         std::cout << " and " ;
    else if ( isNot() )                         std::cout << "not " ;
    else if ( isColon() )                       std::cout << ":" ;
    else if ( isComma() )                       std::cout << ", " ;
    else if ( isQuotation() )                   std::cout << "\"" ;
    else if ( isSingleQuotation() )             std::cout << "\'" ;
    else if( isOpenParen() )                    std::cout << "(" ;
    else if( isCloseParen() )                   std::cout << ")" ;
    else if( isAssignmentOperator() )           std::cout << " = " ;
    else if( isSemiColon() )                    std::cout << "; " ;
    else if( isMultiplicationOperator() )       std::cout << " * " ;
    else if( isAdditionOperator() )             std::cout << " + " ;
    else if( isSubtractionOperator() )          std::cout << " - " ;
    else if( isModuloOperator() )               std::cout << " % " ;
    else if( isDivisionOperator() )             std::cout << " / " ;
    else if( isName() )                         std::cout << getName();
    else if( isWholeNumber() )                  std::cout << getWholeNumber();
    else if( isDecimalNumber() )                std::cout << getDecimalNumber();
    else if( isString() )                       std::cout << getString();
    else if( isGreaterThan() )                  std::cout << " > " ;
    else if( isLessThan() )                     std::cout << " < " ;
    else if( isGreaterThanOrEqual() )           std::cout << " >= " ;
    else if( isLessThanOrEqual() )              std::cout << " <= " ;
    else if( isEqualTo() )                      std::cout << " == " ;
    else if( isNotEqualTo() )                   std::cout << " != " ;
    else if( isNotEqualTo2 () )                 std::cout << " <> " ;
    else if( isFloorDivision() )                std::cout << " // " ;
    else if( isOpenBracket() )                  std::cout << "{" ;
    else if( isClosedBracket() )                std::cout << "}" ;
    else if( isOpenBracket2() )                 std::cout << "[" ;
    else if( isClosedBracket2() )               std::cout << "]" ;
    else if( isDot() )                          std::cout << "." ;
    else if( isKeyword() )                      std::cout << getKeyword() ;
    else                                        std::cout << "Uninitialized token.\n" ;
}
