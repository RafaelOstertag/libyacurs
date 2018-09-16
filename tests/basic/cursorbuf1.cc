// $Id: area.cc 5189 2013-06-11 20:03:17Z rafisol $
//
// Test cursor buffer without displaying

#include <iostream>

#include "cursorbuf.h"

int main() {
    std::string teststr1("abcdefghiklmnopqrstuvwxyz0123456789");

    YACURS::INTERNAL::CursorBuffer cbuff1("");

    for (std::string::size_type i = 0; i < teststr1.length(); i++) {
        cbuff1.end();
        cbuff1.home();

        for (std::string::size_type j = 0; j < i; j++) cbuff1.forward_step();

        int16_t infocurs;
        int16_t stringcurs;
#ifdef DEBUG
        std::cout << cbuff1.string(8, &stringcurs) << std::endl;
#else
        (void)cbuff1.string(8, &stringcurs);
#endif
        cbuff1.info(8, 0, &infocurs);

        if (infocurs != stringcurs) exit(1);

        cbuff1.insert(teststr1[i]);
    }

    if (cbuff1.string() != teststr1) exit(1);

    return 0;
}
