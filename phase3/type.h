#ifndef TYPE_H
#define TYPE_H

#include <vector>
#include <ostream>

typedef std::vector<class Type> Parameters;

class Type {
    int _specifier;
    unsigned _indirection;
    enum {
        ARRAY, ERROR, FUNCTION, SCALAR
    } _declarator;

    unsigned long _length;
    Parameters *_parameters;

public:
    Type(int specifier, unsigned indirection = 0);
    Type(int specifier, unsigned indirection, unsigned long length);
    Type(int specifier, unsigned indirection, Parameters *parameters);
    Type();

    bool isArray() const { return _declarator == ARRAY; }

    bool isFunction() const { return _declarator == FUNCTION; }

    bool isScalar() const { return _declarator == SCALAR; }

    int specifier() const { return _specifier; }

    unsigned indirection() const { return _indirection; }

    int length() const { return _length; }

    Parameters *parameters() const { return _parameters; }

    bool operator==(const Type &that) const;

    bool operator!=(const Type &that) const;
};

std::ostream &operator<<(std::ostream &ostr, const Type &t);

#endif