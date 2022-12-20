#include <krill/grammar.h>
#include <memory>
namespace details {


extern std::unique_ptr<krill::grammar::ActionTable> parsing_table;

extern std::unique_ptr<std::map<std::string, std::string>> lexical_desc;

extern std::unique_ptr<std::vector<std::string>> grammar_desc;

void init();

} // namespace details

inline void init() { details::init(); }
