#ifndef TYPE_H
#define TYPE_H

#include <vector>
#include <ostream>

typedef std::vector<class Type> Parameters;

class Type {
    int _specifier;             // INT, CHAR, etc.
    unsigned _indirection;      // Number of pointers
    enum {
        ARRAY, ERROR, FUNCTION, SCALAR
    } _declarator;              // Name

    unsigned long _length;      // Only valid for arrays
    Parameters *_parameters;    // Only valid for functions

public:
    Type(int specifier, unsigned indirection = 0);                      // Scalar
    Type(int specifier, unsigned indirection, unsigned long length);    // Array
    Type(int specifier, unsigned indirection, Parameters *parameters);  // Function
    Type(); // Error

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