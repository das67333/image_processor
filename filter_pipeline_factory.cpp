#include "filter_pipeline_factory.h"

#include "exceptions.h"
#include "filter_pipeline.h"

void FilterPipelineFactory::Configure(std::initializer_list<std::pair<std::string, FilterFactoryPtr>> creators) {
    filter_creators_.clear();
    for (const auto& [name, ptr] : creators) {
        if (ptr) {
            filter_creators_.insert({name, ptr});
        }
    }
}

FilterPipeline FilterPipelineFactory::CreateFilterPipeline(const std::vector<FilterDescriptor>& vfd) const {
    FilterPipeline fp;
    for (const FilterDescriptor& fd : vfd) {
        BaseFilter* filter = CreateFilter(fd);
        fp.AddFilter(filter);
    }
    return fp;
}

FilterPipelineFactory::FilterFactoryPtr FilterPipelineFactory::GetFilterCreator(const std::string& name) const {
    auto it = filter_creators_.find(name);
    if (it == filter_creators_.end()) {
        throw InvalidCmdArgument("Filter '" + name + "' doesn't exist");
    }
    return it->second;
}

BaseFilter* FilterPipelineFactory::CreateFilter(const FilterDescriptor& fd) const {
    FilterFactoryPtr creator = GetFilterCreator(fd.name);
    return creator(fd);
}
