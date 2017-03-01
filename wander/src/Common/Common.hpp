#ifndef SINA_COMMON_H
#define SINA_COMMON_H

#include<iostream>
#include<stdio.h>
#include<string.h>
#include <stdexcept>

unsigned char char_to_hex(unsigned char x);

int is_alpha_number_char(unsigned char c);

void urlencode(unsigned char *src, unsigned char *dest, int dest_len);

unsigned char *urldecode(unsigned char *encd, unsigned char *decd);

uint32_t getDayZero(time_t timestamp);

uint32_t getTodayZero();

uint32_t getBarrelTime(uint32_t zeroTime, uint32_t currentTime, uint32_t blocksize);

uint16_t getTodayDay();

std::string &replace_all(std::string &str, const std::string &old_value, const std::string &new_value);

time_t getBeforMonthTimeStamp();

uint64_t getTodayZeroName();


//utf8 char length
uint64_t utf8_len(const std::string &str);

//utf8 sub str
std::string utf8_substr(const std::string &str, uint64_t start, uint64_t leng);

std::string string_to_hex(const std::string &input);

std::string hex_to_string(const std::string &input);

#endif //SINA_COMMON_H
