#include "generator.h"

using namespace std;

// objective 4
static ostream &operator<<(ostream &ostr, Expression *expr) {
    expr -> operand(ostr);
    return ostr;
}

// objective 2
void Block::generate() {
    //cout << "#Block:" << endl;
    for (auto stmt : _stmts) {
        stmt -> generate();
    }
}

// objective 3
void Simple::generate() {
    _expr -> generate();
}

// objective 4
void Expression::operand(ostream &ostr) const {
    // output nothing
}

void Number::operand(ostream &ostr) const {
    //cout << "#Number:" << endl;
    ostr << "$" << _value;
}

void Identifier::operand(ostream &ostr) const {
    //cout << "#Identifier:" << endl;
    if (_symbol ->_offset == 0) {
        cout << _symbol -> name();
    }
    else {
        cout << _symbol -> _offset << "(%rbp)";
    }
}

// objective 5
void generateGlobals(Scope *scope) {
    cout << "#Generating Globals:" << endl;
    Symbols globals = scope -> symbols();
    for (unsigned i = 0; i < globals.size(); i++) {
        if(!globals[i]->type().isFunction()) {
            cout << "\t.comm\t" << globals[i]->name() << ", " << globals[i]->type().size() << endl;
        }

    }
}

// objective 7
void Function::generate() {
    cout << "#Function:" << endl;
    int params_offset = 8;
    int local_offset = 0;
    Symbols symbols = _body->declarations()->symbols();
    for (unsigned i = 0; i < symbols.size(); ++i) {
        if (i < _id->type().parameters()->size()) {
            symbols[i]->_offset = params_offset;
            params_offset += symbols[i]->type().size();
        }
        else {
            local_offset -= symbols[i]->type().size();
            symbols[i]->_offset = local_offset;
        }
    }

    cout << "#Prologue:" << endl;
    cout << _id->name() << ":" << endl;
    cout << "\tpushq\t%rbp" << endl;
    cout << "\tmovq\t%rsp, %rbp" << endl;
    cout << "\tsubq\t$" << -local_offset << ", %rsp" << endl;

    _body->generate();

    cout << "#Epilogue:" << endl;
    cout << "\tmovq\t%rbp, %rsp" << endl;
    cout << "\tpopq\t%rbp" << endl;
    cout << "\tret" << endl;
    cout << "\t.globl\t" << _id->name() << endl;
}

// after lab objective 2
void Assignment::generate(){
    cout << "#Assignment:" << endl;
    cout << "\tmovl\t" << _right << ", " << _left << endl;
}

// after lab objective 3
void Call::generate() {
    cout << "#Function Call:" << endl;
    string registers[6] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
    for (int i = _args.size() - 1; i >= 0; i --) {
	    _args[i]->generate();
    }
    for (int i = _args.size() - 1; i >= 0; i --) {
	    cout << "\tmovl\t" << _args[i] << ", " << registers[i] << endl;
    }

  	cout << "\tcall\t" << _id->name() << endl;
}