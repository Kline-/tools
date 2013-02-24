/**
 * Copyright (c) 2012-2013 Matt Goff <matt@goff.cc>
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
#include <pthread.h>
#include <sstream>
#include <stdlib.h>
#include <mysql/mysql.h>
using namespace std;

#define MYSQUID_SERVER "localhost"
#define MYSQUID_PORT   0
#define MYSQUID_DB     "mysquid"
#define MYSQUID_USER   "matt"
#define MYSQUID_PASS   "matt"

#define MYSQUID_OK     " OK"
#define MYSQUID_ERR    " ERR"
#define MYSQUID_USAGE  "usage: mysquid <query>"

struct ThreadData
{
        MYSQL sql;
        string query;
        string input;
};

const void spawn_thread( ThreadData* data );
void* process_request( void* input );

int main( const int argc, const char* argv[] )
{
    MYSQL sql;
    my_bool reconnect = true;
    ThreadData* data;
    string argv_, input;

    if ( argc != 2 )
    {
        cout << MYSQUID_USAGE << endl;
        return 0;
    }

    mysql_init( &sql );
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
        data = new ThreadData;
        data->sql = sql;
        data->query = argv_ ;
        data->input = input;
        spawn_thread( data );
    }

    mysql_close( &sql );

    return 0;
}

const void spawn_thread( ThreadData* data )
{
    pthread_t res_thread;
    pthread_attr_t res_attr;

    pthread_attr_init( &res_attr );
    pthread_attr_setdetachstate( &res_attr, PTHREAD_CREATE_DETACHED );
    pthread_create( &res_thread, &res_attr, &process_request, data );
    pthread_attr_destroy( &res_attr );

    return;
}

void* process_request( void* input )
{
    ThreadData* data = static_cast<ThreadData*>( input );
    MYSQL_RES *res;
    MYSQL_ROW row;
    string channel, token;
    size_t start, end;
    stringstream split;

    channel = data->input.substr( 0, data->input.find_first_of( ' ' ) );
    data->input.erase( 0, data->input.find_first_of( ' ' )+1 );
    split.str( data->input );

    while ( getline( split, token, ' ' ) )
    {
        start = data->query.find( "##", 0, 2 );
        end = data->query.find( "##", start + 2, 2 );

        if ( start != string::npos && end != string::npos )
            data->query.replace( start, end - start + 2, token );
    }

    if ( mysql_query( &data->sql, data->query.c_str() ) )
        cout << channel << MYSQUID_ERR << mysql_error( &data->sql ) << endl;
    else if ( ( res = mysql_store_result( &data->sql ) ) == NULL )
        cout << channel << MYSQUID_ERR << mysql_error( &data->sql ) << endl;
    else if ( ( row = mysql_fetch_row( res ) ) == NULL )
        cout << channel << MYSQUID_ERR << mysql_error( &data->sql ) << endl;
    else if ( mysql_num_rows( res ) == 0 )
        cout << channel << MYSQUID_ERR << endl;
    else
        cout << channel << MYSQUID_OK << endl;

    mysql_free_result( res );
    delete data;

    pthread_exit( reinterpret_cast<void*>( EXIT_SUCCESS ) );
}
