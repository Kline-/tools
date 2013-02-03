#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <mysql/mysql.h>
using namespace std;

#define MYSQUID_SERVER "localhost"
#define MYSQUID_PORT   0
#define MYSQUID_DB     "mysquid"
#define MYSQUID_USER   "matt"
#define MYSQUID_PASS   "matt"

#define MYSQUID_OK     "OK"
#define MYSQUID_ERR    "ERR"
#define MYSQUID_USAGE  "usage: mysquid <query>"

int main( const int argc, const char* argv[] )
{
    MYSQL sql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    my_bool reconnect = true;
    string argv_, input, query, token;
    size_t start, end;
    stringstream split;

    if ( argc != 2 )
    {
        cout << MYSQUID_USAGE << endl;
        return 0;
    }

    mysql_init( &sql);
    mysql_options( &sql, MYSQL_OPT_RECONNECT, &reconnect );

    if ( !mysql_real_connect( &sql, MYSQUID_SERVER, MYSQUID_USER, MYSQUID_PASS, MYSQUID_DB, MYSQUID_PORT, NULL, 0 ) )
    {
        cout << MYSQUID_ERR << mysql_error( &sql ) << endl;
        mysql_close( &sql );

        return 0;
    }

    argv_ = argv[1];
    argv[1] = '\0';

    while ( getline( cin, input ) )
    {
        query = argv_;
        split.str( input );

        while ( getline( split, token, ' ' ) )
        {
            start = query.find( "##", 0, 2 );
            end = query.find( "##", start + 2, 2 );

            if ( start != string::npos && end != string::npos )
                query.replace( start, end - start + 2, token );
        }

        split.clear();

        if ( mysql_query( &sql, query.c_str() ) )
            cout << MYSQUID_ERR << mysql_error( &sql ) << endl;
        else
        {
            if ( ( res = mysql_store_result( &sql ) ) == NULL )
            {
                cout << MYSQUID_ERR << mysql_error( &sql ) << endl;
                mysql_free_result( res );
                continue;
            }

            if ( ( row = mysql_fetch_row( res ) ) == NULL )
            {
                cout << MYSQUID_ERR << mysql_error( &sql ) << endl;
                mysql_free_result( res );
                continue;
            }

            if ( mysql_num_rows( res ) == 0 )
                cout << MYSQUID_ERR << endl;
            else
                cout << MYSQUID_OK << endl;

            mysql_free_result( res );
        }
    }

    mysql_close( &sql );

    return 0;
}
