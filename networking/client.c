#include "sham.h"

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
        if (strlen(a[i].data) == 0)
            return 1;
    }
    return 0;
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

    // chat_mode ----------------------------------------------------------------
    if (chat_mode)
    {
        char *data_sent[10];     // for storing the data_sent
        char *data_recieved[10]; // for storing the data_recieved
        int sent_bytes[10];      // storing the endbytes
        int current_sent_bytes = 0;
        struct timeval sent_time[10];
        for (int i = 0; i < 10; i++)
        {
            data_sent[i] = (char *)malloc(1025);
            data_sent[i][0] = '\0';
            data_recieved[i] = (char *)malloc(1025);
            data_recieved[i][0] = '\0';
            sent_bytes[i] = 0;
        }

        for (int i = 0; i < 10; i++)
            sent_time[i].tv_sec = sent_time[i].tv_usec = 0;
        socklen_t len = sizeof(server_addr);
        int k = 0;
        while (1)
        {
            int maxfd = socketfd + 1;
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(0, &readfds);        // stdin
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
                    sendto(socketfd, &curr_packet, sizeof(curr_packet), 0, (struct sockaddr *)&server_addr, len);
                    gettimeofday(&sent_time[i], NULL);
                }
            }
            // if user input
            if (FD_ISSET(0, &readfds))
            {

                fgets(buff, 1025, stdin);
                int size = strlen(buff);
                // printf("size %d\n\n\n",size);
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
                    printf("%s \n\n\n", curr_packet.data);
                    sendto(socketfd, &curr_packet, sizeof(curr_packet), 0, (struct sockaddr *)&server_addr, len);
                    gettimeofday(&sent_time[available], NULL);
                }
            }
            if (FD_ISSET(socketfd, &readfds))
            {
                pack_str response;
                int rec = recvfrom(socketfd, &response, sizeof(response), 0, (struct sockaddr *)&server_addr, &len);
                uint16_t flags = ntohs(response.a.flags);
                if (flags & ACK)
                {
                    uint32_t ack_rec = ntohl(response.a.ack_num);
                    // printf("%u\n\n\n",ack_rec);
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
                    uint32_t rec_seq = ntohl(response.a.seq_num);
                    int index = (rec_seq - 1) / 1024;
                    if (strlen(data_recieved[index % 10]) == 0)
                        strcpy(data_recieved[index % 10], response.data);
                    pack_str ack_send;
                    ack_send.a.flags = htons(ACK);
                    ack_send.a.ack_num = htonl(rec_seq + strlen(response.data));
                    sendto(socketfd, &ack_send, sizeof(ack_send), 0, (struct sockaddr *)&server_addr, len);
                }
            }

            while (strlen(data_recieved[k % 10]) != 0)
            {
                printf("%s", data_recieved[k % 10]);
                data_recieved[k % 10][0] = '\0';
                k++;
            }
        }

        close(socketfd);
        exit(0);
    }
    // file mode ------------------------------------------------------------------
    else
    {
        fd_set readfds;
        socklen_t len = sizeof(server_addr);
        not_ACKed buffer_data[10];
        FILE *fp = fopen(input_file, "r");
        if (!fp)
        {
            printf("File doesnt exist\n");
            exit(1);
        }
        char buff[1025];
        for (int i = 0; i < 10; i++)
            buffer_data[i].data[0] = '\0';
        int total_size = 0;
        int done =0,flag1=0,flag2=1;
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
                int ack_num = ntohs(response.a.ack_num);
                for (int i = 0; i < 10; i++)
                {
                    if (buffer_data[i].end <= ack_num)
                    {
                        buffer_data[i].data[0] = '\0';
                        buffer_data[i].end = 0;
                        buffer_data[i].pos = 0;
                        buffer_data[i].time.tv_usec = 0;
                        buffer_data[i].time.tv_sec = 0;
                    }
                }

            }
            while (!flag1 && isFree(buffer_data))
            {
                if (!fgets(buff,1025,fp)){
                    flag1 =1;
                    break;
                }
                int free_index = 0;
                for (free_index; free_index < 10; free_index++)
                {
                    if (strlen(buffer_data[free_index].data) == 0)
                        break;
                }
                buffer_data[free_index].pos = total_size / 1024;
                strcpy(buffer_data[free_index].data, buff);
                total_size = total_size + strlen(buff);
                buffer_data[free_index].end = total_size + 1;
                gettimeofday(&buffer_data[free_index].time, NULL);
                pack_str curr_pac;
                curr_pac.a.seq_num = htons(total_size - strlen(buff) + 1);
                strcpy(curr_pac.data, buff);
                sendto(socketfd, &curr_pac, sizeof(curr_pac), 0, (struct sockaddr *)&server_addr, len);
            }
            struct timeval now;
            gettimeofday(&now, NULL);
            for (int i = 0; i < 10; i++)
            {
                if (buffer_data[i].end != 0 && elapsed_ms(buffer_data[i].time, now) > 500)
                {
                    pack_str curr_pac;
                    curr_pac.a.seq_num = htons(buffer_data[i].end - strlen(buffer_data[i].data));
                    strcpy(curr_pac.data, buffer_data[i].data);
                    sendto(socketfd, &curr_pac, sizeof(curr_pac), 0, (struct sockaddr *)&server_addr, len);
                    gettimeofday(&buffer_data[i].time, NULL);
                    flag2 = 0;
                }
            }
            if(flag1 & flag2){
                pack_str pc;
                pc.a.flags = htons(FIN);
                sendto(socketfd,&pc,sizeof(pc),0,(struct sockaddr *)&server_addr,len);
                done = 1;
            }
        }
        printf("File Transfer Succesfull\n");
    }
    close(socketfd);
    return 0;
}
