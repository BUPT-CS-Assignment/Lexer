#ifndef __INDICATOR_HPP__
#define __INDICATOR_HPP__
#include <iostream>

class Indicator
{
private:
    int _row = 1, _col = 1, _last = 0;
public:
    Indicator(int r = 1, int c = 1){
        _row = r;
        _col = c;
        _last = 1;
    }
    void clear()
    {
        _row = _col = 1;
    }
    // Getter & Setter
    void setRow(int r){
        _row = r;
    }
    void setCol(int c){
        _col = c;
    }
    int row(){
        return _row;
    }
    int col(){
        return _col; 
    }

    // Next 
    void nextRow()
    {
        ++_row;
        _last = _col;
        _col = 1;
    }
    void nextCol()
    {
        ++_col;
    }

    void lastRow(){
        --_row;
        _col = _last;
    }

    void lastCol(){
        if(_col > 1)
            --_col;
        else
            lastRow();
    }
};

#endif