//
//  IPOperator.h
//
//  Created by kanghua on 16/3/10.
//  Copyright © 2016年 kanghua. All rights reserved.
//

#ifndef IPOperator_hpp
#define IPOperator_hpp

#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
typedef unsigned int uint;

using namespace std;

class IPOperator {

private:
    string strIP;

public:
    IPOperator(string ip);
    IPOperator(const char* ip);
    IPOperator(char *ip);

    static bool  IsValidIP(char *ip);
    static bool  IsValidIP(const char *ip);
    static bool  IsValidIP(string ip);

    static uint IPToInt(string str);
    static string IntToIP(uint intIP);
};


#endif /* IPOperator_h */
