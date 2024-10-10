#ifndef BASERISCV_H
#define BASERISCV_H

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

enum RegisterType {
    R_NOT_USED,    // 0
    R_NUMBER,  // 1
    R_T_REGISTER,    // 2
    R_A_REGISTER,
    R_X_REGISTER,
};

class IRVisitor;

class BaseRiscV {
 public:
    virtual ~BaseRiscV() = default;
    virtual void Dump(std::ofstream &outFile) const = 0;
};
class AssemblyRiscV : public BaseRiscV {
 public:
    std::unique_ptr<std::string> name;
    std::unique_ptr<int> retR;
    std::unique_ptr<int> retR_t;     // -1: not used, 0: number, 1: t, 2:a, 3:x
    std::unique_ptr<int> paramR1;
    std::unique_ptr<int> paramR1_t;
    std::unique_ptr<int> paramR2;
    std::unique_ptr<int> paramR2_t;

    void Dump(std::ofstream &outFile) const override {
        outFile << "  " << *name;
        for (int i = 0; i < (5 - (*name).length()); i++) {
            outFile << " ";
        }
        if (*retR_t == R_A_REGISTER) {
            outFile << "a" << *retR << ", ";
        }else if (*retR_t == R_T_REGISTER) {
            outFile << "t" << *retR << ", ";
        }else if (*retR_t == R_X_REGISTER) {
            outFile << "x" << *retR << ", ";
        }else if (*retR_t == R_NUMBER) {
            outFile << *retR << ", ";
        }
        if (*paramR1_t == R_A_REGISTER) {
            outFile << "a" << *paramR1 << ", ";
        }else if (*paramR1_t == R_T_REGISTER) {
            outFile << "t" << *paramR1 << ", ";
        }else if (*paramR1_t == R_X_REGISTER) {
            outFile << "x" << *paramR1 << ", ";
        }else if (*paramR1_t == R_NUMBER) {
            outFile << *paramR1 << ", ";
        }

        if (*paramR2_t == R_A_REGISTER) {
            outFile << "a" << *paramR2;
        }else if (*paramR2_t == R_T_REGISTER) {
            outFile << "t" << *paramR2;
        }else if (*paramR2_t == R_X_REGISTER) {
            outFile << "x" << *paramR2;
        }else if (*paramR2_t == R_NUMBER) {
            outFile << *paramR2;
        }
        outFile<< std::endl;
    }
    
};

class AssemblySetRiscV : public BaseRiscV {
 public:
    std::unique_ptr<std::vector<unique_ptr<AssemblyRiscV>>> instructions;

    void Dump(std::ofstream &outFile) const override {
        return;
    }
};

class GroupRiscV : public BaseRiscV {
 public:
    std::unique_ptr<std::vector<unique_ptr<AssemblyRiscV>>> instructions;

    void Dump(std::ofstream &outFile) const override {
        for (const std::unique_ptr<AssemblyRiscV> &assembly : *instructions) {
            std::cout << "In GroupRiscV Dump in FOR LOOP" << std::endl;
            std::cout << " ";
            assembly->Dump(outFile);
        }
    }
};

class FunctionRiscV : public BaseRiscV {
 public:
    std::unique_ptr<std::string> name;
    std::unique_ptr<GroupRiscV> group;
    void Dump(std::ofstream &outFile) const override {
        outFile << *name << ":" << std::endl;
        group->Dump(outFile);
    }
};

class ProgramRiscV : public BaseRiscV {
 public:
    std::unique_ptr<FunctionRiscV> functionRiscV;
    void Dump(std::ofstream &outFile) const override {
        outFile << "  .text" << std::endl;
        outFile << "  .global " << *(functionRiscV->name) << std::endl;
        functionRiscV->Dump(outFile);
    }
};

#endif
