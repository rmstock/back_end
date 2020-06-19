#include "crow.h"
#include "json.hpp"
#include <thread>

#define BUFSIZE 65536
#define UPD_PORT 8080
#define HTTP_PORT 18080

void http_server()
{
	crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });
	
	CROW_ROUTE(app, "/messages")
	([](){
		nlohmann::json array = R"([
				{ "new_string": "Blah Stuff", "id" : 12, "type" : 2},
				{ "new_string": "Other Thing", "id" : 24, "type" : 5},
				{ "new_string": "Last Thing", "id" : 8, "type" : 2}
			])"_json;
		return array.dump();
	});

    app.port(HTTP_PORT).multithreaded().run();
}

void udp_server()
{
	struct sockaddr_in myaddr;      /* our address */
        struct sockaddr_in remaddr;     /* remote address */
        socklen_t addrlen = sizeof(remaddr);            /* length of addresses */
        long int recvlen;                    /* # bytes received */
        int fd;                         /* our socket */
        unsigned char buf[BUFSIZE];     /* receive buffer */

        /* create a UDP socket */

        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("cannot create socket\n");
                return;
        }

        /* bind the socket to any valid IP address and a specific port */

        memset((char *)&myaddr, 0, sizeof(myaddr));
        myaddr.sin_family = AF_INET;
        myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        myaddr.sin_port = htons(UPD_PORT);

        if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
                perror("bind failed");
                return;
        }

        /* now loop, receiving data and printing what we received */
        for (;;) {
                printf("waiting on port %d\n", UPD_PORT);
                recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
                printf("received %ld bytes\n", recvlen);
                if (recvlen > 0) {
                        buf[recvlen] = 0;
                        printf("received message: \"%s\"\n", buf);
                }
        }
        /* never exits */
}


int main()
{
    std::thread http (http_server);
	std::thread udp (udp_server);
	http.join();
	udp.join();
	return 0;
}