#include "BinarySearch.hpp"

vector<PtrIPZoneZipCode> BinarySearch::fileData;

Common::tools::FileInfo BinarySearch::ipFile;

//自定义的结构体比较函数，升序排列
bool AscSort(const IPZoneZipCode* pstItem1, const IPZoneZipCode* pstItem2)
{
    return pstItem1->floorIP < pstItem2->floorIP;
}

//自定义的结构体比较函数，升序排列
bool DesSort(const IPZoneZipCode* pstItem1, const IPZoneZipCode* pstItem2)
{
    return pstItem1->floorIP > pstItem2->floorIP;
}



BinarySearch::BinarySearch(string path, string pattern) {
    filePath = path;
    pattern = pattern;
    loadZipCode(path, pattern);

}

BinarySearch::~BinarySearch() {
    clearPtr();
}

BinarySearch::BinarySearch() {
}


/**
 *@function BinarySearch::split
 *@description 字符串分隔
 *@param strIP   str
         pattern string   分隔符
 *@return vector<string>
 */
vector<string> BinarySearch::split(string str, string pattern) {
    std::vector<std::string> result;
    char* s = const_cast<char*>(str.c_str());
    const char *d = pattern.c_str();
    char *p;
    p = strtok(s,d);
    while(p)
    {
        result.push_back(p);
        p=strtok(NULL,d);
    }

    return result;
}


/**
 *@function BinarySearch::textToIPZoneZipCode
 *@description 文本数据转换为结构体格式
 *@param text    string   文本内容
         pattern string   分隔符
 *@return PtrIPZoneZipCode
 */
PtrIPZoneZipCode BinarySearch::textToIPZoneZipCode(string text, string pattern){
    PtrIPZoneZipCode tmp = new IPZoneZipCode();
    vector<string> textProcess = split(text, pattern);
    if(textProcess.size() == 3) {
        tmp->zone = textProcess[1];
        tmp->zipCode = textProcess[2];

        vector<string> textProcessIP = split(textProcess[0], "-");

        if(textProcessIP.size() == 2) {
            tmp->floorIP = IPOperator::IPToInt(textProcessIP[0]);
            tmp->upperIP = IPOperator::IPToInt(textProcessIP[1]);
        }
    }
    return tmp;
}

/**
 *@function BinarySearch::clearPtr
 *@description 清除指针数据
 *@return void
 */
void BinarySearch::clearPtr() {
    if(!fileData.empty()) {
        std::lock_guard<std::mutex> lck (mtx);
		for(vector<PtrIPZoneZipCode>::iterator it = fileData.begin();it != fileData.end();it++) {
			if(NULL != *it) {
				delete *it;
				*it = NULL;
			}
		}
        fileData.erase(fileData.begin(), fileData.end());
		//fileData.clear();
    }
}


/**
 *@function BinarySearch::loadZipCode
 *@description 加载文本数据
 *@param filePath  string   文本路径
         pattern   string   分隔符
 *@return bool
 */
bool BinarySearch::loadZipCode(string filePath, string pattern) {

    string s;
    std::lock_guard<std::mutex> lck (mtx);
    ifstream fp(filePath.c_str());

    if(!fp) {

        cout << "file open error" << endl;
        return false;
    }
    LOG_INFO << "init size:" << fileData.size();
    while (getline(fp,s)) {
        fileData.push_back(BinarySearch::textToIPZoneZipCode(s, pattern));
    }
    LOG_INFO << "after init size:" << fileData.size();

    //如果不是升序排列,则进行排序处理
    //if(!is_sorted(fileData.begin(), fileData.end(), AscSort)) {
        sort(fileData.begin(), fileData.end(), AscSort);
    //}
    return true;
}

/**
 *@function BinarySearch::reLoadZipCode
 *@description 重新加载文本数据
 *@param filePath  string   文本路径
         pattern   string   分隔符
 *@return bool
 */
bool BinarySearch::reLoadZipCode(string filePath, string pattern) {
    clearPtr();
    return loadZipCode(filePath, pattern);
}


/**
 *@function BinarySearch::FindZipCode
 *@description 查找邮编
 *@param  IP    string
 *@return string
 */
PtrIPZoneZipCode BinarySearch::FindZipCode(string ip) {
    std::string result;
    if(!IPOperator::IsValidIP(ip)) {
        result = "IP illegal";
        throw result;
    }
    if(fileData.size() == 0) {
        result = "no zipcode data";
        throw result;

    }

    int recordSize = (int)fileData.size();

    int startIndex = 0;
    int endIndex = recordSize - 1;
    uint searchNum = IPOperator::IPToInt(ip);

    if(fileData.size() != 0) {
		if(searchNum < fileData[startIndex]->floorIP || searchNum > fileData[endIndex]->upperIP) {
            result = "can not find position";
            throw result;
		}
	}

    while((endIndex - startIndex) > 0) {

        int middleIndex = (startIndex + endIndex)/2;

        if(fileData[middleIndex]->floorIP == searchNum) {
			return fileData[middleIndex];
        } else if(fileData[middleIndex]->floorIP > searchNum){
            endIndex = middleIndex - 1;
        } else {
            startIndex = middleIndex + 1;
        }
    }

    if(startIndex > 0) {
        if(fileData[startIndex - 1]->upperIP >= searchNum &&
               fileData[startIndex - 1]->floorIP <= searchNum) {
			return fileData[startIndex - 1];
        }
    }


    if(fileData[startIndex]->upperIP >= searchNum && fileData[startIndex]->floorIP <= searchNum) {
		return fileData[startIndex];
    }

    /*if(fileData[startIndex + 1]->upperIP >= searchNum && fileData[startIndex + 1]->floorIP <= searchNum){
        return fileData[startIndex + 1]->zipCode;
    }*/
    result = "can not find position";
    throw result;
    //return nullptr;
}
