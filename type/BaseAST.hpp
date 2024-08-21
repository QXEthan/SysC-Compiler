#include <iostream>
#include <memory>
#include <string>
#include "BaseIR.hpp"

class BaseAST {
 public:
  virtual ~BaseAST() = default;
  
  virtual void Dump() const = 0;
  virtual std::unique_ptr<BaseIR> ToIR() const = 0;
};

class CompUnitAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_def;

  void Dump() const override {
      std::cout << "CompUnitAST { ";
      func_def->Dump();
      std::cout << " }";
  }

  std::unique_ptr<BaseIR> ToIR() const override {
    std::cout << "CompUnitAST ToIR" << std::endl;
    std::unique_ptr<IRProgram> program = std::make_unique<IRProgram>(); 
    program -> funcs = func_def->ToIR();
    return program;
  }
};

class FuncDefAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_type;
  std::string ident;
  std::unique_ptr<BaseAST> block; 

  void Dump() const override {
      std::cout << "FuncDefAST { ";
      func_type->Dump();
      std::cout << ", " << ident << ", ";
      block->Dump();
      std::cout << " }";
  }

  std::unique_ptr<BaseIR> ToIR() const override {
      std::cout << "FuncDefAST ToIR" << std::endl;
      std::unique_ptr<FuncIR> func = std::make_unique<FuncIR>(); 
      func->type = func_type->ToIR();
      func->name = ident;
      func->num_blocks = std::make_unique<int>(1);
      std::cout << "FuncDefAST ToIR before blcok" << std::endl;
      func->blocks = block->ToIR();
      return func;
  }

};

class FuncTypeAST : public BaseAST {
  public:
	std::string Int;
	void Dump() const override {
		std::cout << "FuncTypeAST { " << Int << " }";
	}

  std::unique_ptr<BaseIR> ToIR() const override {
      std::cout << "FuncTypeAST ToIR" << std::endl;
      std::unique_ptr<FuncType> func_type = std::make_unique<FuncType>(); 
      func_type->ret = std::make_unique<std::string>("i32");
      std::cout << "FuncTypeAST ToIR After ret" << std::endl;
      func_type->params_count = std::make_unique<int>(0);;
      std::cout << "FuncTypeAST ToIR end" << std::endl;
      return func_type;
  }

};

class BlockAST : public BaseAST {
  public:
	  std::unique_ptr<BaseAST> stmt;
	  void Dump() const override {
		  std::cout << "BlockAST { ";
		  stmt->Dump();
		  std::cout << " }";
	  }

    std::unique_ptr<BaseIR> ToIR() const override {
      std::cout << "BlockAST ToIR" << std::endl;
      std::unique_ptr<EntryBlock> block = std::make_unique<EntryBlock>(); 
      block->name = std::make_unique<std::string>("entry");
      block->ret = stmt->ToIR();
      return block;
  }
};

class StmtAST : public BaseAST {
	public:
    std::unique_ptr<BaseAST> exp;
		void Dump() const override {
			std::cout << "StmtAST { ";
      exp->Dump();
      std::cout << " }";
		}

    std::unique_ptr<BaseIR>  ToIR() const override {
      std::cout << "StmtAST ToIR" << std::endl;
      std::unique_ptr<Ret> ret = std::make_unique<Ret>();
      ret->expIR = exp->ToIR();
      return ret;
    } 
};

class ExpAST : public BaseAST {
  public:
    std::unique_ptr<BaseAST> addExp;
		void Dump() const override {
			std::cout << "Exp { ";
      addExp->Dump();
      std::cout << " }";
		}

    std::unique_ptr<BaseIR>  ToIR() const override {
      std::cout << "ExpAST ToIR" << std::endl;
      std::unique_ptr<ExpIR> expIR = std::make_unique<ExpIR>();
      expIR->addExpIR = addExp->ToIR();
      return expIR;
    } 
};

class UnaryExpAST : public BaseAST {
  public:
    std::unique_ptr<BaseAST> primaryExp;
    std::unique_ptr<BaseAST> unaryOp;
    std::unique_ptr<BaseAST> unaryExp;

		void Dump() const override {
      if (primaryExp != nullptr) {
        std::cout << "primaryExp { ";
        primaryExp->Dump();
        std::cout << " }";
      }
			else {
        unaryOp->Dump();
        unaryExp->Dump();
      }
		}

    std::unique_ptr<BaseIR>  ToIR() const override {
      std::cout << "UnaryExpAST ToIR" << std::endl;
      std::unique_ptr<UnaryExpIR> unaryExpIR = std::make_unique<UnaryExpIR>();
      if (primaryExp != nullptr) {
        unaryExpIR->primaryExpIR = primaryExp->ToIR();
      }
      else {
        unaryExpIR->unaryOpIR = unaryOp->ToIR();
        unaryExpIR->unaryExpIR = unaryExp->ToIR();
      }
      return unaryExpIR;
    }  
};

class PrimaryExpAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> exp;
    std::unique_ptr<int> number;

		void Dump() const override {
      if (exp != nullptr) {
        std::cout << "(";
        exp->Dump();
        std::cout << ")";
      }
			else {
        std::cout << *number;
      }
		}

    std::unique_ptr<BaseIR>  ToIR() const override {
      std::cout << "PrimaryExpAST ToIR" << std::endl;
      std::unique_ptr<PrimaryExpIR> primaryExpIR = std::make_unique<PrimaryExpIR>();
      if (exp != nullptr) {
        primaryExpIR->expIR = exp->ToIR();
      }
      else {
        primaryExpIR->number = std::make_unique<int>(*number);
      }
      return primaryExpIR;
    }    
};

class UnaryOpAST : public BaseAST {
public:
    std::unique_ptr<std::string> op;

		void Dump() const override {
      std::cout << *op;
		}

    std::unique_ptr<BaseIR>  ToIR() const override {
      std::cout << "UnaryOpAST ToIR" << std::endl;
      std::unique_ptr<UnaryOpIR> unaryOpIR = std::make_unique<UnaryOpIR>();
      unaryOpIR->op = std::make_unique<std::string>(*op);
      return unaryOpIR;
    }    
};

class MulExpAST : public BaseAST {
  public:
      std::unique_ptr<BaseAST> mulExp;
      std::unique_ptr<std::string> op;
      std::unique_ptr<BaseAST> unaryExp;

      void Dump() const override {
        return;
      }
      std::unique_ptr<BaseIR>  ToIR() const override {
        std::cout << "MulExpAST ToIR" << std::endl;
        std::unique_ptr<MulExpIR> mulExpIR = std::make_unique<MulExpIR>();
        if (mulExp == nullptr) {
          mulExpIR->unaryExpIR = unaryExp->ToIR();
        }
        else {
          mulExpIR->op = std::make_unique<std::string>(*op);
          mulExpIR->unaryExpIR = unaryExp->ToIR();
          mulExpIR->mulExpIR = mulExp->ToIR();
        }
        return mulExpIR;
      } 
};

class AddExpAST : public BaseAST {
  public:
      std::unique_ptr<BaseAST> mulExp;
      std::unique_ptr<std::string> op;
      std::unique_ptr<BaseAST> addExp ;

      void Dump() const override {
        return;
      }
      std::unique_ptr<BaseIR>  ToIR() const override {
        std::cout << "AddExpAST ToIR" << std::endl;
        std::unique_ptr<AddExpIR> addExpIR = std::make_unique<AddExpIR>();
        if (addExp == nullptr) {
          addExpIR->mulExpIR = mulExp->ToIR();
        }
        else {
          addExpIR->op = std::make_unique<std::string>(*op);
          addExpIR->addExpIR = addExp->ToIR();
          addExpIR->mulExpIR = mulExp->ToIR();
        }
        return addExpIR;
      } 
};