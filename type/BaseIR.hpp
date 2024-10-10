#ifndef BASEIR_H
#define BASEIR_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>

#include "./IRVisitor.hpp"
class IRVisitor;

class BaseIR {
 public:
    virtual ~BaseIR() = default;
    virtual void Dump(std::ofstream &outFile) const = 0;
    virtual std::unique_ptr<BaseRiscV> accept(IRVisitor *v) const = 0;
};

class ValueIR : public BaseIR {
 public:
    std::unique_ptr<std::string> op; // operator string
    std::unique_ptr<int> left;  //left value
    std::unique_ptr<int> right; // right value
    std::unique_ptr<int> tmp; // current number of register
    bool isRet;
    bool isNumber;
    bool isLeftVar;
    bool isRightVar;
    
    void Dump(std::ofstream &outFile) const override {
        std::cout << "In ValueIR Dump" << std::endl;
        if (isRet) {
            if (isLeftVar) {
                outFile << "  ret %" << *left;
            } else {
                outFile << "  ret " << *left;
            }
        }
        else {
            std::cout << "Dump Value" << std::endl;
            outFile << "  %" << *tmp << " = " << *op << " ";
            if (isLeftVar) {
                outFile << "%";
            }
            outFile << *left << ", ";
            if (isRightVar) {
                outFile << "%";
            }
            outFile << *right;
        }
        outFile << std::endl;
    }

    std::unique_ptr<BaseRiscV> accept(IRVisitor *v) const override {return v->visit(this);}
};

class ValuesIR : public BaseIR {
 public:
    std::unique_ptr<std::vector<std::unique_ptr<ValueIR>>> values;
    void Dump(std::ofstream &outFile) const override {
        std::cout << "In Values Dump" << std::endl;
        for (const std::unique_ptr<ValueIR> &val : *values) {
            std::cout << "In Values Dump in FOR LOOP" << std::endl;
            std::cout << " ";
            val->Dump(outFile);
        }
    }

    std::unique_ptr<BaseRiscV> accept(IRVisitor *v) const override {return v->visit(this);}
};

class BasicBlockIR : public BaseIR {
 public:
    std::unique_ptr<std::string> enterName;
    std::unique_ptr<ValuesIR> vals;

    void Dump(std::ofstream &outFile) const override {
        std::cout << "In BasicBlockIR D mp" << std::endl;
        outFile << "{\n%" << *enterName << ":\n";
        std::cout << "In BasicBlockIR Dump Before FOR LOOP" << std::endl;
        vals->Dump(outFile);
        outFile << "}" << std::endl;
        std::cout << "In BasicBlockIR Dump After FOR LOOP" << std::endl;
    }

    std::unique_ptr<BaseRiscV> accept(IRVisitor *v) const override {return v->visit(this);}
};

class FunctionIR : public BaseIR {
 public:
    std::unique_ptr<std::string> name;
    std::unique_ptr<std::string> retType;
    std::unique_ptr<BasicBlockIR> blocks;

    void Dump(std::ofstream &outFile) const override {
        std::cout << "In FunctionIR Dump" << std::endl;
        outFile << "fun @" << *name << "(): " << *retType;
        blocks->Dump(outFile);
    };
    std::unique_ptr<BaseRiscV> accept(IRVisitor *v) const override {return v->visit(this);}
};

class IRProgram : public BaseIR {
 public:
  std::unique_ptr<FunctionIR> funcs;
  std::unique_ptr<ValueIR> vals;
  
  void Dump(std::ofstream &outFile) const override {
    std::cout << "In IRProgram Dump" << std::endl;
    // vals->Dump(outFile);
    funcs->Dump(outFile);
  }

  std::unique_ptr<BaseRiscV> accept(IRVisitor *v) const override {return v->visit(this);}
};

#endif
