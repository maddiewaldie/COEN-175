# include <iostream>
# include "tokens.h"
# include "lexer.h"
//#define DEBUG

using namespace std;
void expression();
void statement();
void statements();
int lookahead;

void match(int t) {
	if (lookahead == t) {
		lookahead = yylex();
	} else { 
		//cout << "error" << endl;
		report("error", "");
	}
}

void expression_list() {
	#ifdef DEBUG 
	cout << "expression_list" << endl; 
	#endif
	expression();
	if (lookahead == COMMA) {
		match(COMMA);
		expression_list();
	}
}

void expression_id() {
	#ifdef DEBUG
	cout << "expression_id" << endl;
	#endif
	if (lookahead == LEFTPAREN) {
		match(LEFTPAREN);
		expression();
		match(RIGHTPAREN);
	} else if (lookahead == ID) {
		match(ID);
		if (lookahead == LEFTPAREN) {
			match(LEFTPAREN);
			if (lookahead != RIGHTPAREN) {
				expression_list();
				match(RIGHTPAREN);
			}
			else if (lookahead == RIGHTPAREN) {
				match(RIGHTPAREN);
			}
		}
	} else if (lookahead == NUM) {
		match(NUM);
	}
	else if (lookahead == STRING) {
		match(STRING);
	} else if (lookahead == CHARACTER) {
		match(CHARACTER);
	} 
}

void expression_brackets() {
	#ifdef DEBUG
	cout << "expression_brackets" << endl;
	#endif
	expression_id();
	while (1) {
		if (lookahead == LEFTBRACKET) {
			match(LEFTBRACKET);
			expression();
			match(RIGHTBRACKET);
			cout << "index" << endl;
		}
		else { break; }
	}
}

void expression_unary() {
	#ifdef DEBUG
	cout << "expression_unary" << endl;
	#endif
	if (lookahead == BITWISEAND) {
		match(BITWISEAND);
		expression_unary();
		cout << "addr" << endl;
	} else if (lookahead == MUL) {
		match(MUL);
		expression_unary();
		cout << "deref" << endl;
	} else if (lookahead == NOT) {
		match(NOT);
		expression_unary();
		cout << "not" << endl;
	} else if (lookahead == SUB) {
		match(SUB);
		expression_unary();
		cout << "neg" << endl;
	} else if (lookahead == SIZEOF) {
		match(SIZEOF);
		expression_unary();
		cout << "sizeof" << endl;
	} else { expression_brackets(); }
}

void expression_multdiv() {
	#ifdef DEBUG
	cout << "expression_multdiv" << endl;
	#endif
	expression_unary();
	while (1) {
		if (lookahead == MUL) {
			match(MUL);
			expression_unary();
			cout << "mul" << endl;
		} else if (lookahead == DIV) {
			match(DIV);
			expression_unary();
			cout << "div" << endl;
		} else if (lookahead == MOD) {
			match(MOD);
			expression_unary();
			cout << "rem" << endl;
		}
		else { break; }
	}
}

void expression_addsub() {
	#ifdef DEBUG
	cout << "expression_addsub" << endl;
	#endif
	expression_multdiv();
	while (1) {
		if (lookahead == ADD) {
			match(ADD);
			expression_multdiv();
			cout << "add" << endl;
		} else if (lookahead == SUB) {
			match(SUB);
			expression_multdiv();
			cout << "sub" << endl;
		}
		else { break; }
	}
}

void expression_comparisons() {
	#ifdef DEBUG
	cout << "expression_comparisons" << endl;
	#endif
	expression_addsub();
	while (1) {
		if (lookahead == LESSTHAN) {
			match(LESSTHAN);
			expression_addsub();
			cout << "ltn" << endl;
		} else if (lookahead == GREATERTHAN) {
			match(GREATERTHAN);
			expression_addsub();
			cout << "gtn" << endl;
		} else if (lookahead == LESSTHANEQUAL) {
			match(LESSTHANEQUAL);
			expression_addsub();
			cout << "leq" << endl;
		} else if (lookahead == GREATERTHANEQUAL) {
			match(GREATERTHANEQUAL);
			expression_addsub();
			cout << "geq" << endl;
		} else { break; }
	}
}

void expression_equality() {
	#ifdef DEBUG
	cout << "expression_equality" << endl;
	#endif
	expression_comparisons();
	while (1) {
		if(lookahead == EQUAL) {
			match(EQUAL);
			expression_comparisons();
			cout << "eql" << endl;
		} else if (lookahead == NOTEQUAL) {
			match(NOTEQUAL);
			expression_comparisons();
			cout << "neq" << endl;
		} else { break; }
	}
}

void expression_and() {
	#ifdef DEBUG
	cout << "expression_and" << endl;
	#endif
	expression_equality();
	while (lookahead == AND) {
		match(AND);
		expression_equality();
		cout << "and" << endl;
	}
}

void expression() {
	#ifdef DEBUG
	cout << "expression" << endl;
	#endif
	expression_and();
	while (lookahead == OR) {
		match(OR);
		expression_and();
		cout << "or" << endl;
	}
}

void pointers() { 
	#ifdef DEBUG
	cout << "pointers" << endl;
	#endif
	while (lookahead == MUL) {
        match(MUL);
    }
}

void declarator() {
	#ifdef DEBUG
	cout << "declarator" << endl;
	#endif
	pointers();
	match(ID);
	if (lookahead == LEFTBRACKET) {
		match(LEFTBRACKET);
		match(NUM);
		match(RIGHTBRACKET);
	}
}

void declarator_list() {
	#ifdef DEBUG
	cout << "declarator_list" << endl;
	#endif
	declarator();
	if (lookahead == COMMA) {
		match(COMMA);
		declarator_list();
	}
}

void specifier() {
	#ifdef DEBUG
	cout << "specifier" << endl;
	#endif
	if (lookahead == INT) {
		match(INT);
	}
	else if (lookahead == CHAR) {
		match(CHAR);
	}
	else if (lookahead == LONG) {
		match(LONG);
	}
	else if (lookahead == VOID) {
		match(VOID);
	}
}

void declaration() {
	#ifdef DEBUG
	cout << "declaration" << endl;
	#endif
	specifier();
	declarator_list();
	match(SEMICOLON);
}

void declarations() { 
	#ifdef DEBUG
	cout << "declarations" << endl;
	#endif
	if (lookahead == INT || lookahead == CHAR || lookahead == LONG || lookahead == VOID) {
		declaration();
		declarations();
	}
}

void assignment() {
	#ifdef DEBUG
	cout << "assignment" << endl;
	#endif
	expression();
	if (lookahead == ASSIGN) {
		match(ASSIGN);
		expression();
	}
}

void statement() {
	#ifdef DEBUG
	cout << "statement" << endl;
	#endif
	if (lookahead == LEFTBRACE) {
		match(LEFTBRACE);
		declarations();
		statements();
		match(RIGHTBRACE);
	} else if (lookahead == RETURN) {
		match(RETURN);
		expression();
		match(SEMICOLON);
	} else if (lookahead == WHILE) {
		match(WHILE);
		match(LEFTPAREN);
		expression();
		match(RIGHTPAREN);
		statement();
	} else if (lookahead == FOR) {
		match(FOR);
		match(LEFTPAREN);
		assignment();
		match(SEMICOLON);
		expression();
		match(SEMICOLON);
		assignment();
		match(RIGHTPAREN);
		statement();
	} else if (lookahead == IF) {
		match(IF);
		match(LEFTPAREN);
		expression();
		match(RIGHTPAREN);
		statement();
		if (lookahead == ELSE) {
			match(ELSE);
			statement();
		}
	}
	else {
		assignment();
		match(SEMICOLON);
	}
}

void statements() { 
	#ifdef DEBUG
	cout << "statements" << endl;
	#endif
	if (lookahead != RIGHTBRACE) {
		statement();
		statements();
	}
}

void global_declarator() {
	pointers();
	match(ID);
	if (lookahead == LEFTPAREN) {
		match(LEFTPAREN);
		match(RIGHTPAREN);
	}
	else if (lookahead == LEFTBRACKET) {
		match(LEFTBRACKET);
		match(NUM);
		match(RIGHTBRACKET);
	}
}

void remaining_decls() {
	#ifdef DEBUG
	cout << "remaining_decls" << endl;
	#endif
	if (lookahead == SEMICOLON) {
		match(SEMICOLON);
	}
	else if (lookahead == COMMA) {
		match(COMMA);
		global_declarator();
		remaining_decls();
	}
}

void parameters() {
	#ifdef DEBUG
	cout << "parameters" << endl;
	#endif
	if (lookahead == VOID) {
		match(VOID);
		if (lookahead == RIGHTPAREN) {
			return;
		}
	}
	else {
		specifier();
		pointers();
		match(ID);
		while (lookahead == COMMA) {
			match(COMMA);
			parameters();
		}
	}
}

void fog() {
    specifier();
    pointers();
    match(ID);

    if (lookahead == LEFTPAREN) {
        match(LEFTPAREN);
        if (lookahead != RIGHTPAREN) {
            parameters();
            match(RIGHTPAREN);
            match(LEFTBRACE);
            declarations();
            statements();
            match(RIGHTBRACE);
        }
        else {
            match(RIGHTPAREN);
            remaining_decls();
        }
    } 
    else if (lookahead == LEFTBRACKET) {
        match(LEFTBRACKET);
        match(NUM);
        match(RIGHTBRACKET);
        remaining_decls();
    }
    else {
        remaining_decls();
    }
}

int main()
{
	//while((lookahead=yylex() != 0)) {
	// 	fog();
	//}
	lookahead=yylex();
	// expression();
	// statement();
	//declaration();
	//statements();
	fog();
}