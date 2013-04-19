// $Id$
//
// Tests margin class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#include "margin.h"

int
main() {
    YACURS::Margin m1(2, 3, 5, 7);
    YACURS::Margin m2(11, 13, 17, 19);

    YACURS::Margin r = m1 + m2;

    if (r.top() != 11 ||
        r.left() != 13 ||
        r.bottom() != 17 ||
        r.right() != 19)
        return 1;

    r = m1 - m2;

    if (r.top() != 2 ||
        r.left() != 3 ||
        r.bottom() != 5 ||
        r.right() != 7)
        return 1;

    if (r != m1 - m2)
        return 1;

    if (!(r == m1 - m2) )
        return 1;

    m1 = m2;

    if (m1 != m2)
        return 1;

    if (!(m1 == m2) )
        return 1;

    return 0;
}
