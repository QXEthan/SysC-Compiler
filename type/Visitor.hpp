#ifndef VISITOR_H
#define VISITOR_H

#include "BaseIR.hpp"

class BaseAST;

using namespace std;

class Visitor {
 public:
    virtual unique_ptr<BaseIR> visit(const BaseAST *baseNode) const = 0;
    virtual ~Visitor() {}
};

#endif