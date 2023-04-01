/*
 * File:	checker.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the semantic checker for Simple C.
 *
 *		If a symbol is redeclared, the redeclaration is discarded
 *		and the original declaration is retained.
 *
 *		Extra functionality:
 *		- inserting an undeclared symbol with the error type
 */

# include <iostream>
# include "lexer.h"
# include "checker.h"
# include "tokens.h"
# include "Symbol.h"
# include "Scope.h"
# include "Type.h"

using namespace std;

static Scope *outermost, *toplevel;
static const Type error, integer(INT);

static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string undeclared = "'%s' undeclared";
static string void_object = "'%s' has type void";

static string error1 = "invalid return type";
static string error2 = "invalid type for test expression";
static string error3 = "lvalue required in expression";
static string error4 = "invalid operands to binary '%s'";
static string error5 = "invalid operands to unary '%s'";
static string error6 = "called object is not a function";
static string error7 = "invalid arguments to called function";


/*
 * Function:	openScope
 *
 * Description:	Create a scope and make it the new top-level scope.
 */

Scope *openScope()
{
    toplevel = new Scope(toplevel);

    if (outermost == nullptr)
	outermost = toplevel;

    return toplevel;
}


/*
 * Function:	closeScope
 *
 * Description:	Remove the top-level scope, and make its enclosing scope
 *		the new top-level scope.
 */

Scope *closeScope()
{
    Scope *old = toplevel;
    toplevel = toplevel->enclosing();
    return old;
}


/*
 * Function:	defineFunction
 *
 * Description:	Define a function with the specified NAME and TYPE.  A
 *		function is always defined in the outermost scope.  This
 *		definition always replaces any previous definition or
 *		declaration.
 */

Symbol *defineFunction(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = outermost->find(name);

    if (symbol != nullptr) {
	if (symbol->type().isFunction() && symbol->type().parameters()) {
	    report(redefined, name);
	    delete symbol->type().parameters();

	} else if (type != symbol->type())
	    report(conflicting, name);

	outermost->remove(name);
	delete symbol;
    }

    symbol = new Symbol(name, type);
    outermost->insert(symbol);
    return symbol;
}


/*
 * Function:	declareFunction
 *
 * Description:	Declare a function with the specified NAME and TYPE.  A
 *		function is always declared in the outermost scope.  Any
 *		redeclaration is discarded.
 */

Symbol *declareFunction(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = outermost->find(name);

    if (symbol == nullptr) {
	symbol = new Symbol(name, type);
	outermost->insert(symbol);

    } else if (type != symbol->type()) {
	report(conflicting, name);
	delete type.parameters();

    } else
	delete type.parameters();

    return symbol;
}


/*
 * Function:	declareVariable
 *
 * Description:	Declare a variable with the specified NAME and TYPE.  Any
 *		redeclaration is discarded.
 */

Symbol *declareVariable(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = toplevel->find(name);

    if (symbol == nullptr) {
	if (type.specifier() == VOID && type.indirection() == 0)
	    report(void_object, name);

	symbol = new Symbol(name, type);
	toplevel->insert(symbol);

    } else if (outermost != toplevel)
	report(redeclared, name);

    else if (type != symbol->type())
	report(conflicting, name);

    return symbol;
}


/*
 * Function:	checkIdentifier
 *
 * Description:	Check if NAME is declared.  If it is undeclared, then
 *		declare it as having the error type in order to eliminate
 *		future error messages.
 */

Symbol *checkIdentifier(const string &name)
{
    Symbol *symbol = toplevel->lookup(name);

    if (symbol == nullptr) {
	report(undeclared, name);
	symbol = new Symbol(name, error);
	toplevel->insert(symbol);
    }

    return symbol;
}

// objective 3.1
Type checkIf(const Type &left) {
    if (left == error) {
         return error;
     }
     else if (left.isPredicate()) {
         return left;
     }
     report(error2);
     return error;
}

Type checkWhile(const Type &left) {
    if (left == error) {
        return error;
    }
    else if (left.isPredicate()) {
        return left;
    }
    report(error2);
    return error;
}

Type checkFor(const Type &left) {
    if (left == error) {
        return error;
    }
    else if (left.isPredicate()) {
        return left;
    }
    report(error2);
    return error;
}

Type checkLogical(const Type &left, const Type &right, const string &operand) {
    if (left == error || right == error) {
        return error;
    }
    if(left.isPredicate() && right.isPredicate()) {
        return integer;
    }
    report(error4, operand);
    return error;
}

Type checkLogicalOr(const Type &left, const Type &right) {
    return checkLogical(left, right, "||");
}

Type checkLogicalAnd(const Type &left, const Type &right) {
     return checkLogical(left, right, "&&");
}

Type checkNot(const Type &right) {
    if (right == error) {
        return error;
    }
    Type promoteRight = right.promote();
    if (promoteRight.isPredicate()) {
        return integer;
    }
    report(error5, "!");
    return error;
}

// objective 3.2
Type checkMultiplicativeExpressions(const Type &left, const Type &right, const string &operand) {
    if (left == error || right == error) {
        return error;
    }
    Type promoteLeft = left.promote();
    Type promoteRight = right.promote();
    if (promoteLeft.isNumeric() && promoteRight.isNumeric()) {
        return LONG;
    }
    report(error4, operand);
    return error;
}

Type checkMultiply(const Type &left, const Type &right) {
    return checkMultiplicativeExpressions(left, right, "*");
}

Type checkDivide(const Type &left, const Type &right) {
    return checkMultiplicativeExpressions(left, right, "/");
}

Type checkMod(const Type &left, const Type &right) {
    return checkMultiplicativeExpressions(left, right, "%");
}

Type checkNeg(const Type &right) {
    if (right == error) {
        return error;
    }
    Type promoteRight = right.promote();
    if (promoteRight.isNumeric()) {
        return LONG;
    }
    report(error5, "-");
    return error;
}

// objective 3.3
Type checkRelationalExpressions(const Type &left, const Type &right, const string &operand) { 
    if (left == error || right == error) {
        return error;
    }
    Type promoteLeft = left.promote();
    Type promoteRight = right.promote();
    if ((promoteLeft == promoteRight) && promoteLeft.isNumeric()) {
        return LONG;
    }
    report(error4, operand);
    return error;
}

Type checkLessThan(const Type &left, const Type &right) { 
    return checkRelationalExpressions(left, right, "<");
}

Type checkGreaterThan(const Type &left, const Type &right) { 
    return checkRelationalExpressions(left, right, ">");
}

Type checkLessThanEqualTo(const Type &left, const Type &right) { 
    return checkRelationalExpressions(left, right, "<=");
}

Type checkGreaterThanEqualTo(const Type &left, const Type &right) { 
    return checkRelationalExpressions(left, right, ">=");
}

// objective 3.4
Type checkEquals(const Type &left, const Type &right) {
    if (left == error || right == error) {
        return error;
    }
    if (left.isCompatibleWith(right)) {
        return integer;
    }
    report(error4, "==");
    return error;
}

Type checkNotEquals(const Type &left, const Type &right) {
    if (left == error || right == error) {
        return error;
    }
    if (left.isCompatibleWith(right)) {
        return LONG;
    }
    report(error4, "!=");
    return error;
}

// objective 3.5
Type checkAdd(const Type &left, const Type &right) {
    if (left == error || right == error) {
        return error;
    }
    Type promoteLeft = left.promote();
    Type promoteRight = right.promote();
    if (promoteLeft.isNumeric() && promoteRight.isNumeric()) {
        return LONG;
    }
    else if (promoteLeft.isPointer() && promoteLeft != Type(VOID, 1) && promoteRight.isNumeric()) {
        return Type(promoteLeft.specifier(), promoteLeft.indirection());
    }
    else if (promoteLeft.isNumeric() && promoteRight.isPointer() && promoteRight != Type(VOID, 1)) {
        return Type(promoteRight.specifier(), promoteRight.indirection());
    }
    report(error4, "+");
    return error;
}

Type checkSub(const Type &left, const Type &right) {
    if (left == error || right == error) {
        return error;
    }
    Type promoteLeft = left.promote();
    Type promoteRight = right.promote();
    if (promoteLeft.isNumeric() && promoteRight.isNumeric()) {
        return LONG;
    }
    else if (promoteLeft.isPointer() && promoteLeft.specifier() != VOID && promoteRight.isNumeric()) {
        return Type(promoteLeft.specifier(), promoteLeft.indirection());
    }
    else if (promoteLeft.isPointer() && promoteRight.isPointer() && (promoteLeft.specifier() != VOID) && (promoteLeft.specifier() == promoteRight.specifier())) {
        return Type(promoteRight.specifier(), promoteRight.indirection());
    }
    report(error4, "-");
    return error;
}

// objective 3.6
Type checkDereference(const Type &right) {
    if (right == error) {
        return error;
    }

    Type promoteRight = right.promote();
    
    if (promoteRight.isPointer() && promoteRight.specifier() != VOID) {
        return Type(promoteRight.specifier(), promoteRight.indirection()-1);
    }

    report(error5, "*");
    return error;
}

Type checkPostfixExpression(const Type &left, const Type &right);Type checkPostfixExpression(const Type &left, const Type &right) {
    if (left == error || right == error) {
        return error;
    }
    Type promoteLeft = left.promote();
    Type promoteRight = right.promote();
    if (promoteLeft.isPointer() && promoteLeft != Type(VOID, 1) && promoteRight.isNumeric()) {
        return Type(promoteLeft.specifier(), promoteLeft.indirection()-1);
    }
    report(error4, "[]");
    return error;
}

Type checkAddress(const Type &right, const bool &lvalue) {
    if (right == error) {
        return error;
    }
    if (lvalue) {
        return Type(right.specifier(), right.indirection()+1);
    }
    report(error3);
    return error;
}

// objecyive 3.7
Type checkSizeof(const Type &right) {
    if (right == error) {
        return error;
    }
    if (right.isPredicate()) {
        return integer;
    }
    report(error5, "sizeof");
    return error;
}

Type checkFunction(const Type &left, Parameters *params) {
    if (left == error) {
        return error;
    }
    if (!left.isFunction()) {
        report(error6);
        return error;
    }
    if (left.parameters() == nullptr) {
        return Type(left.specifier(), left.indirection());
    }
    if (left.parameters()->size() != params->size()) {
        report(error7);
        return error;
    }
    for (unsigned i = 0; i < params->size(); i++) {
        Type param1 = (*left.parameters())[i].promote();
        Type param2 = (*params)[i].promote();
        if (!param1.isCompatibleWith(param2)) {
            report(error7);
            return error;
        }
    }
    return Type(left.specifier(), left.indirection());
}

Type checkReturn(const Type &left, const Type &enclosing) {
    if (left == error || enclosing == error) {
        return error;
    }
    if (left.isCompatibleWith(enclosing)) {
        return left;
    }
    report(error1);
    return error;
}

Type checkAssignment(const Type &left, const Type &right, bool &lvalue) {
    if (left == error || right == error) {
        return error;
    }
    else if (!lvalue) {
        report(error3);
        return error;
    }
    else if (left.isCompatibleWith(right)) {
        return left;
    }
    report(error4, "=");
    return error;
}