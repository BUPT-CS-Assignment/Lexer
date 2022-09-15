#include "../include/scanner.hpp"

bool Scanner::open(const char* file_name)
{
    _ifs.open(file_name, std::ios::in);
    if(!_ifs.is_open())
        EXIT_ERROR("no such file '" << file_name << "'");
    _total_char = 0;
    _ifs_line.open(file_name, std::ios::in);
    _ifs_line.getline(_line_buffer,__buffer_length);
    _ifs.read(_file_buffer, __buffer_mid);
    _file_name = std::string(file_name);
    return true;
}

void Scanner::close()
{
    memset(_file_buffer, 0, sizeof(_file_buffer));
    _ptr_buffer = -1;
    _ifs.close();
}

void Scanner::_rollBack()
{
    if(_cur_ind.col()> 0)
        _cur_ind.setCol(_cur_ind.col() - 1);
    if(_ptr_buffer == 0)
        _ptr_buffer = __buffer_length - 1;
    else
        --_ptr_buffer;
}


char Scanner::_nextChar()
{
    if(_ptr_buffer == __buffer_mid - 1 && !_ifs.eof())
    {
        memset(&_file_buffer[__buffer_mid], EOF, __buffer_mid);
        _ifs.read(&_file_buffer[__buffer_mid], __buffer_mid);
    }
    else if(_ptr_buffer == __buffer_length - 1 && !_ifs.eof())
    {
        memset(&_file_buffer[0], EOF, __buffer_mid);
        _ifs.read(_file_buffer, __buffer_mid);
    }
    char last = _file_buffer[(_ptr_buffer++) % __buffer_length];
    if(last != EOF)
        _ptr_buffer %= __buffer_length;
    char ch = _file_buffer[_ptr_buffer];
    if(ch != ' ' && ch != '\n' && ch != '\t')
        ++_total_char;
    if(last == '\n'){
        _cur_ind.nextRow();
        _ifs_line.getline(_line_buffer,__buffer_length);
    } 
    else
        _cur_ind.nextCol();
    
    return ch;
}

Symbol Scanner::nextSymbol()
{
    _token.clear();
    
    char ch;
    do
        ch = _nextChar();
    while(ch == ' ' || ch == '\n');
    _token.indicator = _cur_ind;
    if(ch == EOF)
        return _token.end();
    _token.append(ch);
    if(ch == '.')
        _procDot();
    else if(_isNum(ch))
        _procNumber();
    else if(_isLetter(ch))
        _procIdentifier();
    else if(ch == '/')
        _procDiv();
    else if(ch == '\"' || ch == '\'')
        _procString(ch);
    else if(_isLimit(ch))
        _procLimit();
    else if(_isOperator(ch))
        _procOperator(ch);
    else
        _procError();

    ++_type_counter[(int)_token.attr()];
    if(_token.attr() == Symbol::Type::COMMENT)
        return nextSymbol();
    return _token;
}


void Scanner::_procDot()
{
    _token.setAttr(Symbol::Type::OPERATOR);
    char ch = _nextChar();
    if(_isLetter(ch))
        return _rollBack();
    _token.append(ch);
    if(_isNum(ch))
    {
        _rollBack();
        _token.rollBack();
        return _procNumber();
    }
    else if(ch == '.')
    {

        ch = _nextChar();
        if(ch == '.')
            return _token.append(ch);
    }
    return _procError();
}


void Scanner::_procString(char start)
{
    _token.setAttr( Symbol::Type::STRING);
    bool escape = false;
    for(;;)
    {
        char ch = _nextChar();
        if(ch == ' ' || ch == '\n' || ch == '\t')
            ++_total_char;
        _token.append(ch);
            
        if(escape){
            escape = false;
            if(ch == '\n'){
                _token.rollBack();
                _token.rollBack();
            }
        }
        else if(!escape && ch == '\\')
            escape = true;
        else if(ch == EOF || (!escape && ch == '\n'))
        {
            _token.rollBack();
            _token.indicator.setCol(_cur_ind.col());
            ++_type_counter[(int)Symbol::Type::ERROR];
            return _reportError(ErrorType::EXPECTED_END);
        }
        else if(!escape && ch == start)
            return;
    }
}

void Scanner::_procDiv()
{
    _token.setAttr(Symbol::Type::OPERATOR);
    char ch = _nextChar();
    if(ch == '*' || ch == '/')
        return _procAnnotaion(ch);
    return _rollBack();
}

void Scanner::_procAnnotaion(char type)
{
    _token.setAttr(Symbol::Type::COMMENT);
    _token.append(type);
    if(type == '/')
    {
        for(char ch;;)
        {
            ch = _nextChar();
            if(ch == '\n' || ch == EOF)
                return;
        }
    }

    bool escape = false, end = false;
    for(char ch;;)
    {
        ch = _nextChar();
        if(ch == '*')
            end = true;
        else if(escape)
            escape = end = false;
        else if(!escape && ch == '\\')
            escape = true;
        else if(!escape && end)
        {
            if(ch == '/')   return;
            end = false;
        }
        else if(ch == EOF)
        {
            _token.indicator.setCol(_cur_ind.col());
            return _reportError(ErrorType::EXPECTED_END);
        }
    }
}

void Scanner::_procIdentifier()
{
    _token.setAttr(Symbol::Type::IDENTIFIER);
    for(char ch;;)
    {
        ch = _nextChar();
        if(ch == EOF)
            break;
        if(_isLetter(ch) || _isNum(ch))
            _token.append(ch);
        else{
            _rollBack();
            break;
        } 
    }
    if(__is_keyword(_token.content().c_str()))
        _token.setAttr(Symbol::Type::KEYWORD);
    
}



void Scanner::_procNumber()
{
    _token.setAttr(Symbol::Type::NUMBER);
    int part = 1, part_int = 1, part_float = 2, part_e = 3, part_end = 4;
    for(char ch;;)
    {
        ch = _nextChar();
        if(ch == EOF) return;
        if(part == part_int)
        {
            if(ch != '.' && ch != 'E' && ch != 'e' && !_isNum(ch))
                return _rollBack();
            part = (ch == '.' ? part_float : ((ch == 'e' || ch == 'E') ? part_e : part_int));
        }
        else if(part == part_float)
        {
            if(ch != 'E' && ch != 'e' && !_isNum(ch))
                return _rollBack();
            part = (ch == 'E' || ch == 'e' ? part_e : part_float);
        }
        else if(part_e)
        {
            if(ch != '+' && ch != '-' && !_isNum(ch))
                return _rollBack();
            part = part_end;
        }
        else if(part_end)
            if(!_isNum(ch))
                return _rollBack();
        _token.append(ch);
    }
}

void Scanner::_procLimit()
{
    _token.setAttr(Symbol::Type::LIMIT);
    return;
}

void Scanner::_procOperator(char start)
{
    _token.setAttr(Symbol::Type::OPERATOR);
    bool append_flag = true;
    char ch = _nextChar();
    if(ch == EOF)   return;
    if(start == '-')
        append_flag = (ch == '-' || ch == '=' || ch == '>');
    else if(start == '*' || start == '/' || start == '!' || start == '%' || start == '^')
        append_flag = (ch == '=');
    else if(start == '>' || start == '<' || start == '=' || start == '|' || start == '&' || start == '+')
        append_flag = (ch == '=' || ch == start);

    if((start == '<' && ch == '<') || (start == '>' && ch == '>'))
    {
        _token.append(ch);
        ch = _nextChar();
        append_flag = (ch == '=');
    }

    return append_flag ? _token.append(ch) : _rollBack();
}

void Scanner::_procError()
{
    _token.setAttr(Symbol::Type::ERROR) ;
    _token.indicator.setCol(_cur_ind.col());
    _reportError(ErrorType::INVALID_WORD);
}

void Scanner::_reportError(ErrorType e)
{
    std::string msg = "";
    if(e == ErrorType::INVALID_WORD)
        msg = "invalid character '" BOLDWHITE + _token.content() + RESET "'";
    else if(e == ErrorType::EXPECTED_END){
        std::string end = std::string(1,_token[0]);
        msg = "expected '" BOLDWHITE + (end == "*"?"*/":end) + RESET "'";
    }
        

    printf(BOLDWHITE "%s:%d:%d: " RED "error: " RESET "%s\n", _file_name.c_str(), _token.indicator.row() + 1, _token.indicator.col() + 1, msg.c_str());
    printf("%5d |%s\n",_token.indicator.row() + 1,_line_buffer);
    __print_space(6);
    printf("|");
    __print_space(_token.indicator.col());
    printf(RED "^\n" RESET);
}

int Scanner::totalChar(){
    return _total_char;
}

void Scanner::typeStatistic(int res[]){
    if(res == nullptr) 
        return;
    memcpy(res,&_type_counter,sizeof(_type_counter));
}

bool Scanner::_isLetter(char ch)
{
    return ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool Scanner::_isNum(char ch)
{
    return ch >= '0' && ch <= '9';
}

bool Scanner::_isLimit(char ch)
{
    return ch == '{' || ch == '}' || ch == '(' || ch == ')'
        || ch == '[' || ch == ']' || ch == ',' || ch == ';';
}

bool Scanner::_isOperator(char ch)
{
    return ch == '<' || ch == '>' || ch == '=' || ch == '+' || ch == '-' || ch == '*'
        || ch == '/' || ch == '%' || ch == '&' || ch == '|' || ch == '!' || ch == '^';
}

bool Scanner::_isSpace(char ch)
{
    return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
}