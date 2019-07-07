#include "pch.h"
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <cwchar>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>


// unicode to utf8
std::string static wstringToUtf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.to_bytes(str);
}
// utf8 to unicode
std::wstring static utf8ToWstring(const std::string& str)
{
    std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.from_bytes(str);
}

// UTF8 <=> Unicode
std::string static UnicodeToUTF8(const std::wstring & wstr)
{
    std::string ret;
    try {
        std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
        ret = wcv.to_bytes(wstr);
    }
    catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    return ret;
}

std::wstring static UTF8ToUnicode(const std::string & str)
{
    std::wstring ret;
    try {
        std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
        ret = wcv.from_bytes(str);
    }
    catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    return ret;
}
// Unicode <=> ANSI
std::string static UnicodeToANSI(const std::wstring & wstr)
{
    std::string ret;
    char sA[256];
    wsprintfA(sA, "%S", wstr.data());
    ret = sA;
    return ret;

    /*std::mbstate_t state = {};
    const wchar_t *src = wstr.data();
    size_t len = std::wcsrtombs(nullptr, &src, 0, &state);
    if (static_cast<size_t>(-1) != len) {
        std::unique_ptr< char[] > buff(new char[len + 1]);
        len = std::wcsrtombs(buff.get(), &src, len, &state);
        if (static_cast<size_t>(-1) != len) {
            ret.assign(buff.get(), len);
        }
    }
    return ret;*/
}

std::wstring static ANSIToUnicode(const std::string & str)
{
    std::wstring ret;
    std::mbstate_t state = {};
    const char *src = str.data();
    size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
    if (static_cast<size_t>(-1) != len) {
        std::unique_ptr< wchar_t[] > buff(new wchar_t[len + 1]);
        len = std::mbsrtowcs(buff.get(), &src, len, &state);
        if (static_cast<size_t>(-1) != len) {
            ret.assign(buff.get(), len);
        }
    }
    return ret;
}
// UTF-8 <=> ANSI
std::string static UTF8ToANSI(const std::string & str)
{
    return UnicodeToANSI(UTF8ToUnicode(str));
}

std::string static ANSIToUTF8(const std::string & str)
{
    return UnicodeToUTF8(ANSIToUnicode(str));
}
