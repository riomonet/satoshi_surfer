#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>


int main(int argc, char *argv[])
{


	struct addrinfo				hints, *res;
	int					sd;			   /* socket descriptor*/
	int					status;			   /* return status    */
	char					buf[100];

        /* INITIALZE ADDRINFO STRUCT WITH REQUIRED VALUES */
	memset(&hints, 0, sizeof(hints));				   /* reset all fields in the addrinfo struct */
	hints.ai_family = AF_UNSPEC;					   /* this allows for ip4 or ip6 */
	hints.ai_socktype = SOCK_STREAM;				   /* TCP stream connection as opposed to udp datagram */
//	hints.ai_flags = AI_PASSIVE;					   /* fill hints.address automaticall usually with local host address */

	
	status = getaddrinfo("192.168.6.103", "11000", &hints, &res);
	sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);  /* get socket */
	//status = bind(sd, res->ai_addr, res->ai_addrlen);		  /* bind port to socket */
	printf("getaddinfo status  = %d\n", status);
	status = connect(sd, res->ai_addr, res->ai_addrlen);
	printf("connect status = %d\n", status);
	

        /* jumping througth hoops to print, how do we print the port number ? */
        struct sockaddr_in *x = (struct sockaddr_in *) res->ai_addr;
	void *addr = &(x->sin_addr);
	inet_ntop(res->ai_family, addr, buf, sizeof buf );
	printf("socket = %d   ip = %s\n",sd, buf);

	
	return 0;
}


