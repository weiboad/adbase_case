#ifndef PATTERN_WUMANBER_HPP
#define PATTERN_WUMANBER_HPP
#include <stdint.h>

#include <vector>
#include <string>
#include <set>

namespace app {

typedef std::set<std::string> ResultSetType;
typedef std::pair<unsigned int, int> PrefixIdPairType;
typedef std::vector<PrefixIdPairType> PrefixTableType;

class WuManber {
    public:
        WuManber();
        ~WuManber();

        /**
         * init Function
         * 
         * @param patterns      pattern list to be matched
         */
        bool init(const std::vector<std::string>& patterns);

        /** 
         * @param text           raw text
         * @param textLength     length of text
         * @param res            string set containing matched patterns
         * 
         * @return value 0: no pattern matchs, n: n patterns matched(n>0)
         */
        int search( const char* text, const int textLength, ResultSetType& res);

        /**
         * @param  str           raw text
         * @param  res           string set containing matched patterns
         *
         * @return value 0: no pattern matchs, n: n patterns matched(n>0)
         */
         int search(const std::string& str, ResultSetType& res);

        /**
         * @brief search text 
         *
         * @return value 0: no pattern matchs, n: n patterns matched(n>0)
         */
        int search(const char* text, const int textLength);

        /**
         * @brief search text
         *
         * @return value 0: no pattern matchs, n: n patterns matched(n>0)
         */
        int search(const std::string& str);

    private:
        // minmum length of patterns
        int32_t mMin;
        // SHIFT table
        std::vector<int32_t> mShiftTable;
        // a combination of HASH and PREFIX table 
        std::vector<PrefixTableType> mHashTable;
        // patterns
        std::vector<std::string> mPatterns;
        // size of SHIFT and HASH table
        int32_t mTableSize;
        // size of block
        int32_t mBlock;
};
}

#endif
