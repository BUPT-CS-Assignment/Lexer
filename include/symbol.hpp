#ifndef __SYMBOL_HPP__
#define __SYMBOL_HPP__
#include "indicator.hpp"
#include "basic.h"

const static std::string __type_str[] ={
    "End","Err",
    "Key","Tag","Num","Opt","Lim",
    "Str","Com"
};

class Symbol
{
    const static int __buffer_length = 256;
public:
    enum class Type
    {
        END,            // 结束符
        ERROR,          // 错误符
        KEYWORD,        // 关键字
        IDENTIFIER,     // 标识符
        NUMBER,         // 数字
        OPERATOR,       // 操作符
        LIMIT,          // 界限符
        STRING,         // 字符
        COMMENT,        // 注释
    };
private:
    int _ptr_content = 0;                   // 内容指针
    char _content[__buffer_length] = {0};   // 字符内容
    Type _attr = Type::END;                 // 字符记号
    
public:
    Indicator indicator;    // 指示器
    // 清空
    void clear(){
        memset(&_content,0,__buffer_length);
        _ptr_content = 0;
        indicator.setRow(0);
        indicator.setCol(0);
        _attr = Type::END;
    }
    
    // 添加字符
    void append(char ch){
        if(_ptr_content + 1 >= __buffer_length)
        {
            _ptr_content = 0;
            _attr = Type::ERROR;
        }
        _content[_ptr_content ++] = ch;
    }

    // 字符回退
    char rollBack(){
        if(_ptr_content <= 0)
            return EOF;
        char ch = _content[_ptr_content - 1];
        _content[--_ptr_content] = 0;
        return ch;
    }
    std::string content(){
        return std::string(_content);
    }

    // 获取属性
    Type attr(){
        return _attr;
    }

    // 设置属性
    void setAttr(Type t){
        _attr = t;
    }

    // 达到末尾
    Symbol& end(){
        _attr = Type::END;
        return *this;
    }

    // 错误
    Symbol& error(){
        _attr = Type::ERROR;
        return *this;
    }

    // 字符串化
    std::string toString(){
        std::string temp = _attr == Type::ERROR ? "!" : " ";
        return __type_str[(int)_attr] + "\t" + temp + "\t " + std::string(_content);
    }

    // 重载 []
    char operator[](int pos){
        if(pos >= 0 && pos < _ptr_content)
            return _content[pos];
        if(pos < 0 && (_ptr_content + pos) > 0)
            return _content[_ptr_content];
        return EOF;
    }
};

#endif