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

%token INT RETURN ADD SUB LN MUL DIV MOD
%token <str_val> IDENT
%token <int_val> INT_CONST

%type <ast_val> FuncDef FuncType Block Stmt Exp UnaryExp PrimaryExp UnaryOp MulExp AddExp     
%type <int_val> Number
%%

CompUnit
  : FuncDef {
    auto comp_unit = make_unique<CompUnitAST>();
    comp_unit->func_def = unique_ptr<BaseAST>($1);
    ast = move(comp_unit);
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
  : AddExp {
    auto ast = new ExpAST();
    ast->addExp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

UnaryExp
 : PrimaryExp {
  auto ast = new UnaryExpAST();
  ast->primaryExp = unique_ptr<BaseAST>($1);
  $$ = ast;
 }
 | UnaryOp UnaryExp {
  auto ast = new UnaryExpAST();
  ast->unaryOp = unique_ptr<BaseAST>($1);
  ast->unaryExp = unique_ptr<BaseAST>($2);
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


%%

void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}

