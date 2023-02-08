/*
 * File:	parser.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "tokens.h"
# include "lexer.h"
# include "checker.h"
# include "type.h"

using namespace std;

static int lookahead;
static string lexbuf;

static void expression();
static void statement();

/*
 * Function:	error
 *
 * Description:	Report a syntax error to standard error.
 */

static void error()
{
    if (lookahead == DONE)
	report("syntax error at end of file");
    else
	report("syntax error at '%s'", lexbuf);

    exit(EXIT_FAILURE);
}

static void error(int lineNum)
{
    if (lookahead == DONE)
	report("syntax error at end of file");
    else
	report("syntax error at '%s'", lexbuf);

	cout << "line num: " << lineNum << endl;

    exit(EXIT_FAILURE);
}


/*
 * Function:	match
 *
 * Description:	Match the next token against the specified token.  A
 *		failure indicates a syntax error and will terminate the
 *		program since our parser does not do error recovery.
 */

static void match(int t)
{
    if (lookahead != t)
	error();

    lookahead = yylex();
    lexbuf = yytext;
}

static void match(int t, int lineNum)
{
    if (lookahead != t) {
		error(lineNum);
	}

    lookahead = yylex();
    lexbuf = yytext;
}

static string identifier() {
	string identifierValue = lexbuf;
	match(ID, __LINE__);
	return identifierValue;
}

static unsigned number() {
	string numLength = lexbuf;
	match(NUM, __LINE__);
	return stoi(numLength);
}


/*
 * Function:	isSpecifier
 *
 * Description:	Return whether the given token is a type specifier.
 */

static bool isSpecifier(int token)
{
    return token == INT || token == CHAR || token == LONG || token == VOID;
}


/*
 * Function:	specifier
 *
 * Description:	Parse a type specifier.  Simple C has only ints, chars,
 *		longs,and void types.
 *
 *		specifier:
 *		  int
 *		  char
 *		  long
 *		  void
 */

int specifier()
{
	int typespec = lookahead;

    if (isSpecifier(lookahead)) {
	match(lookahead, __LINE__);
	}
    else {
	error(__LINE__);
	}

	return typespec;
}


/*
 * Function:	pointers
 *
 * Description:	Parse pointer declarators (i.e., zero or more asterisks).
 *
 *		pointers:
 *		  empty
 *		  * pointers
 */

unsigned pointers()
{
	unsigned count = 0;
    while (lookahead == '*') {
		match('*', __LINE__);
		count++;
	}
	return count;
}


/*
 * Function:	declarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable or an array, with optional pointer declarators.
 *
 *		declarator:
 *		  pointers identifier
 *		  pointers identifier [ num ]
 */

static void declarator(int typespec)
{
    unsigned indirection = pointers();
    string name = identifier();

    if (lookahead == '[') {
	match('[', __LINE__);
	unsigned length = number();
	Type type = Type(typespec, indirection, length);
	declareVariable(name, type);
	match(']', __LINE__);
	//cout << "(" << typespec << ", " << indirection << ", ARRAY)" << endl;
    } else {
	Type type = Type(typespec, indirection);
	declareVariable(name, type);
	//cout << "(" << typespec << ", " << indirection << ", SCALAR)" << endl;
	}
}


/*
 * Function:	declaration
 *
 * Description:	Parse a local variable declaration.  Global declarations
 *		are handled separately since we need to detect a function
 *		as a special case.
 *
 *		declaration:
 *		  specifier declarator-list ';'
 *
 *		declarator-list:
 *		  declarator
 *		  declarator , declarator-list
 */

static void declaration()
{
	int typespec = specifier();
    declarator(typespec);

    while (lookahead == ',') {
	match(',', __LINE__);
	declarator(typespec);
    }

    match(';', __LINE__);
}


/*
 * Function:	declarations
 *
 * Description:	Parse a possibly empty sequence of declarations.
 *
 *		declarations:
 *		  empty
 *		  declaration declarations
 */

static void declarations()
{
    while (isSpecifier(lookahead)) {
	declaration();
	}
}


/*
 * Function:	primaryExpression
 *
 * Description:	Parse a primary expression.
 *
 *		primary-expression:
 *		  ( expression )
 *		  identifier ( expression-list )
 *		  identifier ( )
 *		  identifier
 *		  character
 *		  string
 *		  num
 *
 *		expression-list:
 *		  expression
 *		  expression , expression-list
 */

static void primaryExpression()
{
    if (lookahead == '(') {
	match('(', __LINE__);
	expression();
	match(')', __LINE__);

    } else if (lookahead == CHARACTER) {
	match(CHARACTER, __LINE__);

    } else if (lookahead == STRING) {
	match(STRING, __LINE__);

    } else if (lookahead == NUM) {
	number();

    } else if (lookahead == ID) {
	string id = identifier();
	checkIdentifier(id);

	if (lookahead == '(') {
	    match('(', __LINE__);

	    if (lookahead != ')') {
		expression();

		while (lookahead == ',') {
		    match(',', __LINE__);
		    expression();
		}
	    }

	    match(')', __LINE__);
	}

    } else
	error(__LINE__);
}


/*
 * Function:	postfixExpression
 *
 * Description:	Parse a postfix expression.
 *
 *		postfix-expression:
 *		  primary-expression
 *		  postfix-expression [ expression ]
 */

static void postfixExpression()
{
    primaryExpression();

    while (lookahead == '[') {
	match('[', __LINE__);
	expression();
	match(']', __LINE__);
	cout << "index" << endl;
    }
}


/*
 * Function:	prefixExpression
 *
 * Description:	Parse a prefix expression.
 *
 *		prefix-expression:
 *		  postfix-expression
 *		  ! prefix-expression
 *		  - prefix-expression
 *		  * prefix-expression
 *		  & prefix-expression
 *		  sizeof prefix-expression
 */

static void prefixExpression()
{
    if (lookahead == '!') {
	match('!', __LINE__);
	prefixExpression();
	cout << "not" << endl;

    } else if (lookahead == '-') {
	match('-', __LINE__);
	prefixExpression();
	cout << "neg" << endl;

    } else if (lookahead == '*') {
	match('*', __LINE__);
	prefixExpression();
	cout << "deref" << endl;

    } else if (lookahead == '&') {
	match('&', __LINE__);
	prefixExpression();
	cout << "addr" << endl;

    } else if (lookahead == SIZEOF) {
	match(SIZEOF, __LINE__);
	prefixExpression();
	cout << "sizeof" << endl;

    } else
	postfixExpression();
}


/*
 * Function:	multiplicativeExpression
 *
 * Description:	Parse a multiplicative expression.  Simple C does not have
 *		cast expressions, so we go immediately to prefix
 *		expressions.
 *
 *		multiplicative-expression:
 *		  prefix-expression
 *		  multiplicative-expression * prefix-expression
 *		  multiplicative-expression / prefix-expression
 *		  multiplicative-expression % prefix-expression
 */

static void multiplicativeExpression()
{
    prefixExpression();

    while (1) {
	if (lookahead == '*') {
	    match('*', __LINE__);
	    prefixExpression();
	    cout << "mul" << endl;

	} else if (lookahead == '/') {
	    match('/', __LINE__);
	    prefixExpression();
	    cout << "div" << endl;

	} else if (lookahead == '%') {
	    match('%', __LINE__);
	    prefixExpression();
	    cout << "rem" << endl;

	} else
	    break;
    }
}


/*
 * Function:	additiveExpression
 *
 * Description:	Parse an additive expression.
 *
 *		additive-expression:
 *		  multiplicative-expression
 *		  additive-expression + multiplicative-expression
 *		  additive-expression - multiplicative-expression
 */

static void additiveExpression()
{
    multiplicativeExpression();

    while (1) {
	if (lookahead == '+') {
	    match('+', __LINE__);
	    multiplicativeExpression();
	    cout << "add" << endl;

	} else if (lookahead == '-') {
	    match('-', __LINE__);
	    multiplicativeExpression();
	    cout << "sub" << endl;

	} else
	    break;
    }
}


/*
 * Function:	relationalExpression
 *
 * Description:	Parse a relational expression.  Note that Simple C does not
 *		have shift operators, so we go immediately to additive
 *		expressions.
 *
 *		relational-expression:
 *		  additive-expression
 *		  relational-expression < additive-expression
 *		  relational-expression > additive-expression
 *		  relational-expression <= additive-expression
 *		  relational-expression >= additive-expression
 */

static void relationalExpression()
{
    additiveExpression();

    while (1) {
	if (lookahead == '<') {
	    match('<', __LINE__);
	    additiveExpression();
	    cout << "ltn" << endl;

	} else if (lookahead == '>') {
	    match('>', __LINE__);
	    additiveExpression();
	    cout << "gtn" << endl;

	} else if (lookahead == LEQ) {
	    match(LEQ, __LINE__);
	    additiveExpression();
	    cout << "leq" << endl;

	} else if (lookahead == GEQ) {
	    match(GEQ, __LINE__);
	    additiveExpression();
	    cout << "geq" << endl;

	} else
	    break;
    }
}


/*
 * Function:	equalityExpression
 *
 * Description:	Parse an equality expression.
 *
 *		equality-expression:
 *		  relational-expression
 *		  equality-expression == relational-expression
 *		  equality-expression != relational-expression
 */

static void equalityExpression()
{
    relationalExpression();

    while (1) {
	if (lookahead == EQL) {
	    match(EQL, __LINE__);
	    relationalExpression();
	    cout << "eql" << endl;

	} else if (lookahead == NEQ) {
	    match(NEQ, __LINE__);
	    relationalExpression();
	    cout << "neq" << endl;

	} else
	    break;
    }
}


/*
 * Function:	logicalAndExpression
 *
 * Description:	Parse a logical-and expression.  Note that Simple C does
 *		not have bitwise-and expressions.
 *
 *		logical-and-expression:
 *		  equality-expression
 *		  logical-and-expression && equality-expression
 */

static void logicalAndExpression()
{
    equalityExpression();

    while (lookahead == AND) {
	match(AND, __LINE__);
	equalityExpression();
	cout << "and" << endl;
    }
}


/*
 * Function:	expression
 *
 * Description:	Parse an expression, or more specifically, a logical-or
 *		expression, since Simple C does not allow comma or
 *		assignment as an expression operator.
 *
 *		expression:
 *		  logical-and-expression
 *		  expression || logical-and-expression
 */

static void expression()
{
    logicalAndExpression();

    while (lookahead == OR) {
	match(OR, __LINE__);
	logicalAndExpression();
	cout << "or" << endl;
    }
}


/*
 * Function:	statements
 *
 * Description:	Parse a possibly empty sequence of statements.  Rather than
 *		checking if the next token starts a statement, we check if
 *		the next token ends the sequence, since a sequence of
 *		statements is always terminated by a closing brace.
 *
 *		statements:
 *		  empty
 *		  statement statements
 */

static void statements()
{
    while (lookahead != '}') {
	statement();
	}
}


/*
 * Function:	Assignment
 *
 * Description:	Parse an assignment statement.
 *
 *		assignment:
 *		  expression = expression
 *		  expression
 */

static void assignment()
{
    expression();

    if (lookahead == '=') {
	match('=', __LINE__);
	expression();
    }
}


/*
 * Function:	statement
 *
 * Description:	Parse a statement.  Note that Simple C has so few
 *		statements that we handle them all in this one function.
 *
 *		statement:
 *		  { declarations statements }
 *		  return expression ;
 *		  while ( expression ) statement
 *		  for ( assignment ; expression ; assignment ) statement
 *		  if ( expression ) statement
 *		  if ( expression ) statement else statement
 *		  assignment ;
 */

static void statement()
{
    if (lookahead == '{') {
	openScope();
	match('{', __LINE__);
	declarations();
	statements();
	match('}', __LINE__);
	closeScope();

    } else if (lookahead == RETURN) {
	match(RETURN, __LINE__);
	expression();
	match(';', __LINE__);

    } else if (lookahead == WHILE) {
	match(WHILE, __LINE__);
	match('(', __LINE__);
	expression();
	match(')', __LINE__);
	statement();

    } else if (lookahead == FOR) {
	match(FOR, __LINE__);
	match('(', __LINE__);
	assignment();
	match(';', __LINE__);
	expression();
	match(';', __LINE__);
	assignment();
	match(')', __LINE__);
	statement();

    } else if (lookahead == IF) {
	match(IF, __LINE__);
	match('(', __LINE__);
	expression();
	match(')', __LINE__);
	statement();

	if (lookahead == ELSE) {
	    match(ELSE, __LINE__);
	    statement();
	}
    } else {
	assignment();
	match(';', __LINE__);
    }
}


/*
 * Function:	parameter
 *
 * Description:	Parse a parameter, which in Simple C is always a scalar
 *		variable with optional pointer declarators.
 *
 *		parameter:
 *		  specifier pointers identifier
 */

static Type parameter()
{
    int typespec = specifier();
    unsigned indirection = pointers();
    string name = identifier();
	Type type = Type(typespec, indirection);
	declareVariable(name, type);
	return type;
}


/*
 * Function:	parameters
 *
 * Description:	Parse the parameters of a function, but not the opening or
 *		closing parentheses.
 *
 *		parameters:
 *		  void
 *		  void pointers identifier remaining-parameters
 *		  char pointers identifier remaining-parameters
 *		  int pointers identifier remaining-parameters
 *
 *		remaining-parameters:
 *		  empty
 *		  , parameter remaining-parameters
 */

static Parameters *parameters()
{
	Parameters *parameters = new Parameters;
	int typespec;

    if (lookahead == VOID) {
	match(VOID, __LINE__);
	typespec = VOID;

	if (lookahead == ')') {
	    return parameters;
	}

    } else
	typespec = specifier();

    unsigned indirection = pointers();
    string name = identifier();
	Type type = Type(typespec, indirection);
	declareVariable(name, type);
	parameters->push_back(type);

    while (lookahead == ',') {
	match(',', __LINE__);
	parameters->push_back(parameter());
    }
	return parameters;
}


/*
 * Function:	globalDeclarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable, an array, or a function, with optional pointer
 *		declarators.
 *
 *		global-declarator:
 *		  pointers identifier
 *		  pointers identifier ( )
 *		  pointers identifier [ num ]
 */

static void globalDeclarator(int typespec)
{
    unsigned indirection = pointers();
    string name = identifier();

    if (lookahead == '(') {
	match('(', __LINE__);
    Type type = Type(typespec, indirection, nullptr);
	declareFunction(name, type);
	match(')', __LINE__);

    } else if (lookahead == '[') {
	match('[', __LINE__);
	unsigned length = number();
	Type type = Type(typespec, indirection, length);
	declareVariable(name, type);
	match(']', __LINE__);
    }
	else {
		Type type = Type(typespec, indirection);
		declareVariable(name, type);
	}
}


/*
 * Function:	remainingDeclarators
 *
 * Description:	Parse any remaining global declarators after the first.
 *
 * 		remaining-declarators:
 * 		  ;
 * 		  , global-declarator remaining-declarators
 */

static void remainingDeclarators(int typespec)
{
    while (lookahead == ',') {
	match(',', __LINE__);
	globalDeclarator(typespec);
    }

    match(';', __LINE__);
}


/*
 * Function:	globalOrFunction
 *
 * Description:	Parse a global declaration or function definition.
 *
 * 		global-or-function:
 * 		  specifier pointers identifier remaining-decls
 * 		  specifier pointers identifier ( ) remaining-decls 
 * 		  specifier pointers identifier [ num ] remaining-decls
 * 		  specifier pointers identifier ( parameters ) { ... }
 */

static void globalOrFunction()
{
    int typespec = specifier();
    unsigned indirection = pointers();
    string name = identifier();

    if (lookahead == '[') {
	match('[', __LINE__);
	unsigned length = number();
	Type type = Type(typespec, indirection, length);
	declareVariable(name, type);
	match(']', __LINE__);
	remainingDeclarators(typespec);

    } else if (lookahead == '(') {
	match('(', __LINE__);

	if (lookahead == ')') {
		Type type = Type(typespec, indirection, nullptr);
		declareFunction(name, type);
		match(')', __LINE__);
	    remainingDeclarators(typespec);

	} else {
		openScope();
		Type type = Type(typespec, indirection, parameters());
		defineFunction(name, type);
	    match(')', __LINE__);
	    match('{', __LINE__);
	    declarations();
	    statements();
		closeScope();
	    match('}', __LINE__);
	}

    } else {
		Type type = Type(typespec, indirection);
		declareVariable(name, type);
		remainingDeclarators(typespec);
	}
}


/*
 * Function:	main
 *
 * Description:	Analyze the standard input stream.
 */

int main()
{
	openScope();
    lookahead = yylex();
    lexbuf = yytext;

    while (lookahead != DONE)
	globalOrFunction();

	closeScope();

    exit(EXIT_SUCCESS);
}
