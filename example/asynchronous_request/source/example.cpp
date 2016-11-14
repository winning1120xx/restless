/*
 * Example illustrating a asynchronous HTTP request.
 *
 * Usage:
 *    ./distribution/example/asynchronous_request
 */

#include <memory>
#include <cstdio>
#include <cstdlib>
#include <restless>

using namespace std;
using namespace restless;

void print( const shared_ptr< Response >& response )
{
    fprintf( stderr, "*** Response ***\n" );
    fprintf( stderr, "Status Code:    %i\n", response->get_status_code( ) );
    fprintf( stderr, "Status Message: %s\n", response->get_status_message( ).data( ) );
    fprintf( stderr, "HTTP Version:   %.1f\n", response->get_version( ) );
    fprintf( stderr, "HTTP Protocol:  %s\n", response->get_protocol( ).data( ) );
    
    for ( const auto header : response->get_headers( ) )
    {
        fprintf( stderr, "Header '%s' > '%s'\n", header.first.data( ), header.second.data( ) );
    }
    
    fprintf( stderr, "Body:           %.*s...\n", 3, response->get_body( ).data( ) );
}

void response_handler( const shared_ptr< Session > session )
{
    auto response = session->get_response( );
    auto length = response->get_header( "Content-Length", 0 );
    
    session->fetch( length, [ ]( const shared_ptr< Session > session )
    {
        print( session->get_response( ) );
    } );
}

int main( const int, const char** )
{
    auto session = make_shared< Session >( );
    
    auto request = make_shared< Request >( Uri( "https://www.google.com.au/" ) );
    request->set_header( "Accept", "*/*" );
    request->set_header( "Host", "www.google.com.au" );
    request->set_query_parameter( "query", "search term" );
    
    session->send( request, response_handler );
    session->wait( );
    
    return EXIT_SUCCESS;
}