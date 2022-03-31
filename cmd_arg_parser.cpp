#include "cmd_arg_parser.h"

#include "exceptions.h"
#include "filter_pipeline_factory.h"

void CmdArgParser::Parse(int argc, const char* argv[]) {
    if (argc < 3) {
        throw HelpMessage();
    }
    input_file_name_ = *++argv;
    output_file_name_ = *++argv;
    if (*(argv + 1) && **(argv + 1) != '-') {
        throw InvalidCmdArgument("Filter name must start with '-'");
    }
    FilterDescriptor descr;
    bool found_filter = false;
    while (*++argv) {
        if (**argv == '-') {
            if (found_filter) {
                filter_descriptors_.push_back(std::move(descr));
                descr.name.clear();
                descr.params.clear();
            }
            descr.name = *argv + 1;
            found_filter = true;
        } else {
            descr.params.emplace_back(*argv);
        }
    }
    if (found_filter) {
        filter_descriptors_.push_back(descr);
    }
}

const std::string& CmdArgParser::GetInputFileName() const {
    return input_file_name_;
}

const std::string& CmdArgParser::GetOutputFileName() const {
    return output_file_name_;
}

const std::vector<FilterDescriptor>& CmdArgParser::GetFilterDescriptors() const {
    return filter_descriptors_;
}
