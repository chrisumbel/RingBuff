/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ring_buff.h
 * Author: kilnaar
 *
 * Created on July 20, 2020, 7:35 AM
 */

#ifndef RING_BUFF_H
#define RING_BUFF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RingBuff {
    unsigned char *buff;
    int len;
    unsigned int sink_pos;
    unsigned int source_pos;
    unsigned char sink_iteration;
    unsigned char source_iteration;
} RingBuff_t;    

int ring_init(RingBuff_t *ring_buff, unsigned char *buff, int len);
int ring_is_emtpy(RingBuff_t *ring_buff);
int ring_read(RingBuff_t *ring_buff);
int ring_write(RingBuff_t *ring_buff, unsigned char b);

#ifdef __cplusplus
}
#endif

#endif /* RING_BUFF_H */

