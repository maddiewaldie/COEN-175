#include "type.h"
#include <cassert>
#include "iostream"

Type::Type(int specifier, unsigned indirection) : _specifier(specifier), _indirection(indirection),
                                                  _declarator(SCALAR) {}

Type::Type(int specifier, unsigned indirection, unsigned long length) : _specifier(specifier),
                                                                        _indirection(indirection),
                                                                        _declarator(ARRAY), _length(length) {}

Type::Type(int specifier, unsigned indirection, Parameters *parameters) : _specifier(specifier),
                                                                          _indirection(indirection),
                                                                          _declarator(FUNCTION),
                                                                          _parameters(parameters) {}

bool Type::operator!=(const Type &that) const {
    return !operator==(that);
}

bool Type::operator==(const Type &that) const {
 if (_declarator != that._declarator) {
        return false;
    }
    if (_declarator == ERROR) {
        return true;
    }
    if (_specifier != that._specifier) {
        return false;
    }
    if (_indirection != that._indirection) {
        return false;
    }
    if (_declarator == SCALAR) {
        return true;
    }
    if (_declarator == ARRAY) {
        return _length == that._length;
    }
    assert(_declarator == FUNCTION);
    if (!_parameters || !that._parameters) {
        return true;
    }
    return *_parameters == *that._parameters;
}

std::ostream &operator <<(std::ostream &ostr, const Type &type) {
    ostr << "(" << type.specifier() << "," << type.indirection() << ",";

    if(type.isArray())
        ostr << "ARRAY";
    else if(type.isFunction())
        ostr << "FUNCTION";
    else {
        assert(type.isScalar());
        ostr << "SCALAR";
    }
    ostr << ")";
    return ostr;
}