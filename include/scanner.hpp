#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#include "basic.h"
#include "notation.h"
#include "indicator.hpp"
#include "symbol.hpp"

class Scanner
{
    const static int __buffer_length = 64;  // 缓冲区长度
    const static int __buffer_mid = __buffer_length / 2;
private:
    enum class ErrorType{   //*错误类型
        INVALID_WORD,       // 非法标识符
        EXPECTED_END,       // 需要字符串/注释结尾
        INVALID_FLOAT,      // 浮点无效
        INVALID_HEX,        // 十六进制无效
        INVALID_OCTAL,      // 八进制无效
        INVALID_BINARY,     // 二进制无效
        CHAR_OVERFLOW,      // 字符常量超限
    };
    enum class PrefixType{  //*前缀类型
        DECIMAL,            // 十进制
        HEX,                // 十六进制
        BINARY,             // 二进制
        OCTAL,              // 八进制
        _NAN,               // 非数字
    };
    enum class NumType{     //*数字类型
        INT,                // 整形
        FLOAT,              // 浮点
    };
    // File stream
    std::string _file_name;             // 文件名
    std::ifstream _ifs, _ifs_line;      // 文件流(字符文件流)(行文件流)
    bool _row_flag = false;
    // String buffer
    char _line_buffer[256] = {0};       // 行缓冲区
    char _file_buffer[__buffer_length] = {0};   // 字符缓冲区
    int _ptr_buffer = -1;               // 字符缓冲区指针
    // Counter
    int _type_counter[9] = {0};         // 记号类型计数器
    int _total_char = 0;                // 字符计数器
    int _roll_back_times = 0;           // 指针回退计数器
    // Current Indicator
    Indicator _cur_ind;                 // 行列指示器
    // Read symbol
    Symbol _token;                      // 记号缓冲

    // 字符类型判断
    static bool _isNum(char);           // 数字
    static bool _isLetter(char);        // 字母/下划线
    static bool _isLimit(char);         // 界限符
    static bool _isOperator(char);      // 操作符
    static bool _isSpace(char);         // 空格/换行
    static bool _isHex(char);           // 十六进制 
    static bool _isSuffix(char);        // 后缀

    void _rollBack();                   // 字符指针回退
    char _nextChar();                   // 字符读取

    void _procDot();                    // 点号处理 (->数字/操作符)
    
    void _procNumber();                 // 数字处理
    PrefixType _procPrefix();           // 前缀处理
    void _procSuffix(NumType);          // 后缀处理
    
    void _procIdentifier();             // 标识符处理
    void _procLimit();                  // 界限符处理

    void _procDiv();                    // 除号处理  (->操作符/注释)
    void _procOperator(char);           // 操作符处理
    void _procString(char start);       // 字符串处理
    void _procAnnotaion(char start);    // 注释处理

    void _procError(ErrorType);         // 错误处理
    void _reportError(ErrorType e);     // 错误报告

public:
    
    bool open(const char* file_name);   // 打开文件
    void close();                       // 关闭文件
    int totalChar();                    // 字符总数
    int totalRow();                     // 行总数
    void typeStatistic(int[]);          // 记号计数
    Symbol nextSymbol();                // 下一记号
};


#endif