#ifndef ASTVISITOR_H
#define ASTVISITOR_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class CompUnitVisitor : public Visitor{
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;
};

class FuncDefVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;;   
};

class FuncTypeVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;   
};

class BlockVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;   
};

class StmtVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;
};

class ExpVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;
};

class UnaryExpVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;
};

class PrimaryExpVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;   
};

class UnaryOpVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override; 
};

class MulExpVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;
};

class AddExpVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;
};

class RelExpVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;
};

class EqExpVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;
};

class LAndExpVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;
};

class LOrExpVisitor : public Visitor {
 public:
    unique_ptr<BaseIR> visit(const BaseAST *baseNode) const override;
};


#endif