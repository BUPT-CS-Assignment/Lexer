#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#include "basic.h"
#include "notation.h"
#include "indicator.hpp"
#include "symbol.hpp"

class Scanner
{
    const static int __buffer_length = 256;
    const static int __buffer_mid = __buffer_length / 2;
private:
    enum class ErrorType{
        INVALID_WORD,
        EXPECTED_END,
    };
    // File stream
    std::string _file_name;
    std::ifstream _ifs, _ifs_line;
    // String buffer
    char _line_buffer[__buffer_length] = {0};
    char _file_buffer[__buffer_length] = {0};
    int _type_counter[9] = {0};
    int _ptr_buffer = -1;
    int _total_char = 0;

    // Current Indicator
    Indicator _cur_ind;
    // Read symbol
    Symbol _token;

    // 字符类型判断
    static bool _isNum(char);         // 数字
    static bool _isLetter(char);      // 字母/下划线
    static bool _isLimit(char);       // 界限符
    static bool _isOperator(char);    // 操作符
    static bool _isSpace(char);       // 空格/换行

    void _rollBack();           // 回退
    char _nextChar();           // 读取字符

    void _procDot();           // 点号处理
    void _procNumber();        // 数字处理
    void _procIdentifier();    // 标识符处理
    void _procOperator(char);  // 操作符处理
    void _procLimit();         // 界限符处理

    void _procDiv();           // 除号处理
    void _procString(char start);    // 字符串处理
    void _procAnnotaion(char start); // 注释处理
    void _procError();         // 错误处理

    void _reportError(ErrorType e);    // 错误报告

public:
    
    bool open(const char* file_name);   // 打开文件
    void close();                       // 关闭文件
    int totalChar();
    void typeStatistic(int[]);
    Symbol nextSymbol();                // 下一字符

};


#endif