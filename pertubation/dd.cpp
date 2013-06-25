#include <iostream>
#include <math.h>

int main() {
    double x = 3;
    double y = -4;
    std::cout << atan2(y, x)*180/M_PI << std::endl;
    std::cout << 360 + atan2(y, x)*180/M_PI << std::endl;
    std::cout << sqrt(8*M_PI*M_PI) << std::endl;
}
