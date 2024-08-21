#include <iostream>
#include <memory>
#include <string>
#include <fstream> 


static int ir_count = 0;
// for Unary operations
static int ir_number = 0;
static bool ir_isNumber = false;

// for Binary Operations
static int ir_binary_number_1 = 0;
static int ir_binary_number_2 = 0;
static bool ir_binary_isNumber_1 = false;
static bool ir_binary_isNumber_2 = false;


class BaseIR {
 public:
    virtual ~BaseIR() = default;
    virtual void Dump(std::ofstream &outFile) const = 0;
    virtual void ToRV(std::ofstream &outFile) {
        return;
    };
};

class IRProgram : public BaseIR {
 public:
  std::unique_ptr<BaseIR> funcs;
  
  void Dump(std::ofstream &outFile) const override {
    funcs->Dump(outFile);
  }

  void ToRV(std::ofstream &outFile) override {
    funcs->ToRV(outFile);
  }
};

class FuncIR : public BaseIR {
 public:
    std::unique_ptr<BaseIR> type;
    std::string name;
    std::unique_ptr<int> num_blocks;
    std::unique_ptr<BaseIR> blocks;
    void Dump(std::ofstream &outFile) const override {
        outFile << "fun @";
        outFile << name;
        type->Dump(outFile);
        outFile << " {\n";
        blocks->Dump(outFile);
        outFile << "}" << std::endl;
    }
    void ToRV(std::ofstream &outFile) override {
        outFile << "  .text" << std::endl;
        outFile << "  .global " << name << std::endl;
        outFile << name << ":" << std::endl;
        blocks->ToRV(outFile);
    }

};

class IntType : public BaseIR {
 public:
    void Dump(std::ofstream &outFile) const override {
        outFile << "i32";
    }
};

class ArrayType : public BaseIR {
 public:
    std::unique_ptr<int> num;
    std::unique_ptr<BaseIR> type;
    void Dump(std::ofstream &outFile) const override {
        outFile << "[";
        type->Dump(outFile);
        outFile << ", " << *num << "]";
    }
};

class PointerType : public BaseIR {
 public:
    std::unique_ptr<BaseIR> type;
    void Dump(std::ofstream &outFile) const override {
        outFile << "*";
        type->Dump(outFile);
    } 
};

class FuncType : public BaseIR {
 public:
    std::unique_ptr<BaseIR> params;
    std::unique_ptr<std::string> ret;
    std::unique_ptr<int> params_count;
    void Dump(std::ofstream &outFile) const override {
        outFile << "()";
        if (ret != nullptr) {
            outFile << ": " << *ret;
        }
    }    
};

class EntryBlock : public BaseIR{
 public:
    std::unique_ptr<std::string> name;
    std::unique_ptr<BaseIR> ret;
    void Dump(std::ofstream &outFile) const override {
        outFile << "%" << *name << ": \n";
        ret->Dump(outFile);
        outFile << "\n";
    }

    void ToRV(std::ofstream &outFile) override {
        ret->ToRV(outFile);
    }
};

class Ret : public BaseIR {
 public:
    std::unique_ptr<BaseIR> expIR;
    void Dump(std::ofstream &outFile) const override {
        expIR->Dump(outFile);
        outFile << " ret ";
        if (ir_isNumber == true) {
            outFile << ir_number;
        }
        else {
            outFile << "%" << (ir_count - 1);
        }
    }

    void ToRV(std::ofstream &outFile) override {
        return;
    } 
};

class ExpIR : public BaseIR {
 public:
    std::unique_ptr<BaseIR> addExpIR;
    void Dump(std::ofstream &outFile) const override {
        addExpIR->Dump(outFile);
    }

    void ToRV(std::ofstream &outFile) override {
       return;
    }    
};

class UnaryExpIR : public BaseIR {
 public:
    std::unique_ptr<BaseIR> primaryExpIR;
    std::unique_ptr<BaseIR> unaryOpIR;
    std::unique_ptr<BaseIR> unaryExpIR;

    void Dump(std::ofstream &outFile) const override {
        if (primaryExpIR != nullptr) {
            primaryExpIR->Dump(outFile);
        }
        else {
            unaryExpIR->Dump(outFile);
            unaryOpIR->Dump(outFile);
        }
    }

    void ToRV(std::ofstream &outFile) override {
       return;
    } 
};

class PrimaryExpIR : public BaseIR {
 public:
    std::unique_ptr<BaseIR> expIR;
    std::unique_ptr<int> number;

	void Dump(std::ofstream &outFile) const override {
        if (expIR != nullptr) {
            expIR->Dump(outFile);
        }
        else {
            ir_number = *number;
            ir_isNumber = true;
        }
    }

    void ToRV(std::ofstream &outFile) override {
       return;
    }    
};

class UnaryOpIR : public BaseIR {
 public:
    std::unique_ptr<std::string> op;

	void Dump(std::ofstream &outFile) const override {
      if (*op == "NOT") {
        outFile << " %" << ir_count << " = eq ";
        if (ir_isNumber == true) {
            outFile << ir_number << " 0" << std::endl;
            ir_isNumber = false;
        }
        else {
            outFile << "%" << (ir_count - 1) << " 0" << std::endl;
        }
        ir_count ++;
      }
      else if (*op == "SUB") {
        outFile << " %" << ir_count << " = sub 0, ";
        if (ir_isNumber == true) {
            outFile << ir_number << std::endl;
            ir_isNumber = false;
        }
        else {
            outFile << "%" << (ir_count - 1) << std::endl;
        }
        ir_count ++;
      }
	}

    void ToRV(std::ofstream &outFile) override {
       return;
    }    
};

class MulExpIR : public BaseIR {
    public:
        std::unique_ptr<BaseIR> mulExpIR;
        std::unique_ptr<std::string> op;
        std::unique_ptr<BaseIR> unaryExpIR;
        void Dump(std::ofstream &outFile) const override {
            if (mulExpIR == nullptr) {
                unaryExpIR->Dump(outFile);
            }
            else {
                std::unique_ptr<std::string> left;
                std::unique_ptr<std::string> right;

                unaryExpIR->Dump(outFile);
                if (ir_isNumber == false) {
                    left = std::make_unique<std::string>("%" + std::to_string(ir_count));
                }
                else {
                    left = std::make_unique<std::string>(std::to_string(ir_number));
                }

                mulExpIR->Dump(outFile);
                if (ir_isNumber == false) {
                    right = std::make_unique<std::string>("%" + std::to_string(ir_count));
                }
                else {
                    right = std::make_unique<std::string>(std::to_string(ir_number));
                }
                    
                if (*op == "MUL") {
                    outFile << " %" << ir_count << " = mul " << *left << ", " << *right;
                }
                else if (*op == "DIV") {
                    outFile << " %" << ir_count << " = div " << *left << ", " << *right;
                }
                else if (*op == "MOD") {
                    outFile << " %" << ir_count << " = mod " << *left << ", " << *right;
                }
                ir_count ++;
                ir_number = false;
            }
        };
        void ToRV(std::ofstream &outFile) override {
            return;
        } 

};

class AddExpIR : public BaseIR {
    public:
        std::unique_ptr<BaseIR> mulExpIR;
        std::unique_ptr<std::string> op;
        std::unique_ptr<BaseIR> addExpIR;
        void Dump(std::ofstream &outFile) const override {
            if (addExpIR == nullptr) {
                mulExpIR->Dump(outFile);
            }
            else {
                std::unique_ptr<std::string> left;
                std::unique_ptr<std::string> right;

                mulExpIR->Dump(outFile);
                if (ir_isNumber == false) {
                    left = std::make_unique<std::string>("%" + std::to_string(ir_count));
                }
                else {
                    left = std::make_unique<std::string>(std::to_string(ir_number));
                }

                addExpIR->Dump(outFile);
                if (ir_isNumber == false) {
                    right = std::make_unique<std::string>("%" + std::to_string(ir_count));
                }
                else {
                    right = std::make_unique<std::string>(std::to_string(ir_number));
                }
                    
                if (*op == "SUB") {
                    outFile << " %" << ir_count << " = sub " << *left << ", " << *right;
                }
                else if (*op == "ADD") {
                    outFile << " %" << ir_count << " = add " << *left << ", " << *right;
                }
                ir_count ++;
                ir_number = false;
            }
        };
        void ToRV(std::ofstream &outFile) override {
            return;
        } 

};