struct  test
{    
    double pt;
};

int main(){
    // short comment test
    /* short block comment test */
    /*
        long block comment test *\/ escape test
    */
    //id test
    int _a0, b;                                     
    //operator test
    struct test* t = malloc(sizeof(struct test));   
    t->pt = .1e64;
    (*t).pt = 0.2;
    _a0 >>= 2;
    // suffix test
    int a_ = 10uL, b_ = 100uL;  
    // num type test
    int c_ = 0x1e8, d_=070, e_ = 0b0011;
    float f_ = 1.3f, g_ = 1.1l;
    //string test
    printf("print test \"\n");                  
    char str[] = "string \\t\
        test\
    ";
    //error test
    int a$23 = #4;
    float b = 1.2UL, f=1lf;
    int c = 0x1.4, d=078, e=0b13;
    char str2[] = "no end test \"

    return 0;
}
