// $Id$
//
// Test ColorParser

#include "colorparser.h"

int main() {
    YACURS::ColorParser cp;

    cp();
    cp("trash");

    cp("DeF:wy0;MBT:cy0;MBX:yc0;IWN:yw3;IWF:yg3;IWH:yy7;BTN:wy0;BTF:yg3;LBX:wy0;LBH:yg3;CBG:yc0;CBT:cy0");
    return 0;
}
