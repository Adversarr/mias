#include <krill/grammar.h>
#include <memory>
namespace details {

class Parser {
public:
  Parser() = default;
};

void init();

} // namespace details

inline void init() { details::init(); }
