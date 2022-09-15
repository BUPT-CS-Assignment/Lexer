#include "../include/scanner.hpp"

int main(int argc,char** argv){
    // output file
    std::ofstream ofs("out.list");
    int row = 0;

    // input file
    Scanner sc;
    std::string path = (argc >= 2 ? std::string(argv[1]) : "/scripts/input.c");
    sc.open(path.c_str());

    // symbols
    ofs << ":" << path << std::endl;
    ofs << "\n[lexical analysis]" << std::endl;
    for(;;){
        Symbol s = sc.nextSymbol();
        if(s.attr() == Symbol::Type::END)
            break;
        if(s.indicator.row() + 1 != row){
            row = s.indicator.row() + 1;
            ofs << row;
        }
        ofs<< "\t| " << s.toString() << std::endl;
    }

    // statistic
    int counter[9];
    sc.typeStatistic(counter);
    ofs << "\n[statistic]\nrow: " << row << "\nchar: " << sc.totalChar() << std::endl; 
    
    ofs << "\n[types]" << std::endl;
    for(int i = 1; i < 8; i++){
        ofs << __type_str[i] << ": " << counter[i] << std::endl;
    }    

    // cout
    std::cout << BOLDWHITE << path << ": " BOLDGREEN "statistic" BOLDWHITE ":" RESET << std::endl;
    std::cout << "   [row]   " << BOLDWHITE << row << RESET << std::endl;
    std::cout << "   [char]  " << BOLDWHITE << sc.totalChar() << RESET << std::endl;
    std::cout << BOLDWHITE << path <<": " BOLDGREEN "types" BOLDWHITE ":" RESET << std::endl;
    std::cout << "   [" << __type_str[1] << "]   " << (counter[1] == 0 ? BOLDGREEN : BOLDRED) << counter[1] << RESET << std::endl;
    for(int i = 2; i < 8; i++){
        std::cout << "   [" << __type_str[i] << "]   " BOLDWHITE << counter[i] << RESET << std::endl;
    }
    ofs.close();
    return 0;
}