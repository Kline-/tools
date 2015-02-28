# redditcl.tcl
package provide redditcl 1.0

package require Tcl 8.6
package require base64
package require json
package require http
package require tls

namespace eval ::redditcl:: {
    variable access_token ""
    variable auth ""
    variable baseurl "https://oauth.reddit.com"
    variable header ""
    variable useragent "redditcl/_Kline"

    ::http::config -useragent $::redditcl::useragent
    ::http::register https 443 [list ::tls::socket -tls1 1]
}

proc ::redditcl::get:token {username password clientid clientsecret} {
    # Authenticate and store an OAuth token
    set ::redditcl::auth "Basic [::base64::encode $clientid:$clientsecret]"
    set ::redditcl::header [list Authorization $::redditcl::auth]
    set query [::http::formatQuery grant_type password username $username password $password ]
    set data [::http::geturl "https://ssl.reddit.com/api/v1/access_token" -query $query -headers $::redditcl::header -method POST]
    set ::redditcl::access_token [::json::json2dict [::http::data $data]]
    # Prep the headers for subsequent API calls
    set ::redditcl::auth "bearer [dict get $::redditcl::access_token access_token]"
    set ::redditcl::header [list Authorization $::redditcl::auth]
}

proc ::redditcl::get {api} {
    set data [::http::geturl "$::redditcl::baseurl$api" -headers $::redditcl::header -method GET]

    return [::json::json2dict [::http::data $data]]
}
