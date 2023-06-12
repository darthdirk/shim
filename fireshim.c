
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

//set the BUFFER len so it can handle reasonable request
#define BUFFER_LEN 4096

int filterhost(char* host){
	if (strstr(host, "akamai")) return 1;
	if (strstr(host, "amazon")) return 1;
	if (strstr(host, "cloudfront")) return 1;
	if (strstr(host, "googleusercontent")) return 1;
	if (strstr(host, "doubleclick")) return 1;
	if (strstr(host, "doubleverify")) return 1;
	if (strstr(host, "outbrain")) return 1;
	if (strstr(host, "gigya")) return 1;
	if (strstr(host, "1e100")) return 1;
	if (strstr(host, "adnexus")) return 1;
	if (strstr(host, "facebook")) return 1;
	if (strstr(host, "2o7.net")) return 1;
	if (strstr(host, "hwcdn.net")) return 1;
	if (strstr(host, "fbcdn.net")) return 1;
	
	
	return 0;
	{
		
	}}



//intercept the connect function 
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) 
{
//dlsym to get a pointer to the original_connect func to call it
	int (*original_connect)(int,const struct sockaddr*, socklen_t) = NULL;
	original_connect = dlsym(RTLD_NEXT, "connect");

	struct sockaddr_in *myaddr = (struct sockaddr_in*)addr;
	struct sockaddr_in6 *myaddr6 = (struct sockaddr_in6*)addr;

//gather info on connections ipv4 ipv6 
	char *fam = "OTHER";
	if (addr->sa_family == AF_INET)
		fam = "AF_INET";
	if (addr->sa_family == AF_INET6)
		fam = "AF_INET6";
	
//use ntop to get the printable version of the binary address 
	char address_str[BUFFER_LEN] = {0};
	if (addr->sa_family == AF_INET){
		inet_ntop(addr->sa_family, &(myaddr->sin_addr.s_addr), 
		address_str, BUFFER_LEN);
	} else if (addr->sa_family == AF_INET6){
		inet_ntop(addr->sa_family, &(myaddr6->sin6_addr.s6_addr),
		address_str, BUFFER_LEN);
}
//attempt to get the host name
	char host[BUFFER_LEN] = {0};
	char server[BUFFER_LEN] = {0};
	getnameinfo(addr,addrlen,
				host, BUFFER_LEN,
				server, BUFFER_LEN, 0);

//filter all local traffic out
	if (strstr(host, "dirk")) {
		return original_connect(sockfd, addr, addrlen);
	}
//print the connections that were refused
//	printf("%s (%s) %s - ",host, fam, address_str);
	
	//if I do not want the host then refuse the connection
	if (filterhost(host)){
		//printf("REFUSED!\n");
		errno = ECONNREFUSED;
		return -1;
	}
//just print the connections that made it past my filter
	printf("%s (%s) %s - ",host, fam, address_str);
	
	printf("OK\n");	


     
    printf("%s (%s) %s\n",host, fam, address_str);


	return original_connect(sockfd, addr, addrlen);



	//errno = ECONNREFUSED;
	//return -1;
}

