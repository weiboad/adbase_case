//
//  BinarySearch.h
//  BianrySearch
//
//  Created by kanghua on 16/3/10.
//  Copyright © 2016年 kanghua. All rights reserved.
//

#ifndef  BinarySearch_hpp
#define  BinarySearch_hpp

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include "IPOperator.hpp"
#include <mutex>
#include <unordered_map>
#include <adbase/Logging.hpp>
#include "../Common/tool.hpp"
using namespace std;

typedef unsigned int uint;

typedef struct IPZoneZipCode {
    uint floorIP;
    uint upperIP;
    string zone;
    string zipCode;

    //升序排序时必须写的函数
    bool operator < (const IPZoneZipCode& rhs) const {
        return   floorIP   <  rhs.floorIP;

    }

    //降序排序时必须写的函数
    bool operator > (const IPZoneZipCode& rhs) const {
        return   floorIP > rhs.floorIP;
    }


} *PtrIPZoneZipCode;



bool AscSort(const IPZoneZipCode* pstItem1, const IPZoneZipCode* pstItem2);
bool DesSort(const IPZoneZipCode* pstItem1, const IPZoneZipCode* pstItem2);

class BinarySearch {

// protected:
//     friend class Singleton<BinarySearch>;


private:
    string strIP;
    string filePath;
    string pattern;
    mutable std::mutex mtx;
    static PtrIPZoneZipCode textToIPZoneZipCode(string text, string pattern);
    void clearPtr();


public:
    static Common::tools::FileInfo ipFile;
    static vector<string> split(string str, string pattern);
    BinarySearch(string path, string pattern);
    BinarySearch();
    ~BinarySearch();

    static vector<PtrIPZoneZipCode> fileData;

    vector<PtrIPZoneZipCode> getFileData();

    PtrIPZoneZipCode FindZipCode(string ip);

    bool loadZipCode(string filePath, string pattern);

    bool reLoadZipCode(string filePath, string pattern);

};

#endif /* BinarySearch_h */
