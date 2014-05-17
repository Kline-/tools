/**
 * Copyright (c) 2012-2014 Matthew Goff <matt@goff.cc>
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
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

int main( const int argc, const char *argv[] )
{
    char valid[] = "<policy-file-request/>\0";
    string request, line;
    ifstream policyfile( argv[1] );

    cin >> request;

    if ( !strcmp( request.c_str(), valid ) )
        while ( policyfile.is_open() && policyfile.good() && getline( policyfile, line) )
            cout << line << endl;
    else
        cout << "Invalid request: " << request << endl << "Expected: " << valid << endl;

    return 0;
}
