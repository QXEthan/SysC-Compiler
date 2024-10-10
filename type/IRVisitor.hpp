#ifndef IRVISITOR_H
#define IRVISITOR_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "BaseRiscV.hpp"

class BaseIR;

using namespace std;

class IRVisitor {
 public:
    virtual unique_ptr<BaseRiscV> visit(const BaseIR *baseNode) const = 0;
    virtual ~IRVisitor() {}
};

class ValueIRVisitor : public IRVisitor{
 public:
    unique_ptr<BaseRiscV> visit(const BaseIR *baseNode) const override;
};

class ValuesIRVisitor : public IRVisitor{
 public:
    unique_ptr<BaseRiscV> visit(const BaseIR *baseNode) const override;
};

class BasicBlockIRVisitor : public IRVisitor {
 public:
    unique_ptr<BaseRiscV> visit(const BaseIR *baseNode) const override;
};

class FunctionIRVisitor : public IRVisitor{
 public:
    unique_ptr<BaseRiscV> visit(const BaseIR *baseNode) const override;
};

class IRProgramVisitor : public IRVisitor{
 public:
    unique_ptr<BaseRiscV> visit(const BaseIR *baseNode) const override;
};
#endif