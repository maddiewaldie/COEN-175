#include "checker.h"
#include "scope.h"
#include "type.h"
#include "lexer.h"
#include "tokens.h"
#include <cstdlib>
#include <iostream>

using namespace std;

static Scope *globalScope = nullptr;
static Scope *currentScope = nullptr;

static string errorMessage1 = "redefinition of '%s'";
static string errorMessage2 = "conflicting types for '%s'";
static string errorMessage3 = "redeclaration of '%s'";
static string errorMessage4 = "'%s' undeclared";
static string errorMessage5 = "'%s' has type void";

void openScope() {
    cout << "open scope" << endl;
    if (currentScope == nullptr)
    {
        currentScope = new Scope();
    } else {
        currentScope = new Scope(currentScope);
    }
    
    if (globalScope == nullptr)
    {
        globalScope = currentScope;
    }
}

void closeScope() {
    cout << "close scope" << endl;
    Scope *tempScope = currentScope;
    currentScope = currentScope -> enclosing();
    delete (tempScope);
}

void defineFunction(const std::string &name, const Type &type) { 
    cout << name << ": " << type << endl;
    Symbol *symbol = globalScope -> find(name);

    if (symbol != nullptr) {
        if (symbol -> type().isFunction() && symbol -> type().parameters() != nullptr) {
            report(errorMessage1, name);
        } else if (type != symbol->type()) {
            report(errorMessage2, name);
        }
        globalScope->remove(name);
    }
    symbol = new Symbol(name, type, true);
    globalScope -> insert(symbol);
}

void declareFunction(const std::string &name, const Type &type) {
    cout << name << ":" << type << endl;
    Symbol *symbol = globalScope -> find(name);

    if (symbol == nullptr) {
        symbol = new Symbol(name, type);
        currentScope -> insert(symbol);
    } else if (symbol -> type() != type) {
        report(errorMessage2, name);
    } else if (currentScope -> enclosing() != nullptr) {
        report(errorMessage3, name);
    }
}

void declareVariable(const std::string &name, const Type &type) { 
    cout << name << ": " << type << endl;
    Symbol *symbol = currentScope -> find(name);

    if (symbol == nullptr) {
        if (type.specifier() == VOID && type.indirection() == 0) {
            report(errorMessage5, name);
        }
        symbol = new Symbol(name, type);
        currentScope -> insert(symbol);
    } else if (currentScope->enclosing() != nullptr) {
        report(errorMessage3, name);
    } else if (symbol -> type() != type) {
        report(errorMessage2, name);
    }
}

void checkIdentifier(const std::string &name) { 
    Symbol *symbol = currentScope -> find(name);

    if (symbol == nullptr) {
        symbol = currentScope -> lookup(name);
        if (symbol == nullptr) {
            report(errorMessage4, name);
        }
    }
}