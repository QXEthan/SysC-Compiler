#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>




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
  std::cout << "ret: " << ret << std::endl;
  assert(!ret);

  ast->Dump();
  cout << endl;

  // generate risc-v
  ast->ToIR()->Dump(outFile);
  cout << endl;

  outFile.close();
  return 0;
};
