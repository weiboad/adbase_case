#ifndef PATTERN_PATTERNMANAGER_HPP_
#define PATTERN_PATTERNMANAGER_HPP_

#include <unordered_map>
#include "AdbaseConfig.hpp"
#include "WuManber.hpp"
#include <fstream>
#include <set>

namespace app {

typedef std::unordered_map<std::string, std::string> PatternConfig;
typedef std::unordered_map<std::string, WuManber*> WuManberMap;
class PatternManager {
public:
    PatternManager(AdbaseConfig* config);
    void init();
    int search(std::string& type, std::string& text, std::set<std::string>& patternResult);
    int search(std::string& type, std::string& text);
    ~PatternManager();

private:
    AdbaseConfig* _config;
    WuManberMap _wuManbers;
};

}

#endif
