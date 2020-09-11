#ifndef __RINGBUF
#define __RINGBUF
#include <stdint.h>
#include <stdbool.h>
typedef struct
{
    void * data;
    uint32_t buf_size;
    uint32_t frame_size;
    uint32_t read_idx; //下一个可读取索引
    uint32_t write_idx; //下一个可写入索引
    uint32_t unread_cnts;
    bool overwrite,lock;
}RingBuf_t;

typedef enum {
    READ_END=0,
    WRITE_END,
    READ_OK,
    WRITE_OK
}RingBuf_State_e;

void RingBuf_Creat(RingBuf_t *pbuf,void *source,uint32_t _size,uint32_t _frame_size);
RingBuf_State_e RingBuf_Read(RingBuf_t *pbuf,void **frame_ptr);
RingBuf_State_e RingBuf_Write(RingBuf_t *pbuf,void **frame_ptr);

#endif
