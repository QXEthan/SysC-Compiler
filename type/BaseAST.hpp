#ifndef BASEAST_H
#define BASEAST_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "Visitor.hpp"

class BaseIR;

class BaseAST {
 public:
  virtual ~BaseAST() = default;
  
  virtual void Dump() const = 0;
  virtual std::unique_ptr<BaseIR> accept(const Visitor *v) const = 0;
};

class CompUnitAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_def;

  void Dump() const override {
      std::cout << "CompUnitAST { ";
      func_def->Dump();
      std::cout << " }";
  }

  std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
    return v->visit(this);
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

  std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
    return v->visit(this);
  }

};

class FuncTypeAST : public BaseAST {
  public:
	std::string Int;
	void Dump() const override {
		std::cout << "FuncTypeAST { " << Int << " }";
	}

  std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
    return v->visit(this);
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

    std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
      return v->visit(this);
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

    std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
      return v->visit(this);
    }
};

class ExpAST : public BaseAST {
  public:
    std::unique_ptr<BaseAST> lOrExp;
		void Dump() const override {
			std::cout << "Exp { ";
      lOrExp->Dump();
      std::cout << " }";
		}

    std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
      return v->visit(this);
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
    std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
      return v->visit(this);
    }
     
};

class UnaryOpAST : public BaseAST {
public:
    std::unique_ptr<std::string> op;

		void Dump() const override {
      std::cout << *op;
		}

    std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
      return v->visit(this);
    }    
};

class UnaryExpAST : public BaseAST {
  public:
    std::unique_ptr<PrimaryExpAST> primaryExp;
    std::unique_ptr<UnaryOpAST> unaryOp;
    std::unique_ptr<UnaryExpAST> unaryExp;

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

    std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
      return v->visit(this);
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
      std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
        return v->visit(this);
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
      std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
        return v->visit(this);
      }
};

class RelExpAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> addExp;
  std::unique_ptr<std::string> op;
  std::unique_ptr<BaseAST> relExp;

  void Dump() const override {
    return;
  }
  std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
    return v->visit(this);
  }
};
 
class EqExpAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> relExp;
  std::unique_ptr<std::string> op;
  std::unique_ptr<BaseAST> eqExp;

  void Dump() const override {
    return;
  }
  std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
    return v->visit(this);
  }
};

class LAndExpAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> lAndExp;
  std::unique_ptr<std::string> op;
  std::unique_ptr<BaseAST> eqExp;

  void Dump() const override {
    return;
  }
  std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
    return v->visit(this);
  }
};

class LOrExpAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> lAndExp;
  std::unique_ptr<std::string> op;
  std::unique_ptr<BaseAST> lOrExp;

  void Dump() const override {
    return;
  }
  std::unique_ptr<BaseIR> accept(const Visitor *v) const override {
    return v->visit(this);
  }
};

#endif