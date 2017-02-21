#include <cmath>
#include <iostream>
#include <adbase/Logging.h>
#include "WuManber.hpp"

using namespace std;

namespace app {
// {{{ hashCode()

/** 
 * @brief   String hash function.
 * 
 * @param str   the string needed to be hashed
 * @param len   length of the substr should be hashed
 * 
 * @return hash code
 */
unsigned int hashCode(const char* str, int len) {
    unsigned int hash = 0;
    while (*str && len>0) {
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
        --len;
    }
    return (hash & 0x7FFFFFFF);
}

// }}}
// {{{ WuManber::WuManber()

/** 
 * @brief constructor 
 */
WuManber::WuManber() : mMin(0), mTableSize(0), mBlock(3) {
    //VOID
}

// }}}
// {{{ WuManber::init()

/**
 * @brief init
 */
bool WuManber::init(const vector<string>& patterns) {
    int patternSize = patterns.size();

    //check if no pattern specified
    if (patternSize == 0) {
		LOG_ERROR << "wumanber init failed because no pattern specified.";
        return false;
    }
    
    //caculate the minmum pattern length
    mMin = patterns[0].length();
    int32_t lenPattern = 0;
    for (int i = 0; i < patternSize; ++i) {
        lenPattern = patterns[i].length();
        if (lenPattern < mMin) {
            mMin = lenPattern;
        }
    }

    //check if mBlock larger than mMin
    if (mBlock > mMin) {
		LOG_WARN << "mBlock is larger than minmum pattern length, reset mBlock to minmum, but it will seriously affect the effiency.";
        mBlock = mMin;
    }

    //choose a suitable mTableSize for SHIFT, HASH table
    int32_t primes[6] = {1003, 10007, 100003, 1000003, 10000019, 100000007};
    vector<int32_t> primeList(&primes[0], &primes[6]);

    int32_t threshold = 10 * mMin;
    for (size_t i = 0; i < primeList.size(); ++i) {
        if (primeList[i] > patternSize && primeList[i] / patternSize > threshold) {
            mTableSize = primeList[i];
            break;
        }
    }
    
    //if size of patternList is huge.
    if (0 == mTableSize) {
		LOG_WARN << "amount of pattern is very large, will cost a great amount of memory.";
        mTableSize = primeList[5];
    }

    //construct ShiftTable and HashTable, and set default value for SHIFT table
    mPatterns = patterns;
    mHashTable.resize(mTableSize);
    // default value is m-mBlock+1 for shift
    int32_t defaultValue = mMin - mBlock + 1;
    mShiftTable.resize(mTableSize, defaultValue);

    //loop through patterns
    for (int id = 0; id < patternSize; ++id) { 
        // loop through each pattern from right to left
        for (int index = mMin; index >= mBlock; --index) {
            unsigned int hash = hashCode(patterns[id].c_str() + index - mBlock, mBlock) % mTableSize;
            if (mShiftTable[hash] > (mMin - index)) {
                mShiftTable[hash]  = mMin - index;
            }
            if (index == mMin) {
                unsigned int prefixHash = hashCode(patterns[id].c_str(), mBlock);
                mHashTable[hash].push_back(make_pair(prefixHash, id));
            }
        }
    }

    return true;
}

// }}}
// {{{ WuManber::~WuManber()

/** 
 * @brief destructor
 */
WuManber::~WuManber() {
    //VOID
}

// }}}
// {{{ WuManber::search()

/**
 * @public
 * @brief search multiple pattern in text at one time
 */
int WuManber::search(const char* text, const int textLength, ResultSetType& res) {
    //hit count: value to be returned
    int hits = 0;
    int32_t index = mMin - 1; // start off by matching end of largest common pattern
    
    int32_t blockMaxIndex = mBlock - 1;
    int32_t windowMaxIndex = mMin - 1;
    
    while (index < textLength) {
        unsigned int blockHash = hashCode(text + index - blockMaxIndex, mBlock);
        blockHash = blockHash % mTableSize;
        int shift = mShiftTable[blockHash];
        if (shift > 0) {
            index += shift;
        } else {  
            // we have a potential match when shift is 0
            unsigned int prefixHash = hashCode(text + index - windowMaxIndex, mBlock);
            PrefixTableType &element = mHashTable[blockHash];
            PrefixTableType::iterator iter = element.begin();

            while (element.end() != iter) {
                if (prefixHash == iter->first) {   
                    // since prefindex matches, compare target substring with pattern
                    // we know first two characters already match
                    const char* indexTarget = text + index - windowMaxIndex;    //+mBlock
                    const char* indexPattern = mPatterns[iter->second].c_str(); //+mBlock
                    
                    while (('\0' != *indexTarget) && ('\0' != *indexPattern)) {
                        // match until we reach end of either string
                        if (*indexTarget == *indexPattern) {
                            // match against chosen case sensitivity
                            ++indexTarget;
                            ++indexPattern;
                        } else {
                            break;
						}
                    }
                    // match succeed since we reach the end of the pattern.
                    if ('\0' == *indexPattern) {
                        res.insert(string(mPatterns[iter->second]));
                        ++hits;
                    }
                } //end if
                ++iter;
            } //end while
            ++index;
        } //end else
    } //end while

    return hits;
}

// }}}
// {{{ WuManber::search()

/**
 * search
 */
int WuManber::search(const string& str, ResultSetType& res) {
    return search(str.c_str(), str.length(), res);
}

// }}}
// {{{ WuManber::search()

/**
 * search
 */
int WuManber::search(const char* text, const int textLength) {
    //hit count: value to be returned
    int hits = 0;
    int index = mMin - 1; // start off by matching end of largest common pattern

    uint32_t blockMaxIndex = mBlock - 1;
    uint32_t windowMaxIndex = mMin - 1;

    while (index < textLength) {
        unsigned int blockHash = hashCode(text + index - blockMaxIndex, mBlock);
        blockHash = blockHash % mTableSize;
        int shift = mShiftTable[blockHash];
        if (shift > 0) {
            index += shift;
        } else {
            // we have a potential match when shift is 0
            unsigned int prefixHash = hashCode(text + index - windowMaxIndex, mBlock);
            //prefixHash = prefixHash % mTableSize;
            PrefixTableType &element = mHashTable[blockHash];
            PrefixTableType::iterator iter = element.begin();

            while (element.end() != iter) {
                if (prefixHash == iter->first) {
                    // since prefindex matches, compare target substring with pattern
                    // we know first two characters already match
                    const char* indexTarget = text + index - windowMaxIndex;    //+mBlock
                    const char* indexPattern = mPatterns[iter->second].c_str();  //+mBlock

                    while (('\0' != *indexTarget) && ('\0' != *indexPattern)) {
                        // match until we reach end of either string
                        if (*indexTarget == *indexPattern) {
                            // match against chosen case sensitivity
                            ++indexTarget;
                            ++indexPattern;
                        } else {
                            break;
						}
                    }
                    // match succeed since we reach the end of the pattern.
                    if ('\0' == *indexPattern) {
                        ++hits;
                    }
                }//end if
                ++iter;
            }//end while
            ++index;
        }//end else
    }//end while

    return hits;
}

// }}}
// {{{ WuManber::search()

int WuManber::search(const string& str) {
    return search(str.c_str(), str.length());
}

// }}}
}
