//
// Created by smith on 3/20/2019.
//

#include<vector>
#include<iostream>
#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"
#include "SymTab.hpp"

static Token savedToken;
static Token primaryToken;
bool firstRun = true; // Are we on the firstLine of the program? (true = yes, false = no)
bool parsedAfunc = false;
std::string current_funcName;
std::vector<std::map<std::string, Function*> > symTabFunc;
std::map<std::string, Function*> ScopeFunc;

extern bool inScope(std::string name){
    if(symTabFunc.size() > 0){
        int current = symTabFunc.size() - 1;
        return symTabFunc[current].find(name) != symTabFunc[current].end() || ScopeFunc.find(name) != ScopeFunc.end();
    }
    return ScopeFunc.find(name) != ScopeFunc.end();
}

extern Function *findFunction(std::string name){
    if(symTabFunc.size() > 0){
        for(int i = 0; i < symTabFunc.size(); i++)
            if(symTabFunc[i].find(name) != symTabFunc[i].end())
                return symTabFunc[i].find(name)->second;
    }
    return ScopeFunc.find(name)->second;
}

// Parser functions
void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::statements() {

    Statements *value = new Statements();
    Token tok = tokenizer.getToken();

    if(firstRun && tok.isIndent()) {
        std::cout << "Cannot indent first line, exiting program!" << std::endl;
        exit(1000);
    }

    while(tok.eol() || tok.isKeyword() || tok.isName() || tok.isIf() ||
    tok.isElse() || tok.isElseIf() || tok.isName()) { // {NEWLINE | stmt}*
        parsedAfunc = false;

        if(tok.eol() ) { // NEWLINE
            //DO NOTHING
        }
        else if (tok.isKeyword() || tok.isName() || tok.isIf() ||
        tok.isElse() || tok.isElseIf()) { // stmt
            tokenizer.ungetToken();
            Statement *state = statement();
            value->addStatement(state);
        }
        tok = tokenizer.getToken();
        if(parsedAfunc){
            std::cout<<symTabFunc.size()<<std::endl;
            symTabFunc.push_back(ScopeFunc);
            ScopeFunc.clear();
        }
    }

    firstRun = false;

    tokenizer.ungetToken();
    return value;
}

Statement *Parser::statement() {

    Token tok = tokenizer.getToken();
    if(tok.isIf()) {
        tokenizer.ungetToken();
        return ifStatement();
    }
    else if (tok.isKeyword()) {
        if (tok.getKeyword() == "print") {
            tokenizer.ungetToken();
            return printStatement();
        }
        else if (tok.getKeyword() == "for") {
            tokenizer.ungetToken();
            return forStatement();
        }
        else if (tok.getKeyword() == "def") {
            parsedAfunc = true;
            tokenizer.ungetToken();
            return function_Def();
        }
        else if (tok.getKeyword() == "return") {
            tokenizer.ungetToken();
            return return_stmt();
        }
    }
    else if(tok.isName()) {
        savedToken = tok;
        Token tokCheck = tokenizer.getToken();
        if(tokCheck.isDot()) {
            return array_ops();
        }
        else if(inScope(savedToken.getName())){ //if function is by itself Ex: "foo()"
            tokenizer.ungetToken();
            return call_stmt();
        }
        else {
            tokenizer.ungetToken();
            return assignStatement();
        }
    }
    else {
        die("Parser::statement:", "Unexpected token, got", tok);
    }

    return nullptr; //Should never be reached
}

Array_ops *Parser::array_ops() {

    Token check = tokenizer.getToken();
    if (check.isAppend()) {
        Token openParen = tokenizer.getToken();
        if (!openParen.isOpenParen())
            die("Parser::array_ops", "Expected a Keyword '(' token, instead got", openParen);

        ExprNode *value = or_test();

        Token closeParen = tokenizer.getToken();
        if (!closeParen.isCloseParen())
            die("Parser::array_ops", "Expected a Keyword ')' token, instead got", closeParen);


        return new Array_ops(savedToken.getName(), value, true);

    }
    else if (check.isPop()) {
        Token openParen = tokenizer.getToken();
        if (!openParen.isOpenParen())
            die("Parser::array_ops", "Expected a Keyword '(' token, instead got", openParen);

        Token closeParen = tokenizer.getToken();
        if (!closeParen.isCloseParen())
            die("Parser::array_ops", "Expected a Keyword ')' token, instead got", closeParen);


        return new Array_ops(savedToken.getName(), nullptr, false);
    }
    else {
        die("Parser::array_ops", "Expected an 'append' or 'pop' token, instead got", check);
    }

    return nullptr; //Should never be reached
}

IfStatement *Parser::ifStatement() {

    std::vector<ExprNode *> elifExprVector;
    std::vector<Statements *> elifAssignVector;
    bool isElseTrue = false;

    Token varIf = tokenizer.getToken();
    if (!varIf.isIf())
        die("Parser::ifStatement", "Expected a Keyword 'if' token, instead got", varIf);

    ExprNode *ifExpr = or_test();

    Token varColon = tokenizer.getToken();
    if (!varColon.isColon())
        die("Parser::ifStatement", "Expected a ':' token, instead got", varColon);

    Token varNewline = tokenizer.getToken();
    if (!varNewline.eol())
        die("Parser::ifStatement", "Expected a 'NEWLINE' token, instead got", varNewline);

    Token varIndent = tokenizer.getToken();
    if (!varIndent.isIndent())
        die("Parser::ifStatement", "Expected a 'INDENT' token, instead got", varIndent);

    Statements *ifStatements = statements();

    Token varDedent = tokenizer.getToken();
    if (!varDedent.isDedent())
        die("Parser::ifStatement", "Expected a 'DEDENT' token, instead got", varDedent);

    Token varElseIf = tokenizer.getToken();
    while (varElseIf.isElseIf()) {
        bool isElseTrue = true;
        ExprNode *elifExpr = or_test();
        elifExprVector.push_back(elifExpr);

        Token varColonTwo = tokenizer.getToken();
        if (!varColonTwo.isColon())
            die("Parser::ifStatement", "Expected a ':' token, instead got", varColonTwo);

        Token varNewlineTwo = tokenizer.getToken();
        if (!varNewlineTwo.eol())
            die("Parser::ifStatement", "Expected a 'NEWLINE' token, instead got", varNewlineTwo);

        Token varIndentTwo = tokenizer.getToken();
        if (!varIndentTwo.isIndent())
            die("Parser::ifStatement", "Expected a 'INDENT' token, instead got", varIndentTwo);

        Statements *elifStatements = statements();
        elifAssignVector.push_back(elifStatements);

        Token varDedentTwo = tokenizer.getToken();
        if (!varDedentTwo.isDedent())
            die("Parser::ifStatement", "Expected a 'DEDENT' token, instead got", varDedentTwo);

        varElseIf = tokenizer.getToken();
    }

    if(varElseIf.isElse()) {
        bool isElseTrue = true;
        Token varColonThree = tokenizer.getToken();
        if (!varColonThree.isColon())
            die("Parser::ifStatement", "Expected a ':' token, instead got", varColonThree);

        Token varNewlineThree = tokenizer.getToken();
        if (!varNewlineThree.eol())
            die("Parser::ifStatement", "Expected a 'NEWLINE' token, instead got", varNewlineThree);

        Token varIndentThree = tokenizer.getToken();
        if (!varIndentThree.isIndent())
            die("Parser::ifStatement", "Expected a 'INDENT' token, instead got", varIndentThree);

        Statements *elseStatements = statements();

        Token varDedentThree = tokenizer.getToken();
        if (!varDedentThree.isDedent())
            die("Parser::ifStatement", "Expected a 'DEDENT' token, instead got", varDedentThree);

        return new IfStatement(ifExpr, ifStatements, elifExprVector,
                               elifAssignVector, elseStatements, isElseTrue);
    }
    else {
        tokenizer.ungetToken();
        Statements *elseStatements = nullptr;
        return new IfStatement(ifExpr, ifStatements, elifExprVector,
                               elifAssignVector, elseStatements, isElseTrue);
    }

}

ForStatement *Parser::forStatement() {

    Token varKeyword = tokenizer.getToken();
    if (!varKeyword.isKeyword())
        die("Parser::forStatement", "Expected a Keyword token, instead got", varKeyword);

    ExprNode *Expr1 = id();

    Token varIsInKeyword = tokenizer.getToken();
    if (varIsInKeyword.getKeyword() != "in")
        die("Parser::forStatement", "Expected a Keyword 'in' token, instead got", varIsInKeyword);

    Token varIsRangeKeyword = tokenizer.getToken();
    if (varIsRangeKeyword.getKeyword() != "range")
        die("Parser::forStatement", "Expected a Keyword 'range' token, instead got", varIsRangeKeyword);

    Token varIsOpenParen = tokenizer.getToken();
    if (!varIsOpenParen.isOpenParen())
        die("Parser::forStatement", "Expected a '(' token, instead got", varIsOpenParen);

    ExprNode *Expr2 = test();

    Token varIsClosedParen = tokenizer.getToken();
    if (!varIsClosedParen.isCloseParen())
        die("Parser::forStatement", "Expected a ')' token, instead got", varIsClosedParen);

    Token varColon = tokenizer.getToken();
    if (!varColon.isColon())
        die("Parser::forStatement", "Expected a Colon ':' token, instead got", varColon);

    Token varNewline = tokenizer.getToken();
    if (!varNewline.eol())
        die("Parser::forStatement", "Expected a 'NEWLINE' token, instead got", varNewline);

    Token varIndent = tokenizer.getToken();
    if (!varIndent.isIndent())
        die("Parser::forStatement", "Expected a 'INDENT' token, instead got", varIndent);

    Statements *State = statements();

    Token varDedent = tokenizer.getToken();
    if (!varDedent.isDedent())
        die("Parser::forStatement", "Expected a 'DEDENT' token, instead got", varDedent);

    return new ForStatement(Expr1, Expr2, State);
}

AssignmentStatement *Parser::assignStatement() {

    Token openBracker = tokenizer.getToken();
    if(openBracker.isOpenBracket2()) {

        Token subValue = tokenizer.getToken();

        Token varClosed2 = tokenizer.getToken();
        if (!varClosed2.isClosedBracket2())
            die("Parser::assignStatement", "Expected an ']', instead got", varClosed2);

        Token assignOp = tokenizer.getToken();
        if (!assignOp.isAssignmentOperator())
            die("Parser::assignStatement", "Expected an '=', instead got", assignOp);

        ExprNode *rightHandSideExpr = test();
        TypeDescriptor value;
        value = subValue.getWholeNumber();

        return new AssignmentStatement(savedToken.getName(), value, rightHandSideExpr, true);

    }
    else {
        tokenizer.ungetToken();
        Token assignOp = tokenizer.getToken();
        if (!assignOp.isAssignmentOperator())
            die("Parser::assignStatement", "Expected an '=', instead got", assignOp);

        Token openBracket = tokenizer.getToken();
        if (openBracket.isOpenBracket2()) {

            Token varClosed = tokenizer.getToken();
            if (varClosed.isClosedBracket2())
                return new AssignmentStatement(savedToken.getName(), nullptr, true);
            else {
                tokenizer.ungetToken();
                ExprNode *rightHandSideExpr = test();

                Token varClosed2 = tokenizer.getToken();
                if (!varClosed2.isClosedBracket2())
                    die("Parser::assignStatement", "Expected an ']', instead got", varClosed2);

                return new AssignmentStatement(savedToken.getName(), rightHandSideExpr, true);
            }
        } else {
            tokenizer.ungetToken();
            ExprNode *rightHandSideExpr = or_test();
            return new AssignmentStatement(savedToken.getName(), rightHandSideExpr, false);
        }
    }

    return nullptr; // will never reach
}

PrintStatement *Parser::printStatement() {

    Token varKeyword = tokenizer.getToken();
    if (!varKeyword.isKeyword())
        die("Parser::printStatement", "Expected a Keyword token, instead got", varKeyword);

    ExprNode *rightHandSideExpr = test();

    return new PrintStatement(varKeyword.getKeyword(), rightHandSideExpr);
}

Function *Parser::function_Def() {
    
    std::vector<std::string> parameterList;
    
    Token varKeyword = tokenizer.getToken();
    if (!varKeyword.isKeyword())
        die("Parser::function_Def", "Expected a Keyword token, instead got", varKeyword);
    
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::function _Def", "Expected a name token, instead got", varName);
    
    current_funcName = varName.getName();
    
    Token varIsOpenParen = tokenizer.getToken();
    if (!varIsOpenParen.isOpenParen())
        die("Parser::function_Def", "Expected a '(' token, instead got", varIsOpenParen);
    
    Token varParam = tokenizer.getToken();
    while (varParam.isName()) {
        parameterList.push_back(varParam.getName());
        
        Token varNext = tokenizer.getToken();
        if (varNext.isComma())
            varParam = tokenizer.getToken();
        else
            break;
    }

    tokenizer.ungetToken();
    
    varParam = tokenizer.getToken();
    if (!varParam.isCloseParen())
        die("Parser::function_Def", "Expected a ')' token, instead got", varParam);
    
    Token varColon = tokenizer.getToken();
    if (!varColon.isColon())
        die("Parser::function_Def", "Expected a ':' token, instead got", varColon);
    
    Token varNewline = tokenizer.getToken();
    if (!varNewline.eol())
        die("Parser::function_Def", "Expected a 'NEWLINE' token, instead got", varNewline);
    
    Token varIndent = tokenizer.getToken();
    if (!varIndent.isIndent())
        die("Parser::function_Def", "Expected a 'INDENT' token, instead got", varIndent);

    Statements *stmnts = statements();
    
    Token varDedent = tokenizer.getToken();
    if (!varDedent.isDedent())
        die("Parser::function_Def", "Expected a 'DEDENT' token, instead got", varDedent);

    Function *newFunc = new Function(varName.getName(), parameterList, stmnts, false);
    ScopeFunc.insert(std::pair<std::string, Function*> (varName.getName(),newFunc));

    return newFunc;
}

Function *Parser::call_stmt() { // stand alone function call

    TypeDescriptor returnValue;

    if (!savedToken.isName())
        die("Parser::call_stmt", "Expected a name token, instead got", savedToken);

    Token varIsOpenParen = tokenizer.getToken();
    if (!varIsOpenParen.isOpenParen())
        die("Parser::call_stmt", "Expected a '(' token, instead got", varIsOpenParen);

    Token varParam = tokenizer.getToken();
    if(!varParam.isCloseParen()) {
        tokenizer.ungetToken();
        ExprNode *ExprParam = test();
        Token varParam2 = tokenizer.getToken();

        if (!varParam2.isCloseParen())
            die("Parser::call_stmt", "Expected a ')' token, instead got", varParam2);

        if(!inScope(savedToken.getName()))
            die("Parser::call_stmt", "Function was not declared in this scope ", savedToken);

        return new Function(savedToken.getName(),findFunction(savedToken.getName()), ExprParam, true);
    }
    else {
        if (!varParam.isCloseParen())
            die("Parser::call_stmt", "Expected a ')' token, instead got", varParam);

        if(!inScope(savedToken.getName()))
            die("Parser::call_stmt", "Function was not declared in this scope ", savedToken);

        return new Function(savedToken.getName(),findFunction(savedToken.getName()), true);
    }
}

ExprNode *Parser::call() { // function call exprNode

    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::call", "Expected a name token, instead got", varName);

    Token varIsOpenParen = tokenizer.getToken();
    if (!varIsOpenParen.isOpenParen())
        die("Parser::call", "Expected a '(' token, instead got", varIsOpenParen);

    Token varCloseParen = tokenizer.getToken();
    if(!varCloseParen.isCloseParen()) {
        tokenizer.ungetToken();
        ExprNode *ExprParam = test();

        Token varParam = tokenizer.getToken();
        if (!varParam.isCloseParen())
            die("Parser::call", "Expected a ')' token, instead got", varParam);

        if (!inScope(varName.getName()))
            die("Parser::call", "Function does not exist in scope. ", varName);

        return new FunctionExpr(varName, ExprParam);
    }
    else {

        if (!varCloseParen.isCloseParen())
            die("Parser::call_stmt", "Expected a ')' token, instead got", varCloseParen);

        if (!inScope(varName.getName()))
            die("Parser::call", "Function does not exist in scope. ", varName);

        return new FunctionExpr(varName, nullptr);
    }
}

ReturnStatement *Parser::return_stmt() {
    
    Token varKeyword = tokenizer.getToken();
    if (!varKeyword.isKeyword())
        die("Parser::return_stmt", "Expected a Keyword token, instead got", varKeyword);
    
    ExprNode *Expr = or_test();
    return new ReturnStatement(current_funcName,Expr);
}

ExprNode *Parser::test() {

    ExprNode *left = or_test();
    Token tok = tokenizer.getToken();
    while ( tok.isComma() ) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = or_test();
        left = p;
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;

}

ExprNode *Parser::or_test() {

    ExprNode *left = and_test();
    Token tok = tokenizer.getToken();
    while ( tok.isOr() ) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = and_test();
        left = p;
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::and_test() {

    ExprNode *left = not_test();
    Token tok = tokenizer.getToken();
    while ( tok.isAnd() ) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = not_test();
        left = p;
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;

}

ExprNode *Parser::not_test() {

    Token tok = tokenizer.getToken();

    if ( tok.isNot() ) {
        ExprNode *right = not_test();
        PrefixExprNode *p = new PrefixExprNode(tok);
        p->right() = right;
        right = p;
        tokenizer.ungetToken();
        return right;
    }
    else {
        tokenizer.ungetToken();
        ExprNode *right = rel_expr();
        return right;
    }

    return nullptr;  // Will not reach this statement!
}


ExprNode *Parser::rel_expr() {

    ExprNode *left = expr();
    Token tok = tokenizer.getToken();
    while (tok.isEqualTo() || tok.isNotEqualTo() || tok.isNotEqualTo2() || tok.isGreaterThan() ||
    tok.isGreaterThanOrEqual() || tok.isLessThan() || tok.isLessThanOrEqual()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = expr();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::expr() {

    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::term() {

    ExprNode *left = factor();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()
    || tok.isFloorDivision()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = factor();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::factor() {

    Token tok = tokenizer.getToken();

    if(tok.isSubtractionOperator()) {
        ExprNode *right = factor();
        PrefixExprNode *p = new PrefixExprNode(tok);
        p->right() = right;
        right = p;
        return right;
    }
    else {
        if(tok.isName()){
            if(inScope(tok.getName())){
                tokenizer.ungetToken();
                return call();
            }
        }
        else if(tok.isLen()) {
            return array_len();
        }
        tokenizer.ungetToken();
        return primary();
    }

}

ExprNode *Parser::array_len() {
    Token varOpen = tokenizer.getToken();
    if (!varOpen.isOpenParen())
        die("Parser::array_len", "Expected a '(' token, instead got", varOpen);

    Token tok = tokenizer.getToken();
    if (!tok.isName())
        die("Parser::array_len", "Expected a 'id' token, instead got", tok);

    Token varClosed = tokenizer.getToken();
    if (!varClosed.isCloseParen())
        die("Parser::array_len", "Expected a ')' token, instead got", varClosed);
    return new Array_len(tok);
}

ExprNode *Parser::subscription() {

    ExprNode *left = or_test();

    Token varClosed = tokenizer.getToken();
    if (!varClosed.isClosedBracket2())
        die("Parser::subscription", "Expected a ']' token, instead got", varClosed);

    return new Subscript(primaryToken, left);
}

ExprNode *Parser::id() {

    Token tok = tokenizer.getToken();

    if (tok.isName()) {
        return new Variable(tok);
    }
    else {
        die("Parser::id", "Unexpected token", tok);
    }

    return nullptr; //will never hit
}

ExprNode *Parser::primary() {

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return new WholeNumber(tok);
    else if( tok.isDecimalNumber() )
        return new DecimalNumber(tok);
    else if( tok.isName()) {
        Token tok2 = tokenizer.getToken();
        if(tok2.isOpenBracket2()) { //is it a subscript
            primaryToken = tok;
            return subscription();
        }
        else {
            tokenizer.ungetToken();
            return new Variable(tok); //is it a variable
        }
    }
    else if( tok.isKeyword())
        return new Keyword(tok);
    else if( tok.isString())
        return new String(tok);
    else if (tok.eof()) {
        std::cout << "Error" << std::endl;
        exit(3432);
    }
    else if (tok.isOpenParen()) {
        ExprNode *p = or_test();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
