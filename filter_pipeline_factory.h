#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class BaseFilter;
class FilterPipeline;

struct FilterDescriptor {
    std::string name;
    std::vector<std::string> params;
};

class FilterPipelineFactory {
public:
    using FilterFactoryPtr = BaseFilter* (*)(const FilterDescriptor&);
    using FilterToFactoryMap = std::unordered_map<std::string, FilterFactoryPtr>;

    void Configure(std::initializer_list<std::pair<std::string, FilterFactoryPtr>> creators);
    FilterPipeline CreateFilterPipeline(const std::vector<FilterDescriptor>& vfd) const;

protected:
    FilterFactoryPtr GetFilterCreator(const std::string& name) const;
    BaseFilter* CreateFilter(const FilterDescriptor& fd) const;

    FilterToFactoryMap filter_creators_;
};
