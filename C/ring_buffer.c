#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ringBuffer
{
    int _size;//用于判断环形buffer是否是满的
    int _wroffset;  
    int _rdoffset;
    char* _buf;
    int _buf_len;
}ringBuffer;

ringBuffer* ringbuffer_init(int buf_len)
{
    if (buf_len == 0)
        return NULL;
    ringBuffer* ring = (ringBuffer*)malloc(sizeof(ringBuffer));
    if (ring == NULL) {
        printf("malloc error \n");
        return NULL;
    }

    memset(ring, 0, sizeof(ringBuffer));
    ring->_buf_len = buf_len;
    ring->_buf = (char*)malloc(buf_len);
    if (ring->_buf == NULL) {
        printf("malloc error! \n");
        return NULL;
    }

    memset(ring->_buf, 0, buf_len);
    return ring;
}
//返回读的数据的数量
int ringbuffer_read(ringBuffer* ring, char* outbuf, int read_len)
{
    int can_read_len = 0;
    if (outbuf == NULL) 
        return 0;
    if (ring->_size == 0) 
        return 0;//没有数据可以读
    can_read_len = ring->_wroffset - ring->_rdoffset;
    if (can_read_len < 0)
        can_read_len += ring->_buf_len;
    if (can_read_len < read_len) {
        read_len = can_read_len;//readLen是能读到的数量
    }
    if (read_len < ring->_buf_len - ring->_rdoffset) {
        //说明不用不用折回来读
        memcpy(outbuf, &ring->_buf[ring->_rdoffset], read_len);
    } else {
        //说明存到最大的地方没存完，需要继续折回来读,第一次读先读到数组的结尾
        memcpy(outbuf, &ring->_buf[ring->_rdoffset], ring->_buf_len - ring->_rdoffset);
        memcpy(&outbuf[ring->_buf_len - ring->_rdoffset], ring->_buf, read_len - (ring->_buf_len - ring->_rdoffset));
    }
    //更新下实际的数量大小
    ring->_size -= read_len;
    //更新读指针
    ring->_rdoffset = (read_len + ring->_rdoffset) % ring->_buf_len;
    return read_len;//返回读到的数据量
}
//返回是否写入成功,如果空间不够就不写入，失败返回0
int ringbuffer_write(ringBuffer* ring, char* inbuf, int write_len)
{
    int can_write_len = 0;
    if (ring == NULL || inbuf == NULL || write_len == 0) {
        return 0;
    }
    //检查环形buffer是否是满的
    if (ring->_size == ring->_buf_len) {
        return 0;
    }
    //检查剩余的空间是否够写入
    can_write_len = ring->_rdoffset - ring->_wroffset;
    if (can_write_len <= 0) 
        can_write_len += ring->_buf_len;
    if (write_len > can_write_len) 
        return -1;//没有足够的空间则不写入
    //开始写
    if (write_len < ring->_buf_len - ring->_wroffset) {
        //不用折行，直接写入
        memcpy(&ring->_buf[ring->_wroffset], inbuf, write_len);
    } else {
        //一行写完以后，需要重新折过来，从开头在写入 wrireLen-(cb_buffer->buflen-cb_buffer->wroffset)
        memcpy(&ring->_buf[ring->_wroffset], inbuf, ring->_buf_len - ring->_wroffset);
        memcpy(ring->_buf, &inbuf[ring->_buf_len - ring->_wroffset], write_len - (ring->_buf_len - ring->_wroffset));
    }
    //更新写指针
    ring->_wroffset = (write_len + ring->_wroffset) % ring->_buf_len;
    //更新size
    ring->_size += write_len;
    return write_len;
}
int main() 
{
    char writeBuf[15] = { 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };
    char readBuf[15];
    ringBuffer* cb_buffer = NULL;
    cb_buffer = ringbuffer_init(10 * 1024 * 1024);
    int a = ringbuffer_write(cb_buffer, writeBuf, 5);
    printf("write length is %d\n", a);
    printf("size is %d,buflen is %d,content is %s,writeOffset is %d\n", cb_buffer->_size, cb_buffer->_buf_len, cb_buffer->_buf, cb_buffer->_wroffset);
    a = ringbuffer_read(cb_buffer, readBuf, 10);
    printf("read length is %d\n", a);
    printf("size is %d,buflen is %d,content is %s,readOffset is %d\n", cb_buffer->_size, cb_buffer->_buf_len, cb_buffer->_buf, cb_buffer->_rdoffser);
    a = ringbuffer_read(cb_buffer, readBuf, 10);
    printf("read length is %d\n", a);
    a = ringbuffer_write(cb_buffer, writeBuf, 9);
    printf("write length is %d\n", a);
    printf("size is %d,buflen is %d,content is %s,writeOffset is %d\n", cb_buffer->_size, cb_buffer->_buf_len, cb_buffer->_buf, cb_buffer->_wroffset);
    ringbuffer_close(cb_buffer);

    return 0;
    
}