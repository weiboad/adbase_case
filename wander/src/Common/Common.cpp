#include "Common.hpp"

unsigned char char_to_hex(unsigned char x) {
    return (unsigned char) (x > 9 ? x + 55 : x + 48);
}

int is_alpha_number_char(unsigned char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        return 1;
    return 0;
}

//url编码实现

void urlencode(unsigned char *src, unsigned char *dest, int dest_len) {
    unsigned char ch;
    int len = 0;

    while (len < (dest_len - 4) && *src) {
        ch = (unsigned char) *src;
        if (*src == ' ') {
            *dest++ = '+';
        }
        else if (is_alpha_number_char(ch) || strchr("=!~*'()", ch)) {
            *dest++ = *src;
        }
        else {
            *dest++ = '%';
            *dest++ = char_to_hex((unsigned char) (ch >> 4));
            *dest++ = char_to_hex((unsigned char) (ch % 16));
        }
        ++src;
        ++len;
    }
    *dest = 0;
    return;
}



//解url编码实现

unsigned char *urldecode(unsigned char *encd, unsigned char *decd) {
    unsigned int j, i;
    char *cd = (char *) encd;
    char p[2];
    j = 0;

    for (i = 0; i < strlen(cd); i++) {
        memset(p, '\0', 2);
        if (cd[i] != '%') {
            decd[j++] = cd[i];
            continue;
        }

        p[0] = cd[++i];
        p[1] = cd[++i];

        p[0] = p[0] - 48 - ((p[0] >= 'A') ? 7 : 0) - ((p[0] >= 'a') ? 32 : 0);
        p[1] = p[1] - 48 - ((p[1] >= 'A') ? 7 : 0) - ((p[1] >= 'a') ? 32 : 0);
        decd[j++] = (unsigned char) (p[0] * 16 + p[1]);

    }
    decd[j] = '\0';

    return decd;
}


uint32_t getBarrelTime(uint32_t zeroTime, uint32_t currentTime, uint32_t blocksize) {
    if (currentTime - zeroTime <= 0 || currentTime <= 0 || blocksize <= 0) {
        return 0;
    }

    uint32_t diffTime = currentTime - zeroTime;
    uint32_t BarrelTime = zeroTime + ((diffTime / blocksize) * blocksize);
    return BarrelTime;
}

uint16_t getTodayDay() {
    time_t timep;
    struct tm *p;
    time(&timep);

    p = localtime(&timep);
    return p->tm_mday;
}

uint32_t getTodayZero() {
    time_t timep;
    time(&timep);

    return getDayZero(timep);
}

//获取当前日期的00:00:00的timestamp
uint32_t getDayZero(time_t timestamp) {
    return timestamp - timestamp % (24 * 60 * 60);
}

std::string &replace_all(std::string &str, const std::string &old_value, const std::string &new_value) {
    while (true) {
        std::string::size_type pos(0);
        if ((pos = str.find(old_value)) != std::string::npos)
            str.replace(pos, old_value.length(), new_value);
        else break;
    }
    return str;
}

time_t getBeforMonthTimeStamp() {
    time_t timep;
    struct tm *p;
    time(&timep);

    p = localtime(&timep);
    if (p->tm_mon == 0) {
        p->tm_year = (p->tm_year - 1);
        p->tm_mon = 11;
    } else {
        p->tm_mon = (p->tm_mon - 1);
    }

    p->tm_mday = 1;

    time_t num = mktime(p);
    return num;
}

//显示string 16进制
std::string string_to_hex(const std::string &input) {
    static const char *const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i) {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

//16进制字符串翻译成 string
std::string hex_to_string(const std::string &input) {
    static const char *const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");

    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2) {
        char a = input[i];
        const char *p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char *q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw std::invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}


uint64_t utf8_len(const std::string &str) {
    uint64_t i = 0;
    uint64_t count = 0;
    uint64_t c;

    for (i = 0; i < str.length(); i++) {
        count++;

        c = (unsigned char) str[i];
        if (c <= 127) i += 0;
        else if ((c & 0xE0) == 0xC0) i += 1;
        else if ((c & 0xF0) == 0xE0) i += 2;
        else if ((c & 0xF8) == 0xF0) i += 3;
            //else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
            //else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
        else return 0;//invalid utf8
    }
    return count;
}

std::string utf8_substr(const std::string &str, uint64_t start, uint64_t leng) {
    if (leng == 0) { return ""; }
    uint64_t c, i, ix, q, min = std::string::npos, max = std::string::npos;
    for (q = 0, i = 0, ix = str.length(); i < ix; i++, q++) {
        if (q == start) { min = i; }
        if (q <= start + leng || leng == std::string::npos) { max = i; }

        c = (unsigned char) str[i];
        if (c <= 127) i += 0;
        else if ((c & 0xE0) == 0xC0) i += 1;
        else if ((c & 0xF0) == 0xE0) i += 2;
        else if ((c & 0xF8) == 0xF0) i += 3;
            //else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
            //else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
        else return "";//invalid utf8
    }
    if (q <= start + leng || leng == std::string::npos) { max = i; }
    if (min == std::string::npos || max == std::string::npos) { return ""; }
    return str.substr(min, max - min);
}


uint64_t getTodayZeroName() {
    //get today
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep);
    char timestr[20];
    snprintf(timestr, 20, "%04d%02d%02d", (p->tm_year - 100 + 2000), p->tm_mon + 1, p->tm_mday);
    uint64_t todaytime = strtoul(timestr, NULL, 10) * 10000;
    return todaytime;
}
