#include "sham.h"
void four_way_hand_shake_reciever(int socketfd, struct sockaddr_in *sender)
{
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
    while (1)
    {
        memset(&pkt, 0, sizeof(pkt));
        int r = recvfrom(socketfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)sender, &len);
        if ((ntohs(pkt.a.flags) & ACK) == ACK)
        {
            printf("Reciver: Recived Final ACK\n");
            break;
        }
    }
}

void four_way_hand_shake_sender(int socketfd, struct sockaddr_in *sender)
{
    printf("Initiater: Sent FIN\n");
    pack_str pkt;
    socklen_t len = sizeof(*sender);

    // Wait until we get ACK
    while (1)
    {
        memset(&pkt, 0, sizeof(pkt));
        int r = recvfrom(socketfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)sender, &len);
        if ((ntohs(pkt.a.flags) & ACK) == ACK)
        {
            printf("Initiator: Recived ACK\n");
            break;
        }
    }

    // Wait until we get FIN
    while (1)
    {
        memset(&pkt, 0, sizeof(pkt));
        int r = recvfrom(socketfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)sender, &len);
        if ((ntohs(pkt.a.flags) & FIN) == FIN)
        {
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
int isFree(not_ACKed a[10])
{
    for (int i = 0; i < 10; i++)
    {
        if (a[i].len == 0)
            return 1;
    }
    return 0;
}
int comp(const void *a, const void *b)
{
    not_ACKed A = *(not_ACKed *)a;
    not_ACKed B = *(not_ACKed *)b;
    return A.end - B.end;
}
int main(int argc, char *argv[])
{

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int chat_mode = 0;
    float loss_rate = 0.0;
    char *input_file = (char *)malloc(1024);
    char *output_file = (char *)malloc(1024);
    input_file[0] = '\0';
    output_file[0] = '\0';
    if (strcmp(argv[3], "--chat") == 0)
    {
        chat_mode = 1;
        if (argc == 5)
            loss_rate = atof(argv[4]);
    }
    else
    {
        strcpy(input_file, argv[3]);
        strcpy(output_file, argv[4]);
        if (argc == 6)
            loss_rate = atof(argv[5]);
    }
    int socketfd;
    struct sockaddr_in server_addr;

    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket Creation failed");
        return 0;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid server IP");
        close(socketfd);
        exit(1);
    }

    client_3_way_handshake(socketfd, &server_addr);

    fd_set readfds;
    // chat mode   -----------------------------------------
    if (chat_mode)
    {
        char buff[1025];
        not_ACKed data_recieved[10];
        not_ACKed data_sent[10];
        memset(&data_recieved, 0, sizeof(data_recieved));
        memset(&data_sent, 0, sizeof(data_sent));
        int quit = 0;
        int tot_size = 0;
        int recieved = 1;
        socklen_t len = sizeof(server_addr);

        printf("Chat mode started. Type '/quit' to exit.\n");

        while (!quit)
        {
            FD_ZERO(&readfds);
            FD_SET(0, &readfds);
            FD_SET(socketfd, &readfds);
            struct timeval timeout = {0, 100000};
            int activity = select(socketfd + 1, &readfds, NULL, NULL, &timeout);

            if (activity > 0)
            {
                // Handle incoming data
                if (FD_ISSET(socketfd, &readfds))
                {
                    pack_str response;
                    int rec = recvfrom(socketfd, &response, sizeof(response), MSG_DONTWAIT, (struct sockaddr *)&server_addr, &len);
                    if (rec > 0)
                    {
                        // Check if it's an ACK for data we sent
                        if ((ntohs(response.a.flags) & ACK) == ACK)
                        {
                            int ack_num = ntohl(response.a.ack_num);
                            for (int i = 0; i < 10; i++)
                            {
                                if (data_sent[i].end != 0 && data_sent[i].end <= ack_num)
                                {
                                    data_sent[i].data[0] = '\0';
                                    data_sent[i].end = 0;
                                    data_sent[i].len = 0;
                                    data_sent[i].time.tv_usec = 0;
                                    data_sent[i].time.tv_sec = 0;
                                }
                            }
                        }
                        else
                        {
                            // It's actual data - store and process
                            if (ntohl(response.len) > 0)
                            {
                                int seq_num = ntohl(response.a.seq_num);
                                int data_len = ntohl(response.len);

                                // Find free slot or update existing
                                int slot = -1;
                                for (int i = 0; i < 10; i++)
                                {
                                    if (data_recieved[i].len == 0)
                                    {
                                        slot = i;
                                        break;
                                    }
                                }

                                if (slot != -1)
                                {
                                    memcpy(data_recieved[slot].data, response.data, data_len);
                                    data_recieved[slot].data[data_len] = '\0';
                                    data_recieved[slot].len = data_len;
                                    data_recieved[slot].end = seq_num + data_len;

                                    // Sort by sequence number
                                    qsort(data_recieved, 10, sizeof(data_recieved[0]), comp);

                                    // Process consecutive data
                                    for (int i = 0; i < 10; i++)
                                    {
                                        if (data_recieved[i].len > 0 && recieved == (data_recieved[i].end - data_recieved[i].len))
                                        {
                                            printf("%s", data_recieved[i].data);
                                            fflush(stdout);
                                            recieved = data_recieved[i].end;
                                            data_recieved[i].data[0] = '\0';
                                            data_recieved[i].end = 0;
                                            data_recieved[i].len = 0;
                                        }
                                    }
                                }

                                // Send ACK
                                pack_str ack_packet;
                                memset(&ack_packet, 0, sizeof(ack_packet));
                                ack_packet.a.ack_num = htonl(recieved);
                                ack_packet.a.flags = htons(ACK);
                                sendto(socketfd, &ack_packet, sizeof(ack_packet), 0, (struct sockaddr *)&server_addr, len);
                            }
                        }
                    }
                }

                // Handle user input
                if (FD_ISSET(0, &readfds))
                {
                    if (fgets(buff, 1024, stdin) != NULL)
                    {
                        if (strcmp(buff, "/quit\n") == 0)
                        {
                            quit = 1;
                            break;
                        }

                        // Send data if we have free slots
                        if (isFree(data_sent))
                        {
                            int free_index = 0;
                            for (free_index = 0; free_index < 10; free_index++)
                            {
                                if (data_sent[free_index].len == 0)
                                    break;
                            }

                            int msg_len = strlen(buff);
                            printf("%d\n",msg_len);
                            data_sent[free_index].len = msg_len;
                            strcpy(data_sent[free_index].data, buff);
                            tot_size += msg_len;
                            data_sent[free_index].end = tot_size;
                            gettimeofday(&data_sent[free_index].time, NULL);

                            pack_str packet;
                            memset(&packet, 0, sizeof(packet));
                            packet.a.seq_num = htonl(tot_size - msg_len + 1);
                            memcpy(packet.data, buff, msg_len);
                            packet.len = htonl(msg_len);

                            int to_send = sizeof(packet.a) + sizeof(packet.len) + msg_len;
                            sendto(socketfd, &packet, to_send, 0, (struct sockaddr *)&server_addr, len);
                        }
                    }
                }
            }

            // Handle retransmissions
            struct timeval now;
            gettimeofday(&now, NULL);
            for (int i = 0; i < 10; i++)
            {
                if (data_sent[i].end != 0 && elapsed_ms(data_sent[i].time, now) > 500)
                {
                    pack_str retrans_packet;
                    memset(&retrans_packet, 0, sizeof(retrans_packet));
                    retrans_packet.a.seq_num = htonl(data_sent[i].end - data_sent[i].len + 1);
                    memcpy(retrans_packet.data, data_sent[i].data, data_sent[i].len);
                    retrans_packet.len = htonl(data_sent[i].len);

                    int to_send = sizeof(retrans_packet.a) + sizeof(retrans_packet.len) + data_sent[i].len;
                    sendto(socketfd, &retrans_packet, to_send, 0, (struct sockaddr *)&server_addr, len);
                    gettimeofday(&data_sent[i].time, NULL);
                    printf(".");
                    fflush(stdout);
                }
            }
        }

        printf("\nChat ended.\n");
    }
    // file mode ------------------------------------------------------------------
    else
    {
        fd_set readfds;
        socklen_t len = sizeof(server_addr);
        not_ACKed buffer_data[10];
        pack_str file_name;
        file_name.a.seq_num = htonl(0);
        strcpy(file_name.data, output_file);
        sendto(socketfd, &file_name, sizeof(file_name), 0, (struct sockaddr *)&server_addr, len);
        FILE *fp = fopen(input_file, "rb");
        if (!fp)
        {
            printf("File doesnt exist\n");
            exit(1);
        }
        char buff[1025];
        for (int i = 0; i < 10; i++)
        {
            buffer_data[i].data[0] = '\0';
            buffer_data[i].end = 0;
            buffer_data[i].len = 0;
        }
        int total_size = 0;
        int done = 0, flag1 = 0, flag2 = 1;
        while (!done)
        {
            flag2 = 1;
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 50000;
            FD_ZERO(&readfds);
            FD_SET(socketfd, &readfds);
            int activity = select(socketfd + 1, &readfds, NULL, NULL, &tv);
            if (activity > 0 && FD_ISSET(socketfd, &readfds))
            {
                pack_str response;
                int rec = recvfrom(socketfd, &response, sizeof(response), 0, (struct sockaddr *)&server_addr, &len);
                int ack_num = ntohl(response.a.ack_num);
                // printf("%d\n",ack_num);
                for (int i = 0; i < 10; i++)
                {
                    if (buffer_data[i].end <= ack_num)
                    {
                        buffer_data[i].data[0] = '\0';
                        buffer_data[i].end = 0;
                        buffer_data[i].len = 0;
                        buffer_data[i].time.tv_usec = 0;
                        buffer_data[i].time.tv_sec = 0;
                    }
                }
            }
            while (!flag1 && isFree(buffer_data))
            {
                int n = fread(buff, 1, sizeof(buff), fp);
                if (n <= 0)
                {
                    // printf("%d\n",total_size);
                    flag1 = 1;
                    break;
                }
                int free_index = 0;
                for (free_index; free_index < 10; free_index++)
                {
                    if (buffer_data[free_index].len == 0)
                        break;
                }
                buffer_data[free_index].len = n;
                memcpy(buffer_data[free_index].data, buff, n);
                total_size = total_size + n;
                buffer_data[free_index].end = total_size + 1;
                gettimeofday(&buffer_data[free_index].time, NULL);
                pack_str curr_pac;
                curr_pac.a.seq_num = htonl(total_size - n + 1);
                memcpy(curr_pac.data, buff, n);
                curr_pac.len = htonl(n);
                int to_send = sizeof(curr_pac.a) + sizeof(curr_pac.len) + n;
                sendto(socketfd, &curr_pac, to_send, 0, (struct sockaddr *)&server_addr, len);
            }
            struct timeval now;
            gettimeofday(&now, NULL);
            for (int i = 0; i < 10; i++)
            {
                if (buffer_data[i].end != 0 && elapsed_ms(buffer_data[i].time, now) > 500)
                {
                    pack_str curr_pac;
                    curr_pac.a.seq_num = htonl(buffer_data[i].end - buffer_data[i].len);
                    memcpy(curr_pac.data, buffer_data[i].data, buffer_data[i].len);
                    curr_pac.len = htonl(buffer_data[i].len);
                    int to_send = sizeof(curr_pac.a) + sizeof(curr_pac.len) + buffer_data[i].len;
                    sendto(socketfd, &curr_pac, to_send, 0, (struct sockaddr *)&server_addr, len);
                    gettimeofday(&buffer_data[i].time, NULL);
                    flag2 = 0;
                }
            }
            if (flag1 & flag2)
            {
                pack_str pc;
                pc.a.flags = htons(FIN);
                sendto(socketfd, &pc, sizeof(pc), 0, (struct sockaddr *)&server_addr, len);
                four_way_hand_shake_sender(socketfd, &server_addr);
                done = 1;
            }
        }
        printf("File Transfer Succesfull\n");
    }
    close(socketfd);
    return 0;
}