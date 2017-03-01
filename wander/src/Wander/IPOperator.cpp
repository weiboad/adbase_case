/////////////////////////////////////////////////////////////////
//FileName:KSample.h/data0/BinarySearch/BianrySearch/BianrySearch
//Creator:JohnDoe
//Date:2004-2-421:42:54
//Comment:
/////////////////////////////////////////////////////////////////
#include "IPOperator.hpp"

//构造函数
IPOperator::IPOperator(string ip) {
    this->strIP = ip;
}

IPOperator::IPOperator(char *ip) {
    this->strIP = ip;
}

IPOperator::IPOperator(const char* ip) {
    this->strIP = ip;
}


/**
 *@function IsValidIP
 *@description 判断ip是否合法
 *@param ip char *
 */
bool IPOperator::IsValidIP(char *ip) {
    string strIP = ip;
    return IPOperator::IsValidIP(strIP);
}


/**
 *@function IsValidIP
 *@description 判断ip是否合法
 *@param ip const char *
 */
bool IPOperator::IsValidIP(const char *ip) {
    string strIP = ip;
    return IPOperator::IsValidIP(strIP);
}

/**
 *@function IsValidIP
 *@description 判断ip是否合法
 *@param ip string
 */
bool IPOperator::IsValidIP(string strIP) {
    if(strIP.length() < 1) {
        return false;
    }



    int lenInputStr = (int)strIP.length();
    int countDot=0;
    for(int i= 0; i < lenInputStr; i++){
        if(strIP[i] == '.') {
            countDot++;
            if(i == 0 || i == lenInputStr - 1) {
                return false;
            }
            if(strIP[i-1] < '0'|| strIP[i-1] > '9') {
                return false;
            }
            if(strIP[i+1] < '0'|| strIP[i+1] > '9') {
                return false;
            }

        } else {
            if(strIP[i] < '0'|| strIP[i] > '9') {
                return false;
            }
        }
    }
    if(countDot != 3) {
        return false;
    }

	string *tmp = new string(strIP.c_str());
    char *pCut = strtok(const_cast<char*>(tmp->c_str()), ".");
    //char* pCut = strtok(const_cast<char*>(strIP.c_str()), ".");
    //cout << pCut << endl;

    while( NULL != pCut)
    {
        int num;
        sscanf(pCut, "%d", &num);
        if(num < 0) {
            return false;
        }
        if(num != 0 && pCut[0] == '0') {
            return false;
        }
        if(num > 255) {
            return false;
        }
        //cout << pCut << endl;
        pCut = strtok(NULL, ".");
    }

	delete tmp;
    return true;
}

/**
 *@function IsValidIP
 *@description ip转换uint数字
 *@param strIP string
 *@return unit
 */
uint IPOperator::IPToInt(string strIP) {

    uint i = 3, total = 0;
	string *tmp = new string(strIP.c_str());
    char *pCut = strtok(const_cast<char*>(tmp->c_str()), ".");
    while (pCut != NULL) {
        int cur = atoi(pCut);
        if (cur >= 0 && cur <= 255) {
            total += cur * pow(256, i);
        }
        i--;
        pCut = strtok(NULL, ".");
    }
	delete tmp;
    return total;

}

/**
 *@function IntToIP
 *@description uint转换为string
 *@param strIP string
 *@return unit
 */
string  IPOperator::IntToIP(uint intIP) {
    uint ip[4];

    ip[0] = (intIP >> 24) >> 0;
    ip[1] = ((intIP << 8) >> 24) >> 0;
    ip[2] = (intIP << 16) >> 24;
    ip[3] = (intIP << 24) >> 24;

    string strIP = "";
    for(int i = 0; i < 4;i++) {
        if(ip[i] > 255) {
            return "0.0.0.0";
        }
        string tmp;
        stringstream strStream;
        strStream << ip[i];
        strStream >> tmp;
        strIP = strIP + tmp + ".";
    }
    return strIP.substr(0, strIP.length() - 1);
}
