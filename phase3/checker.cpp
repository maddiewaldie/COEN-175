#include "checker.h"
#include "type.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void openScope() {
    cout << "open scope" << endl;
}

void closeScope() {
    cout << "close scope" << endl;
}

void defineFunction(const std::string &name, const Type &type) { cout << name << ": " << type << endl; }
void declareFunction(const std::string &name, const Type &type) {cout << name << ": " << type << endl; }
void declareVariable(const std::string &name, const Type &type) { cout << name << ": " << type << endl; }
void checkIdentifier(const std::string &name) { }