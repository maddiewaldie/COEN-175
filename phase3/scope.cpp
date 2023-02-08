#include "scope.h"
#include "symbol.h"

Scope::Scope(Scope *enclosing): _enclosing(enclosing) { }

void Scope::insert(Symbol *symbol) {
    _symbols.push_back(symbol);
}

void Scope::remove(const string &name) {
    unsigned i;
    for (i = 0; i < _symbols.size(); i++) {
        if (name == _symbols[i] -> name()) { break; }
    }
    if (i != _symbols.size()) {
        _symbols.erase(_symbols.begin() + i);
    }
}

Symbol * Scope::find(const string &name) const {
    for (size_t i = 0; i < _symbols.size(); i++) {
        if (_symbols[i] -> name() == name) {
            return _symbols[i];
        }
    }
    return nullptr;
}

Symbol * Scope::lookup(const string &name) const {
    Symbol *symbol;
    if ((symbol = find(name)) != nullptr) { return symbol; }
    return _enclosing != nullptr ? _enclosing -> lookup(name) : nullptr;
}