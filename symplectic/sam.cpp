#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

int main() {
    printf("%.50f\n", 0.0239758347589743917);
    printf("%04d\n", 4);
    std::string ss = "hej";
    std::stringstream aa;
    aa << 4;
    //std::cout << ss+"hello"+aa << std::endl;
    char str[13];
    sprintf(str, "out%.4d.ppm", 4);
    std::cout << str << std::endl;
    //printf("%s", ss.c_str());
}
