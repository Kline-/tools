/**
 * Copyright (C) 2012-2014 Matthew Goff <matt@goff.cc>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 **/
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
using namespace std;

int wtab[] =
{
 /*  0   1   2   3   4   5   6   7 */
     0, 10, 10, 10, 10, 10, 10, 10,
 /*  8   9  10  11  12  13  14  15 */
    10, 10, 10, 10, 10, 10, 10, 10,
 /* 16  17  18  19  20  21  22  23 */
    10, 10, 10, 10, 10, 10, 10, 10,
 /* 24  25  26  27  28  29  30  31 */
    10, 10, 10, 10, 10, 10, 10, 10,
 /* 32  33  34  35  36  37  38  39 */
    10, 10, 10, 11, 11, 11, 12, 12,
 /* 40  41  42  43  44  45  46  47 */
    12, 13, 13, 13, 13, 14, 14, 14,
 /* 48  49  50  51  52  53  54  55 */
    15, 15, 15, 16, 16, 16, 16, 17,
 /* 56  57  58  59  60  61  62  63 */
    17, 17, 18, 18, 18, 19, 19, 19,
 /* 64                             */
    19
};

int main( const int argc, const char* argv[] )
{
    unsigned long long numbit, value, i, x;

    if ( argc != 3 )
    {
        cout << "usage: " << argv[0] << " <# bits to test> <value to test>" << endl;
        return 0;
    }

    numbit = atoi( argv[1] );
    value = atol( argv[2] );

    if ( numbit < 1 || numbit > 64 )
    {
        cout << "That would be pointless, sorry." << endl;
        return 0;
    }

    i = 1;
    x = 1;

    cout << "[";
    cout.width( wtab[numbit] + 3 );
    cout << right << "Bit Value] [";
    cout.width(2);
    cout << "##";
    cout.width( wtab[numbit] + 3 );
    cout << left << "] [Remainder";
    cout << "]" << endl;

    for ( i; i <= numbit; i++ )
    {
        cout << "[";
        cout.width( wtab[numbit] );
        cout << right << x << "] [";
        if ( i < 10 )
         cout << 0 << i;
        else
         cout << i;
        cout << "] [";
        if ( value & x )
        {
            cout.width( wtab[numbit] );
            cout << left << value << "]" << endl;
        }
        else
        {
            cout.width( wtab[numbit] + 1 );
            cout << right << " ]" << endl;
        }
        value &= ~x;
        x *= 2;
    }

    return 0;
}
