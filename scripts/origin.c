struct  test
{    
    double pt;
};


int main(){
    struct test* t = malloc(sizeof(struct test));
    struct test t2;
    t -> pt = .1e64;
    t2.pt = 1.2;
    int _a0 = 10;
    _a0 >>= 2;
    printf("print test \"\n");     // short comment test
    char str[] = "string\
        test\
    ";

    /* 
        long comment test *\/ *end-test
    */

    #a$23    // invalid tag test
    char str2[] = "no block \"
    test
    return 0;
}
