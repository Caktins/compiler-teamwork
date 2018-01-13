
#include "Halide.h"
#include <stdio.h>

using namespace Halide;

int main(int argc, char **argv) {

    Func gradient("gradient");
    Var x("x"), y("y");
    gradient(x, y) = x + y;

    Buffer<int> output = gradient.realize(8, 8);

    gradient.compile_to_lowered_stmt("gradient.html", {}, HTML);

    printf("Success!\n");
    return 0;
}
