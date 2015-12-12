/* C-style comments can contain
multiple lines */
/* or just one */

// C++-style comments can comment one line

// or, they can
// be strung together

// /* Also comment */
/* // Test */

// "Quoted comment"

// comment line 1\
                comment line 2\
                comment line 3\
/****** Comment
/**** / comment continue*/// comment


#include <iostream>

int main()
{
    std::cout << '//' << std::endl;
    std::cout << "/* THIS IS NOT A COMMENT */" << std::endl;
    std::cout << "// TOO */" << std::endl;
    std::cout << "// THIS IS NOT A COMMENT */" << std::endl;

    // The below code won't be run
    // return 1;

    // The below code will be run
    return 0;
}