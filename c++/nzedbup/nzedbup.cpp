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
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

#define NZEDBUP_BINARIES_CMD   "php update_binaries.php"
#define NZEDBUP_BINARIES_SLEEP 60

#define NZEDBUP_RELEASES_CMD   "php update_releases.php 1 true"
#define NZEDBUP_RELEASES_SLEEP 120

struct ThreadData
{
    string args;
    int sleep;
};

const void spawn_thread( ThreadData* data );
void* process_request( void* input );

int main( const int argc, char* argv[] )
{
    ThreadData* data;

    data = new ThreadData;
    data->args = NZEDBUP_BINARIES_CMD;
    data->sleep = NZEDBUP_BINARIES_SLEEP;
    spawn_thread( data );

    data = new ThreadData;
    data->args = NZEDBUP_RELEASES_CMD;
    data->sleep = NZEDBUP_RELEASES_SLEEP;
    spawn_thread( data );

    for ( ; ; )
    {
        ::sleep( 10 );
    }

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

    system( data->args.c_str() );
    ::sleep( data->sleep );
    spawn_thread( data );

    return 0;
}
