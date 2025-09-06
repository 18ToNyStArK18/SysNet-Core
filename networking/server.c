#include "sham.h"
void four_way_hand_shake_reciever(int socketfd, struct sockaddr_in *sender) {
    printf("Reciever: recieved FIN\n");
    pack_str pkt;
    socklen_t len = sizeof(*sender);

    // Send ACK
    memset(&pkt, 0, sizeof(pkt));
    pkt.a.flags = htons(ACK);
    sendto(socketfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)sender, len);
    printf("Reciever: Sent ACK\n");

    // Send FIN
    memset(&pkt, 0, sizeof(pkt));
    pkt.a.flags = htons(FIN);
    sendto(socketfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)sender, len);
    printf("Reciever: Sent FIN\n");

    // Wait for final ACK
    while (1) {
        memset(&pkt, 0, sizeof(pkt));
        int r = recvfrom(socketfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)sender, &len);
        if ((ntohs(pkt.a.flags) & ACK) == ACK) {
            printf("Reciver: Recived Final ACK\n");
            break;
        }
    }
}


void four_way_hand_shake_sender(int socketfd, struct sockaddr_in *sender) {
    printf("Initiater: Sent FIN\n");
    pack_str pkt;
    socklen_t len = sizeof(*sender);

    // Wait until we get ACK
    while (1) {
        memset(&pkt, 0, sizeof(pkt));
        int r = recvfrom(socketfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)sender, &len);
        if ((ntohs(pkt.a.flags) & ACK) == ACK) {
            printf("Initiator: Recived ACK\n");
            break;
        }
    }

    // Wait until we get FIN
    while (1) {
        memset(&pkt, 0, sizeof(pkt));
        int r = recvfrom(socketfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)sender, &len);
        if ((ntohs(pkt.a.flags) & FIN) == FIN) {
            printf("Initiator: Recived FIN\n");
            break;
        }
    }

    // Send final ACK
    memset(&pkt, 0, sizeof(pkt));
    pkt.a.flags = htons(ACK);
    sendto(socketfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)sender, len);
    printf("Intitater: Sent Final ACK\n");
}


void client_3_way_handshake(int socketfd, struct sockaddr_in *server_addr)
{
    socklen_t sock_len = sizeof(*server_addr);
    sham packet, response;
    int rec;
    uint32_t seq = 1;

    // Send SYN
    memset(&packet, 0, sizeof(packet));
    packet.seq_num = htonl(seq);
    packet.flags = htons(SYN);

    sendto(socketfd, &packet, sizeof(packet), 0, (struct sockaddr *)server_addr, sock_len);
    printf("CLIENT: Sent SYN with seq_num=%u\n", seq);

    // Receive SYN+ACK
    rec = recvfrom(socketfd, &response, sizeof(response), 0, (struct sockaddr *)server_addr, &sock_len);
    if (rec < 0)
    {
        perror("recvfrom failed");
        exit(1);
    }

    uint32_t server_seq_num = ntohl(response.seq_num);
    uint32_t ack_num = ntohl(response.ack_num);

    if (ntohs(response.flags) == (SYN | ACK))
    {
        printf("CLIENT: Received SYN+ACK with server_seq=%u, ack=%u\n", server_seq_num, ack_num);
    }
    else
    {
        printf("CLIENT: Wrong flags in response!\n");
        exit(1);
    }

    // Send ACK
    memset(&packet, 0, sizeof(packet));
    packet.flags = htons(ACK);
    packet.ack_num = htonl(server_seq_num + 1);

    sendto(socketfd, &packet, sizeof(packet), 0, (struct sockaddr *)server_addr, sock_len);
    printf("CLIENT: Sent ACK=%u\n", server_seq_num + 1);

    printf("CLIENT: Connection established!\n");
    return;
}
int comp(const void *a, const void *b)
{
	not_ACKed A = *(not_ACKed *)a;
	not_ACKed B = *(not_ACKed *)b;
	return A.end - B.end;
}
void server_3_way_handshake(int socketfd, struct sockaddr_in *client)
{
	sham packet, response;
	socklen_t len = sizeof(*client);

	// Receive SYN from client
	int rec = recvfrom(socketfd, &response, sizeof(response), 0, (struct sockaddr *)client, &len);
	if (rec < 0)
	{
		perror("recvfrom failed");
		exit(1);
	}

	uint32_t client_seq = ntohl(response.seq_num);
	printf("SERVER: Received SYN with seq_num=%u\n", client_seq);

	if (ntohs(response.flags) != SYN)
	{
		printf("SERVER: Expected SYN, got wrong flag!\n");
		exit(1);
	}

	// Send SYN+ACK
	uint32_t server_seq_num = 100; // example server seq
	memset(&packet, 0, sizeof(packet));
	packet.seq_num = htonl(server_seq_num);
	packet.ack_num = htonl(client_seq + 1);
	packet.flags = htons(SYN | ACK);

	sendto(socketfd, &packet, sizeof(packet), 0, (struct sockaddr *)client, len);
	printf("SERVER: Sent SYN+ACK with seq_num=%u, ack_num=%u\n", server_seq_num, client_seq + 1);

	// Receive final ACK
	rec = recvfrom(socketfd, &packet, sizeof(packet), 0, (struct sockaddr *)client, &len);
	if (rec < 0)
	{
		perror("recvfrom failed");
		exit(1);
	}

	uint32_t ack_num = ntohl(packet.ack_num);
	if (ntohs(packet.flags) == ACK)
	{
		printf("SERVER: Received ACK=%u\n", ack_num);
	}

	printf("SERVER: Connection established!\n");
	return;
}

int main(int argc, char *argv[])
{

	int port = atoi(argv[1]);
	int socketfd;
	struct sockaddr_in server, client;
	float loss_rate = 0.0;
	int chat_mode = 0;
	if (argc > 2)
	{
		if (strcmp(argv[2], "--chat") == 0)
			chat_mode = 1;
		else
			loss_rate = atof(argv[2]);
		if (argc == 4)
			loss_rate = atof(argv[3]);
	}
	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("SOCKET CREATION FAILED");
		return 0;
	}

	memset(&server, 0, sizeof(server));
	memset(&client, 0, sizeof(client));

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(socketfd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("BIND FAILED");
		close(socketfd);
		return 0;
	}

	printf("SERVER: Listening on port %d...\n", port);
	server_3_way_handshake(socketfd, &client);

	// chat mode   -----------------------------------------
	if (chat_mode)
	{
	}
	// file mode -------------------------------------------
	else
	{
		socklen_t len = sizeof(client);
		int recieved = 1;
		not_ACKed buffer_data[10];
		pack_str file;
		int f = recvfrom(socketfd,&file,sizeof(file),0,(struct sockaddr *)&client,&len);
		if(f < 0){
			printf("Error in recieveing data\n");
		}
		char *output_file = (char *)malloc(1024);
		if(ntohl(file.a.seq_num) == 0)
			strcpy(output_file,file.data);
		else{
			printf("Didnt recieve the file name\n");
			exit(0);
		}
		for (int i = 0; i < 10; i++)
		{
			buffer_data[i].data[0] = '\0';
			buffer_data[i].end = 0;
			buffer_data[i].len = 0;
		}
		FILE *fp = fopen(output_file, "wb");
		while (1)
		{
			pack_str response;
			int r = recvfrom(socketfd, &response, sizeof(response), 0, (struct sockaddr *)&client, &len);
			if(ntohs(response.a.flags) & FIN){
				four_way_hand_shake_reciever(socketfd,&client);
				fclose(fp);
				printf("FILE succesfully recieved\n");
				exit(0);
			}
			memcpy(buffer_data[0].data, response.data,ntohl(response.len));
			buffer_data[0].len = ntohl(response.len);
			buffer_data[0].end = ntohl(response.a.seq_num) + buffer_data[0].len;
			qsort(buffer_data, 10, sizeof(buffer_data[0]), comp);
			int i = 0;
			while (buffer_data[i].end == 0)
				i++;
			for (i; i < 10; i++)
			{
				if (recieved + buffer_data[i].len == buffer_data[i].end)
				{
					fwrite(buffer_data[i].data,1,buffer_data[i].len,fp);
					recieved = buffer_data[i].end;
					buffer_data[i].data[0] = '\0';
					buffer_data[i].end = 0;	
				}
				else
					break;
			}
			pack_str sending;
			sending.a.ack_num = htonl(recieved);
			sendto(socketfd, &sending, sizeof(sending), 0, (struct sockaddr *)&client, len);
			// printf("%d\n",recieved);
		}
	}
	close(socketfd);
	return 0;
}
