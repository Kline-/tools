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
#include <dlfcn.h>
#include <stdio.h>
#include <iostream>
#include "plugin.h"
using namespace std;

int main( const int argc, const char* argv[] )
{
    void* hello = NULL;
    FILE* pop = NULL;
    const int siz = 1024;
    char tmp[siz] = {'\0'};
    string buf;

    if ( ( pop = popen( "g++ -o hello.so hello.cpp -fpic -ldl -rdynamic -shared 2>&1", "r" ) ) != NULL )
    {
        while ( !feof( pop ) )
            if ( fgets( tmp, siz, pop ) != NULL )
                buf.append( tmp );
        pclose( pop );
    }

    if ( !buf.empty() )
    {
        cout << "Error compiling plugin: " << endl << buf;
        return 1;
    }

    if ( ( hello = dlopen( "./hello.so", RTLD_LAZY ) ) == NULL )
    {
        cout << "Error loading plugin: " << dlerror() << endl;
        return 1;
    }

    NewPlugin* NewHello = (NewPlugin*) dlsym( hello, "New" );
    DeletePlugin* DeleteHello = (DeletePlugin*) dlsym( hello, "Delete" );

    if ( !NewHello || !DeleteHello )
    {
        cout << "Error loading symbols: " << dlerror() << endl;
        return 1;
    }

    Plugin* pHello = NewHello();

    cout << "Plugin: " << pHello->gName() << endl;
    cout << "    ";
    pHello->Run();

    DeleteHello( pHello );
    dlclose( hello );

    return 0;
}
