#include "PatternManager.hpp"
#include <adbase/Logging.hpp>

namespace app {
// {{{ PatternManager::PatternManager()

PatternManager::PatternManager(AdbaseConfig* config) :
    _config(config) {
}

// }}}
// {{{ PatternManager::~PatternManager()

PatternManager::~PatternManager() {
    if (!_wuManbers.size()) {
        return;
    }

    for (auto & t: _wuManbers) {
        if (t.second != nullptr) {
            delete t.second;
            t.second = nullptr;
        }
    }
}

// }}}
// {{{ void PatternManager::init()

void PatternManager::init() {
    if (!_config->patternConfig.size()) {
        LOG_FATAL << "Must has pattern text config.";
    }

    for (auto &t : _config->patternConfig) {
        std::vector<std::string> patterns;
        std::ifstream pat(t.second);
        if (!pat.good() || !pat.is_open()) {
            LOG_FATAL << "Pattern type :" << t.first << " config file: " << t.second << " is not good.";
        }
        std::string str;
        while (pat>> str) {
            patterns.push_back(str);
        }

        WuManber* wumanber = new WuManber;
        wumanber->init(patterns);
        _wuManbers[t.first] = wumanber; 
        LOG_INFO << "Init dict " << t.first << " complete.";
    }
}

// }}}
// {{{ int PatternManager::search() 

int PatternManager::search(std::string& type, std::string& text, std::set<std::string>& patternResult) {
    if (_wuManbers.find(type) == _wuManbers.end()) {
        LOG_INFO << "Search pattern type `" << type << "` not exists."; 
        return -1;
    }   

    WuManber* wumanber = _wuManbers[type];
    wumanber->search(text, patternResult);
    return static_cast<int>(patternResult.size());
}

// }}}
// {{{ int PatternManager::search() 

int PatternManager::search(std::string& type, std::string& text) {
    if (_wuManbers.find(type) == _wuManbers.end()) {
        LOG_INFO << "Search pattern type `" << type << "` not exists."; 
        return -1;
    }   

    WuManber* wumanber = _wuManbers[type];
    return wumanber->search(text);
}

// }}}
}
