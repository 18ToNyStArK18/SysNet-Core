#include "sham.h"
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
		char *data_sent[10];	 // for storing the data_sent
		char *data_recieved[10]; // for storing the data_recieved
		int sent_bytes[10];		 // storing the endbytes
		int current_sent_bytes = 0;
		struct timeval sent_time[10];
		memset(sent_bytes, 0, sizeof(sent_bytes));
		for (int i = 0; i < 10; i++)
		{
			data_sent[i] = (char *)malloc(1025);
			data_sent[i][0] = '\0';
			data_recieved[i] = (char *)malloc(1025);
			data_recieved[i][0] = '\0';
		}

		for (int i = 0; i < 10; i++)
			memset(&sent_time[i], 0, sizeof(sent_time[i]));
		socklen_t len = sizeof(client);
		int k = 0;
		while (1)
		{
			int maxfd = socketfd + 1;
			fd_set readfds;
			FD_ZERO(&readfds);
			FD_SET(0, &readfds);		// stdin
			FD_SET(socketfd, &readfds); // from socket
			select(maxfd, &readfds, NULL, NULL, NULL);
			char *buff = (char *)malloc(1025);
			// checking the timer value
			for (int i = 0; i < 10; i++)
			{
				struct timeval curr_time;
				gettimeofday(&curr_time, NULL);
				if (sent_bytes[i] && elapsed_ms(sent_time[i], curr_time) >= timer)
				{
					pack_str curr_packet;
					strcpy(curr_packet.data, data_sent[i]);
					curr_packet.a.seq_num = htonl(sent_bytes[i] - strlen(data_sent[i]) + 1);
					curr_packet.a.flags = htons(0);
					curr_packet.a.ack_num = htonl(0);
					sendto(socketfd, &curr_packet, sizeof(curr_packet), 0, (struct sockaddr *)&client, len);
					gettimeofday(&sent_time[i], NULL);
				}
			}
			// if user input
			if (FD_ISSET(0, &readfds))
			{
				fgets(buff, 1025, stdin);
				int size = strlen(buff);
				int available = 0;
				for (available = 0; available < 10; available++)
				{
					if (strlen(data_sent[available]) == 0)
						break;
				}
				if (available < 10)
				{
					strcpy(data_sent[available], buff);
					current_sent_bytes = current_sent_bytes + size;
					sent_bytes[available] = current_sent_bytes + 1;
					pack_str curr_packet;
					curr_packet.a.seq_num = htonl(current_sent_bytes - size + 1);
					curr_packet.a.flags = htons(0);
					curr_packet.a.ack_num = htonl(0);
					strcpy(curr_packet.data, buff);
					sendto(socketfd, &curr_packet, sizeof(curr_packet), 0, (struct sockaddr *)&client, len);
					gettimeofday(&sent_time[available], NULL);
				}
			}
			if (FD_ISSET(socketfd, &readfds))
			{
				pack_str response;
				int rec = recvfrom(socketfd, &response, sizeof(response), 0, (struct sockaddr *)&client, &len);
				uint16_t flags = ntohs(response.a.flags);
				if (flags & ACK)
				{
					uint32_t ack_rec = ntohl(response.a.ack_num);
					for (int i = 0; i < 10; i++)
					{
						if (sent_bytes[i] == ack_rec)
						{
							sent_bytes[i] = 0;
							data_sent[i][0] = '\0';
							memset(&sent_time[i], 0, sizeof(sent_time[i]));
						}
					}
				}
				else
				{
					// printf("%s\n\n\n",response.data);
					uint32_t rec_seq = ntohl(response.a.seq_num);
					int index = (rec_seq - 1) / 1024;
					printf("%d %u\n\n\n", index, rec_seq);
					if (strlen(data_recieved[index % 10]) == 0)
						strcpy(data_recieved[index % 10], response.data);
					pack_str ack_send;
					ack_send.a.flags = htons(ACK);
					ack_send.a.ack_num = htonl(rec_seq + strlen(response.data));
					sendto(socketfd, &ack_send, sizeof(ack_send), 0, (struct sockaddr *)&client, len);
				}
			}
			while (strlen(data_recieved[k % 10]) != 0)
			{
				printf("%s", data_recieved[k % 10]);
				data_recieved[k % 10][0] = '\0';
				k++;
			}
			int i = 0;
			while (i < 10 && !strlen(data_recieved[i]))
				i++;
			if (i == 10)
				k = 0;
		}

		close(socketfd);
		exit(0);
	}
	// file mode -------------------------------------------
	else
	{
		socklen_t len = sizeof(client);
		int recieved = 1;
		not_ACKed buffer_data[10];
		for (int i = 0; i < 10; i++)
		{
			buffer_data[i].data[0] = '\0';
			buffer_data[i].end = 0;
		}
		// recieve output.txt
		char output_file[] = "output.txt"; // temporary
		FILE *fp = fopen(output_file, "w");
		while (1)
		{
			pack_str response;
			int r = recvfrom(socketfd, &response, sizeof(response), 0, (struct sockaddr *)&client, &len);
			if(ntohs(response.a.flags) & FIN){
				fclose(fp);
				printf("FILE succesfully recieved\n");
				exit(0);
			}
			strcpy(buffer_data[0].data, response.data);
			buffer_data[0].end = ntohs(response.a.seq_num) + strlen(buffer_data[0].data);
			qsort(buffer_data, 10, sizeof(buffer_data[0]), comp);
			int i = 0;
			while (buffer_data[i].end == 0)
				i++;
			for (i; i < 10; i++)
			{
				if (recieved + strlen(buffer_data[i].data) == buffer_data[i].end)
				{
					fwrite(buffer_data[i].data,1,strlen(buffer_data[i].data),fp);
					recieved = buffer_data[i].end;
					buffer_data[i].data[0] = '\0';
					buffer_data[i].end = 0;	
				}
				else
					break;
			}
			pack_str sending;
			sending.a.ack_num = htons(recieved);
			sendto(socketfd, &sending, sizeof(sending), 0, (struct sockaddr *)&client, len);
		}
	}
	close(socketfd);
	return 0;
}
