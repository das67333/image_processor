#pragma once

#include <string>
#include <vector>

struct FilterDescriptor;

class CmdArgParser {
public:
    void Parse(int argc, const char* argv[]);
    const std::string& GetInputFileName() const;
    const std::string& GetOutputFileName() const;
    const std::vector<FilterDescriptor>& GetFilterDescriptors() const;

protected:
    std::string input_file_name_;
    std::string output_file_name_;
    std::vector<FilterDescriptor> filter_descriptors_;
};
