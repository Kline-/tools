#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

int main( int argc, char *argv[] )
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
