//
// Created by smith on 3/20/2019.
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "ArithExpr.hpp"
#include "SymTab.hpp"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;

};

// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);

    void print();

private:
    std::vector<Statement *> _statements;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr, bool isArray );
    AssignmentStatement(std::string lhsVar, TypeDescriptor sub, ExprNode *rhsExpr, bool isArray );

    std::string &lhsVariable();
    ExprNode *&rhsExpression();
    bool &isArray();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    TypeDescriptor _sub;
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
    bool _isarray;
};


// PrintStatement

class PrintStatement : public Statement {
public:
    PrintStatement();
    PrintStatement(std::string lhsVar, ExprNode *rhsExpr);

    std::string &lhsVariable();
    ExprNode *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
};

// ForStatement

class ForStatement : public Statement {
public:
    ForStatement();
    ForStatement(ExprNode *firstExpr, ExprNode *secondExpr, Statements *firstState);

    ExprNode *&firstExpr();
    ExprNode *&secondExpr();
    Statements *&firstState();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    ExprNode *_firstExpr;
    ExprNode *_secondExpr;
    Statements *_firstState;
};

// Function

class Function : public Statement {
public:
    Function();
    Function(std::string Expr, std::vector<std::string> parameterList, Statements *statement, bool isCall);
    Function(std::string Expr, Function *func, ExprNode *ExprParam, bool isCall);
    Function(std::string Expr, Function *func, bool isCall);

    std::vector<std::string> &parameterList();
    Statements *&statement();

    void evaluate(SymTab &symTab);
    void print();
    
private:
    std::string _Expr;
    Statements * _statement;
    Function *_func;
    ExprNode *_ExprParam;
    std::vector<std::string> _parameterList;
    bool _isCall;
    TypeDescriptor _returnValue;
};

// Return

class ReturnStatement : public Statement {
public:
    ReturnStatement();
    ReturnStatement(std::string, ExprNode *Expr);
    
    ExprNode *&Expr();
    
    virtual void evaluate(SymTab &symTab);
    virtual void print();
    
private:
    std::string _varName;
    ExprNode *_Expr;
};

// If Statement

class IfStatement : public Statement {
public:

    IfStatement();
    IfStatement(ExprNode *ifExpr, Statements *ifStatements,
                std::vector<ExprNode *> elifExpr,
                std::vector<Statements *> elifStatements, Statements *elseStatements,
                bool isElseTrue);

    ExprNode *&ifExpr();
    Statements *&ifStatements();
    Statements *&elseStatements();

    void evaluate(SymTab &symTab);
    void print();

private:
    ExprNode *_ifExpr;
    Statements *_ifStatements;
    std::vector<ExprNode *> _elifExpr;
    std::vector<Statements *> _elifStatements;
    Statements *_elseStatements;
    bool _isElseTrue;
};

// Array_ops

class Array_ops : public Statement {
public:
    Array_ops();
    Array_ops(std::string lhsVar, ExprNode *rhsExpr, bool isAppend );

    std::string &lhsVariable();
    ExprNode *&rhsExpression();
    bool &isAppend();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
    bool _isappend;
};

#endif //EXPRINTER_STATEMENTS_HPP
