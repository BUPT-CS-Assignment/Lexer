#ifndef __INDICATOR_HPP__
#define __INDICATOR_HPP__
#include <iostream>

class Indicator
{
private:
    int _row = 0, _col = 0;
public:
    Indicator(int r = 0, int c = 0){
        _row = r;
        _col = c;
    }
    void clear()
    {
        _row = _col = 0;
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
        _col = 0;
    }
    void nextCol()
    {
        ++_col;
    }
};

#endif