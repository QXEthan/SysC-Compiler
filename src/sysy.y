%code requires {
  #include <memory>
  #include <string>
}

%{

#include <iostream>
#include <memory>
#include <string>
#include "../type/BaseAST.hpp"

int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

using namespace std;

%}


%parse-param { std::unique_ptr<BaseAST> &ast }

%union {
  std::string *str_val;
  int int_val;
  BaseAST *ast_val;
}

%token INT RETURN ADD SUB LN MUL DIV MOD LT GT LE GE EQ NE LAND LOR
%token <str_val> IDENT
%token <int_val> INT_CONST

%type <ast_val> FuncDef FuncType Block Stmt Exp UnaryExp PrimaryExp UnaryOp MulExp AddExp RelExp EqExp LAndExp LOrExp     
%type <int_val> Number
%%

CompUnit
  : FuncDef {
    auto comp_unit = make_unique<CompUnitAST>();
    comp_unit->func_def = unique_ptr<BaseAST>($1);
    ast = std::move(comp_unit);
  }
  ;

FuncDef
  : FuncType IDENT '(' ')' Block {
    auto ast = new FuncDefAST();
    ast->func_type = unique_ptr<BaseAST>($1);
    ast->ident = *unique_ptr<string>($2);
    ast->block = unique_ptr<BaseAST>($5);
    $$ = ast;
  }
  ;

FuncType
  : INT {
    auto ast = new FuncTypeAST();
    ast->Int = *(new string("int"));
    $$ = ast;
  }
  ;

Block
  : '{' Stmt '}' {
  auto ast = new BlockAST();
  ast->stmt = unique_ptr<BaseAST>($2);
  $$ = ast; 

  }
  ;

Stmt
  : RETURN Exp ';' {
  auto ast = new StmtAST();
  ast->exp = unique_ptr<BaseAST>($2);
  $$ = ast; 
  }
  ;

Exp
  : LOrExp {
    auto ast = new ExpAST();
    ast->lOrExp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

UnaryExp
 : PrimaryExp {
  auto ast = new UnaryExpAST();
  ast->primaryExp = unique_ptr<PrimaryExpAST>(dynamic_cast<PrimaryExpAST*> ($1));
  $$ = ast;
 }
 | UnaryOp UnaryExp {
  auto ast = new UnaryExpAST();
  ast->unaryOp = unique_ptr<UnaryOpAST>(dynamic_cast<UnaryOpAST*> ($1));
  ast->unaryExp = unique_ptr<UnaryExpAST>(dynamic_cast<UnaryExpAST*> ($2));
  $$ = ast;
 }
 ;

PrimaryExp
 : '(' Exp ')' {
  auto ast = new PrimaryExpAST();
  ast->exp = unique_ptr<BaseAST>($2);
  $$ = ast;
 }
 | Number {
    auto ast = new PrimaryExpAST();
    ast->number = unique_ptr<int>(new int($1));
    $$ = ast;
 }
 ;

Number
   : INT_CONST {
     $$ = $1;
    }
    ;

UnaryOp
 : ADD {
  auto ast = new UnaryOpAST();
  ast-> op = make_unique<string>("ADD");
  $$ = ast;
 }
 | SUB {
  auto ast = new UnaryOpAST();
  ast-> op = make_unique<string>("SUB");
  $$ = ast;
 }
 | LN {
  auto ast = new UnaryOpAST();
  ast-> op = make_unique<string>("NOT");
  $$ = ast;
 }
 ;

 MulExp
 : UnaryExp {
  auto ast = new MulExpAST();
  ast-> unaryExp = unique_ptr<BaseAST>($1);
  $$ = ast;
 }
 | MulExp MUL UnaryExp {
  auto ast = new MulExpAST();
  ast-> mulExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("MUL");
  ast-> unaryExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 | MulExp DIV UnaryExp {
  auto ast = new MulExpAST();
  ast-> mulExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("DIV");
  ast-> unaryExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 | MulExp MOD UnaryExp {
  auto ast = new MulExpAST();
  ast-> mulExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("MOD");
  ast-> unaryExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 ;

 AddExp
 : MulExp {
  auto ast = new AddExpAST();
  ast-> mulExp = unique_ptr<BaseAST>($1);
  $$ = ast;
 }
 | AddExp ADD MulExp {
  auto ast = new AddExpAST();
  ast-> addExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("ADD");
  ast-> mulExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 | AddExp SUB MulExp {
  auto ast = new AddExpAST();
  ast-> addExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("SUB");
  ast-> mulExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 ;

 RelExp
 : AddExp {
  auto ast = new RelExpAST();
  ast-> addExp = unique_ptr<BaseAST>($1);
  $$ = ast;
 }
 | RelExp LT AddExp {
  auto ast = new RelExpAST();
  ast-> relExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("LT");
  ast-> addExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 | RelExp GT AddExp {
  auto ast = new RelExpAST();
  ast-> relExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("GT");
  ast-> addExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 | RelExp LE AddExp {
  auto ast = new RelExpAST();
  ast-> relExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("LE");
  ast-> addExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 | RelExp GE AddExp {
  auto ast = new RelExpAST();
  ast-> relExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("GE");
  ast-> addExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 ;

EqExp
: RelExp {
  auto ast = new EqExpAST();
  ast-> relExp = unique_ptr<BaseAST>($1);
  $$ = ast;  
}
| EqExp EQ RelExp {
  auto ast = new EqExpAST();
  ast-> eqExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("EQ");
  ast-> relExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 | EqExp NE RelExp {
  auto ast = new EqExpAST();
  ast-> eqExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("NE");
  ast-> relExp = unique_ptr<BaseAST>($3);
  $$ = ast;
 }
 ;

LAndExp
: EqExp {
  auto ast = new LAndExpAST();
  ast-> eqExp = unique_ptr<BaseAST>($1);
  $$ = ast; 
}
| LAndExp LAND EqExp {
  auto ast = new LAndExpAST();
  ast-> lAndExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("LAND");
  ast-> eqExp = unique_ptr<BaseAST>($3);
  $$ = ast;
}
;

LOrExp
: LAndExp {
  auto ast = new LOrExpAST();
  ast-> lAndExp = unique_ptr<BaseAST>($1);
  $$ = ast; 
}
| LOrExp LOR LAndExp {
  auto ast = new LOrExpAST();
  ast-> lOrExp = unique_ptr<BaseAST>($1);
  ast-> op = make_unique<string>("LOR");
  ast-> lAndExp = unique_ptr<BaseAST>($3);
  $$ = ast;
}
;

%%

void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}

