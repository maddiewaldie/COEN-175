/*
 * File:	checker.h
 *
 * Description:	This file contains the public function declarations for the
 *		semantic checker for Simple C.
 */

# ifndef CHECKER_H
# define CHECKER_H
# include <iostream>
# include "Scope.h"

using namespace std;

Scope *openScope();
Scope *closeScope();

Symbol *defineFunction(const std::string &name, const Type &type);
Symbol *declareFunction(const std::string &name, const Type &type);
Symbol *declareVariable(const std::string &name, const Type &type);
Symbol *checkIdentifier(const std::string &name);

// objective 3.1
Type checkIf(const Type &left);
Type checkWhile(const Type &left);
Type checkFor(const Type &left);
Type checkLogical(const Type &left, const Type &right, const string &operand);
Type checkLogicalOr(const Type &left, const Type &right);
Type checkLogicalAnd(const Type &left, const Type &right);
Type checkNot(const Type &right);

// objective 3.2
Type checkMultiplicativeExpressions(const Type &left, const Type &right, const string &operand);
Type checkMultiply(const Type &left, const Type &right);
Type checkDivide(const Type &left, const Type &right);
Type checkMod(const Type &left, const Type &right);
Type checkNeg(const Type &right);

// objective 3.3
Type checkRelationalExpressions(const Type &left, const Type &right, const string &operand);
Type checkLessThan(const Type &left, const Type &right);
Type checkGreaterThan(const Type &left, const Type &right);
Type checkLessThanEqualTo(const Type &left, const Type &right);
Type checkGreaterThanEqualTo(const Type &left, const Type &right);

// objective 3.4
Type checkEquals(const Type &left, const Type &right);
Type checkNotEquals(const Type &left, const Type &right);

// objective 3.5
Type checkAdd(const Type &left, const Type &right);
Type checkSub(const Type &left, const Type &right);

// objective 3.6
Type checkDereference(const Type &right);
Type checkPostfixExpression(const Type &left, const Type &right);Type checkPostfixExpression(const Type &left, const Type &right);
Type checkAddress(const Type &right, const bool &lvalue);

// objecyive 3.7
Type checkSizeof(const Type &right);

Type checkFunction(const Type &left, Parameters *params);
Type checkReturn(const Type &left, const Type &enclosing);
Type checkAssignment(const Type &left, const Type &right, bool &lvalue);

# endif /* CHECKER_H */
