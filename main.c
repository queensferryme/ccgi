/*
 * Main Module
 * 
 * this modules is the major part of CGI,
 * and it maps requests to coresponding view function.
 */

#include <fcgi_stdio.h>

#include "routers.h"

int main() {
	while(FCGI_Accept() >= 0) {
		char *route = getenv("DOCUMENT_URI"),
			 *query_string = getenv("QUERY_STRING"),
			 *method = getenv("REQUEST_METHOD");
		if(equal(route, "/user") && equal(method, "GET"))
			user(query_string);
		else if(equal(route, "/admin") && equal(method, "GET"))
			admin();
		else if(equal(route, "/update") && equal(method, "POST"))
			update(query_string);
		else if(equal(route, "/delete") && equal(method, "GET"))
			delete(query_string);
		else sysinfo();
	}
	return 0;
}

