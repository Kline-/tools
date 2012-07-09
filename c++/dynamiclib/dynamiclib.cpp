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
