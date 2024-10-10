#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "../type/BaseAST.hpp"
#include "../type/ASTVisitor.cpp"
#include "../type/IRVisitor.cpp"

using namespace std;

extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);

int main(int argc, const char *argv[]) {
  assert(argc == 5);
  auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];
  ofstream outFile(output);
  if (!outFile) {
        cerr << "Failed to open output file: " << output << endl;
        return 1;
  }

  yyin = fopen(input, "r");
  assert(yyin);

  unique_ptr<BaseAST> ast;
  unique_ptr<BaseIR> ir;
  auto ret = yyparse(ast);
  assert(!ret);

  // generate IR
  CompUnitVisitor* visitor = new CompUnitVisitor();
  unique_ptr<BaseIR> programIR = ast->accept(visitor);
  programIR->Dump(outFile);
  cout << endl;
  cout << endl;
  cout << endl;
  // generate RiscV
  IRProgramVisitor* irVisitor = new IRProgramVisitor();
  unique_ptr<BaseRiscV> riscV = programIR->accept(irVisitor);
  riscV->Dump(outFile);
  cout << endl;
  cout << "After gener RiscV" << endl;

  outFile.close();
  delete(visitor);
  delete(irVisitor);
  return 0;
};
