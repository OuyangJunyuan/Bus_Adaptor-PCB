#include "ringbuf.h"

void RingBuf_Creat(RingBuf_t *pbuf,void *source,uint32_t _size,uint32_t _frame_size) {
    pbuf->data = source;
    pbuf->buf_size = _size;
    pbuf->frame_size = _frame_size;
    pbuf->read_idx = _size - 1;
    pbuf->write_idx = 0;
    pbuf->overwrite = false;
    pbuf->lock = false;
    pbuf->unread_cnts=0;
}

RingBuf_State_e RingBuf_Read(RingBuf_t *pbuf,void **frame_ptr) {
    if(pbuf->unread_cnts==0)
    {
        return READ_END;
    }else
    {
        *frame_ptr = (uint8_t *) (pbuf->data) + pbuf->frame_size * pbuf->read_idx;
        pbuf->read_idx = (pbuf->read_idx + 1) % pbuf->buf_size;
        --(pbuf->unread_cnts);
        return READ_OK;
    }

}

RingBuf_State_e RingBuf_Write(RingBuf_t *pbuf,void **frame_ptr) {
    *frame_ptr = (uint8_t *) (pbuf->data) + pbuf->frame_size * pbuf->write_idx;
    pbuf->write_idx = (pbuf->write_idx + 1) % pbuf->buf_size;
    ++(pbuf->unread_cnts);
    if (pbuf->unread_cnts >= pbuf->buf_size) {
        pbuf->unread_cnts=pbuf->buf_size;
        pbuf->read_idx = pbuf->write_idx;
        return WRITE_END;
    }
    return WRITE_OK;
}
