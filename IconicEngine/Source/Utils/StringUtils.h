﻿#pragma once
#include <string>
#include <vector>

class StringUtils
{
public:
    static std::vector<std::string> Split(const std::string& Str, const char Delimeter = ',');
    static std::string Join(const std::vector<std::string>& StrArray, const char Delimeter = ',');
    static std::string Replace(const std::string& Str, char From, char To);
};
