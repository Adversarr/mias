
#include "parser.h"

namespace details {

std::unique_ptr<krill::grammar::ActionTable> parsing_table;

std::unique_ptr<std::map<std::string, std::string>> lexical_desc;

std::unique_ptr<std::vector<std::string>> grammar_desc;

void init() {
  std::map<std::string, std::string> lex;
  lex.insert({"tData", "\\.data"});
  lex.insert({"tText", "\\.text"});
  lex.insert({"tWordNum", "0x[0-9a-f]+|[0-9]+"});
  lex.insert({"tEndl", "\n|#[\n]*\n"});
  lex.insert({"tRCom", "(add)|(addu)|(sub)|(subu)|(and)|(or)|(xor)|(nor)|(slt)|(sltu)|(sllv)|(srlv)|(srav)"});
  lex.insert({"tICom", "(addi)|(addiu)|(andi)|(ori)|(xori)|(slti)|(sltiu)"});
  lex.insert({"tLWICom", "(lb)|(lbu)|(lh)|(lhu)|(sb)|(sh)|(lw)|(sw)"});
  lex.insert({"tDRCom", "(mult)|(multu)|(div)|(divu)|(mfc0)|(mtc0)|(jalr)"});
  lex.insert({"tJCom", "(j)|(jalr)"});
  lex.insert({"tSyscall", "syscall"});
  lex.insert({"tIdName", "[a-zA-Z][0-9a-zA-Z]*"});
  lex.insert({"tPCom", "(push)|(pop)"});
  lex.insert({"tHalf", "\\.half"});
  lex.insert({"tFloat", "\\.float"});
  lex.insert({"tAscii", "\\.ascii"});
  lexical_desc =
      std::make_unique<std::map<std::string, std::string>>(std::move(lex));

  std::vector<std::string> grammar_desc;
  grammar_desc.push_back("nPro -> nData nText");
  grammar_desc.push_back("nData -> nDataSeg nVars");
  grammar_desc.push_back("nDataSeg -> tData nSegAddr tEndl");
  grammar_desc.push_back("nSegAddr -> tWordNum");
  grammar_desc.push_back("nSegAddr -> ");
  grammar_desc.push_back("nVars -> nVar nVars");
  grammar_desc.push_back("nVars -> ");
  grammar_desc.push_back("nVar -> tIdName tColon nVarData");
  grammar_desc.push_back("nVarData -> nWordData tEndl nVarData");
  grammar_desc.push_back("nVarData -> nHalfData tEndl nVarData");
  grammar_desc.push_back("nVarData -> nByteData tEndl nVarData");
  grammar_desc.push_back("nVarData -> nFordData tEndl nVarData");
  grammar_desc.push_back("nVarData -> nAsciiData tEndl nVarData");
  grammar_desc.push_back("nVarData -> nSoraData tEndl nVarData");
  grammar_desc.push_back("nVarData -> ");
  // grammar_desc.
  //



}

} // namespace details
