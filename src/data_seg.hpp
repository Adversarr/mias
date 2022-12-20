
#include <map>
#include <vector>
namespace details {

class DataSegParser {

public:
  DataSegParser();

  void Reset();

  void Parse(const std::vector<std::string> &clean_input);

  enum class CommandType {
    kEmpty, kVar, kSpace, kAlign, kEnd, kData
  };


private:
  CommandType GetNextCommandType() const;

  void RunVarDefinitionCommand();

  void RunDataDefinitionCommand();

  void RunSpaceCommand();

  void RunAlignCommand();

public:
  std::vector<uint32_t> GenerateOutput() const;

private:

  std::vector<std::string> input_str_;

  std::vector<uint32_t> dmem_;

  size_t dmem_usage_;

  std::map<std::string, size_t> addr_name_;

  size_t index_;

  


};

} // namespace details
