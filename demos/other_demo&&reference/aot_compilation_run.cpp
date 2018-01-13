
#include "Halide.h"


#include "HalideBuffer.h"

#include <stdio.h>

int main(int argc, char **argv) {


    // The ImageParam inputs have become pointers to "halide_buffer_t"
    // structs. This is struct that Halide uses to represent arrays of
    // data.  Unless you're calling the Halide pipeline from pure C
    // code, you don't want to use it
    // directly. Halide::Runtime::Buffer is a simple wrapper around
    // halide_buffer_t that will implicitly convert to a
    // halide_buffer_t *. We will pass Halide::Runtime::Buffer objects
    // in those slots.

    // The Halide::Buffer class we have been using in JIT code is in
    // fact just a shared pointer to the simpler
    // Halide::Runtime::Buffer class. They share the same API.

    // Finally, the return value of "brighter" is an error code. It's
    // zero on success.

    // Let's make a buffer for our input and output.
    Halide::Runtime::Buffer<uint8_t> input(640, 480), output(640, 480);

    // Halide::Runtime::Buffer also has constructors that wrap
    // existing data instead of allocating new memory. Use these if
    // you have your own Image type that you want to use.

    int offset = 5;
    int error = brighter(input, offset, output);

    if (error) {
        printf("Halide returned an error: %d\n", error);
        return -1;
    }

    // check the filter performed as advertised. 
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 640; x++) {
            uint8_t input_val = input(x, y);
            uint8_t output_val = output(x, y);
            uint8_t correct_val = input_val + offset;
            if (output_val != correct_val) {
                printf("output(%d, %d) was %d instead of %d\n",
                       x, y, output_val, correct_val);
                return -1;
            }
        }
    }

    // Everything worked!
    printf("Success!\n");
    return 0;
}
