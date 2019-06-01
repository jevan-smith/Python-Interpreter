//
// Created by smith on 3/20/2019.
//

#include "Statements.hpp"
#include "TypeDescriptor.hpp"
#include "Functions.hpp"
// Scope of variables for within a function.

// Statement
Statement::Statement() {}

// Statements
Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements)
        s->print();
}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

// AssignmentStatement
AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr}, _isarray{false} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr, bool isArray) :
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr},  _isarray{isArray}{}

AssignmentStatement::AssignmentStatement(std::string lhsVar, TypeDescriptor sub, ExprNode *rhsExpr, bool isArray) :
        _lhsVariable{lhsVar}, _sub(sub), _rhsExpression{rhsExpr}, _isarray{isArray}{}

void AssignmentStatement::evaluate(SymTab &symTab) {
    if(!isArray()) {
        TypeDescriptor rhs = rhsExpression()->evaluate(symTab);
        symTab.setValueFor(lhsVariable(), rhs);
    }
    else {
        if (rhsExpression() != nullptr) {
            if(_sub.getTypeValue() != 5) {
                TypeDescriptor result;
                TypeDescriptor rhs = rhsExpression()->evaluate(symTab);
                result = symTab.getValueFor(_lhsVariable);
                result[_sub.getIntValue()] = rhs;
                symTab.setValueFor(lhsVariable(), result);
            }
            else {
                TypeDescriptor result;
                TypeDescriptor rhs = rhsExpression()->evaluate(symTab);
                result = rhs.turnIntoArray();
                symTab.setValueFor(lhsVariable(), result);
            }
        } else {
            TypeDescriptor result;
            result.createArray();
            symTab.setValueFor(lhsVariable(), result);
        }
    }
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

bool &AssignmentStatement::isArray() {
    return _isarray;
}

void AssignmentStatement::print() {
    bool isString = false;
    if(_rhsExpression->token().isString())
        isString = true;
    if(isString) {
        std::cout << _lhsVariable << " = ";
        std::cout << "\"";
        _rhsExpression->print();
        std::cout << "\"";
        std::cout << std::endl;
    }
    else {
        std::cout << _lhsVariable << " = ";
        _rhsExpression->print();
        std::cout << std::endl;
    }
}

// PrintStatement
PrintStatement::PrintStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

PrintStatement::PrintStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void PrintStatement::evaluate(SymTab &symTab) {
    TypeDescriptor rhs = rhsExpression()->evaluate(symTab);
    if(rhs.getTypeValue() == 4) {
        std::cout << rhs << std::endl;
    }
    else {
        TypeDescriptor commasRemoved;
        commasRemoved = rhs.removeComma(rhs);
        std::cout << commasRemoved << std::endl;
    }
}

std::string &PrintStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&PrintStatement::rhsExpression() {
    return _rhsExpression;
}

void PrintStatement::print() {
    std::cout << _lhsVariable << " ";
    _rhsExpression->print();
    std::cout << std::endl;
}

// ForStatement
ForStatement::ForStatement() : _firstExpr{nullptr}, _secondExpr{nullptr}, _firstState{nullptr} {}

ForStatement::ForStatement(ExprNode *expr1, ExprNode *expr2, Statements *State):
        _firstExpr{expr1}, _secondExpr{expr2}, _firstState{State} {}


void ForStatement::evaluate(SymTab &symTab) {

    range rangeValue;
    TypeDescriptor values = secondExpr()->evaluate(symTab);
    TypeDescriptor removeCommas;
    removeCommas = values.removeComma(values);
    TypeDescriptor currentIteratorValue;

    rangeValue.setValues(removeCommas);

    currentIteratorValue = rangeValue.getStartValue();
    const int stepValue = rangeValue.getStepValue();

    Token id = firstExpr()->token();

    if(stepValue < 0) {
        while (rangeValue.getStartValue() > rangeValue.getEndValue()) {

            symTab.setValueFor(id.getName(), currentIteratorValue);

            firstExpr()->evaluate(symTab);

            firstState()->evaluate(symTab); // Evaluates statement

            rangeValue.next(); //iterates to next value

            currentIteratorValue = rangeValue.getStartValue(); //updates currentIteratorValue
        }
    }
    else {
        while (rangeValue.getStartValue() < rangeValue.getEndValue()) {

            symTab.setValueFor(id.getName(), currentIteratorValue);

            firstExpr()->evaluate(symTab);

            firstState()->evaluate(symTab); // Evaluates statement

            rangeValue.next(); //iterates to next value

            currentIteratorValue = rangeValue.getStartValue(); //updates currentIteratorValue
        }
    }
}

ExprNode *&ForStatement::firstExpr() {
    return _firstExpr;
}

ExprNode *&ForStatement::secondExpr() {
    return _secondExpr;
}

Statements *&ForStatement::firstState() {
    return _firstState;
}

void ForStatement::print() {
    std::cout << "for ";
    _firstExpr->print();
    std::cout << " in range(";
    _secondExpr->print();
    std::cout << "):" << std::endl;
    std::cout << "[INDENT]" << std::endl;
    _firstState->print(), std::cout << "[DEDENT]" << std::endl;
}

// Function
Function::Function() : _Expr{""}, _statement{nullptr}, _parameterList{nullptr}, _isCall{false} {}
// For parsing a function.
Function::Function(std::string Expr, std::vector<std::string> parameterList, Statements *statement, bool isCall) : _Expr{Expr}, _parameterList{parameterList}, _statement{statement}, _isCall{isCall} {}

// For function calls which the function has parameters.
Function::Function(std::string Expr, Function *func, ExprNode *ExprParam, bool isCall) :
    _Expr{Expr},_func{func}, _ExprParam{ExprParam}, _isCall{isCall}  {}

// For function calls which the function has no parameters.
Function::Function(std::string Expr,Function *func, bool isCall) : _Expr{Expr},_func{func}, _isCall{isCall}  {}

std::vector<std::string> &Function::parameterList() {
    return _parameterList;
}
Statements *&Function::statement() {
    return _statement;
}

void Function::evaluate(SymTab &symTab) {
    if(_isCall){
        SymTab newSymTab = symTab; // save the old symTab for later
        std::vector<std::string> params;
        std::vector<TypeDescriptor> values;
        if(_ExprParam != NULL){
            for(int i =0; i < _func->_parameterList.size(); i++){
                TypeDescriptor rhs = _ExprParam->evaluate(symTab);
                rhs = rhs.turnIntoArray();
                if (rhs.length() == 1) {
                    TypeDescriptor newRhs = _ExprParam->evaluate(symTab);
                    if(newRhs.getTypeValue() == 4) {
                        params.push_back(_func->parameterList()[i]);
                    }
                    symTab.setValueFor(_func->parameterList()[i], newRhs);
                } else {
                    if(rhs[i].getTypeValue() == 4) {
                        params.push_back(_func->parameterList()[i]);
                    }
                    symTab.setValueFor(_func->parameterList()[i], rhs[i]);
                }
            }
        }
        _func->statement()->evaluate(symTab);
        if(params.size() != 0){
            for(int i = 0; i < params.size(); i++) {
                values.push_back(symTab.getValueFor(params[i]));
            }
        }
        symTab = newSymTab;
        if(params.size() != 0){
            for(int i = 0; i < params.size(); i++) {
                symTab.setValueFor(params[i], values[i]);
            }
        }
    }
}

void Function::print() {
    if(_isCall){
        std::cout << _Expr << "(";
        if(_ExprParam != NULL) {
            _ExprParam->print();
        }
        std::cout << ")" << std::endl;
    }
    else{
        std::cout << "def " << _Expr;
        std::cout << ":" << std::endl;
        _statement->print();
    }
}

// Return Statement
ReturnStatement::ReturnStatement() : _varName{""}, _Expr{nullptr} {}

ReturnStatement::ReturnStatement(std::string Name, ExprNode *Expr) : _varName{Name}, _Expr{Expr} {}

ExprNode *&ReturnStatement::Expr() {
    return _Expr;
}

void ReturnStatement::evaluate(SymTab &symTab) {
    TypeDescriptor Exp = Expr()->evaluate(symTab);
    symTab.setValueFor(_varName, Exp);
}

void ReturnStatement::print() {
    std::cout << "return ";
    _Expr->print();
    std::cout<<std::endl;
}

// IfStatement
IfStatement::IfStatement() : _ifExpr{nullptr}, _ifStatements{nullptr}, _elifExpr{nullptr},
                             _elifStatements{nullptr}, _elseStatements{nullptr}, _isElseTrue{"false"} {}

IfStatement::IfStatement(ExprNode *ifExpr, Statements *ifStatements, std::vector<ExprNode *> elifExpr,
                         std::vector<Statements *> elifStatements, Statements *elseStatements,
                         bool isElseTrue):
        _ifExpr{ifExpr}, _ifStatements{ifStatements}, _elifExpr{elifExpr},
        _elifStatements{elifStatements}, _elseStatements{elseStatements},
        _isElseTrue{isElseTrue} {}

ExprNode *&IfStatement::ifExpr() {
    return _ifExpr;
}

Statements *&IfStatement::ifStatements() {
    return _ifStatements;
}

Statements *&IfStatement::elseStatements() {
    return _elseStatements;
}

void IfStatement::evaluate(SymTab &symTab) {
    TypeDescriptor ifExpression = ifExpr()->evaluate(symTab);
    bool skip = false;

    if(ifExpression.getBoolValue()) {
        skip = true;
        ifStatements()->evaluate(symTab);
    }
    else if(_elifExpr.size()) {
        for (auto i = 0; i < _elifExpr.size(); i++) {
            TypeDescriptor elifExpression = _elifExpr[i]->evaluate(symTab);
            if(elifExpression.getBoolValue()) {
                skip = true;
                _elifStatements[i]->evaluate(symTab);
                break;
            }
        }
    }
    if (!skip && _isElseTrue){
        elseStatements()->evaluate(symTab);
    }
}

void IfStatement::print() {
    std::cout << "if ";
    ifExpr()->print();
    std::cout << ":" << std::endl;
    ifStatements()->print();
    for(auto i = 0; i < _elifExpr.size(); i++) {
        std::cout << "elif ", _elifExpr[i]->print();
        std::cout << ":" << std::endl;
        _elifStatements[i]->print();
    }
    if(_elseStatements) {
        std::cout << "else";
        std::cout << ":" << std::endl;
        elseStatements()->print();
    }
}

// Array_ops
Array_ops::Array_ops() : _lhsVariable{""}, _rhsExpression{nullptr}, _isappend{false}{}

Array_ops::Array_ops(std::string lhsVar, ExprNode *rhsExpr, bool isAppend) :
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr},  _isappend{isAppend}{}

void Array_ops::evaluate(SymTab &symTab) {
    TypeDescriptor rhs;
    TypeDescriptor value;
    TypeDescriptor result;
    if(isAppend()) {
        rhs = rhsExpression()->evaluate(symTab);
        value = symTab.getValueFor(lhsVariable());
        result.createArray();
        result = value.push(rhs);
        symTab.setValueFor(lhsVariable(), result);
    }
    else {
        value = symTab.getValueFor(lhsVariable());
        result = value.pop();
        symTab.setValueFor(lhsVariable(), result);
    }
}

std::string &Array_ops::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&Array_ops::rhsExpression() {
    return _rhsExpression;
}

bool &Array_ops::isAppend() {
    return _isappend;
}

void Array_ops::print() {

}
