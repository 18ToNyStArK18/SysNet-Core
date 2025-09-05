#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<stdint.h>
#include<arpa/inet.h>
#include<unistd.h>



typedef struct sham_header {

uint32_t seq_num; // Sequence Number

uint32_t ack_num; // Acknowledgment Number

uint16_t flags; // Control flags (SYN, ACK, FIN)

uint16_t window_size; // Flow control window size

}sham;

typedef struct pack_str
{
    sham a;
    char data[1025];
}pack_str;
typedef struct s_truct
{
    int pos;
    int end;
    char data[1025];
    struct timeval time;
}not_ACKed;
#define SYN 0X1
#define ACK 0X2
#define FIN 0X3
#define timer 500000
static inline long elapsed_ms(struct timeval a, struct timeval b) {
    return (b.tv_sec - a.tv_sec)/1000 + (b.tv_usec - a.tv_usec)*1000 ;
}
