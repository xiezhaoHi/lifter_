#ifndef CANMODULEBUFFER_H
#define CANMODULEBUFFER_H

#include"ControlCAN/ControlCAN.h"

class CanModuleBuffer
{

public:
    /**
     *  function 构造函数
     *  @param   size    缓冲区大小默认1M，
     *					 也可以通过setBufferSize(int size)进行重置缓冲区大小，
     *					 但是一定要在缓冲区为空的前提下，否则会丢失已有数据。
     *                 true,成功; false,失败
     */
    CanModuleBuffer(int size = 1024*1024);
    virtual ~CanModuleBuffer();
    int write(VCI_CAN_OBJ * const &buf, int const &count);
    int read(VCI_CAN_OBJ* buf, int const &count);
    void can_memcpy(VCI_CAN_OBJ *buffDst, int begin, VCI_CAN_OBJ * const &buffSrc, int size);
    /**
     *  function 缓冲区是否已满?
     *  @return  bool  返回状态
     *                 true,真; false,假
     */
    bool isFull();
    /**
     *  function 缓冲区是否空?
     *  @return  bool  返回状态
     *                 true,真; false,假

     */
    bool isEmpty();
    /**
     *  function 设置缓冲区为空
     *  @return  bool  返回状态
     *                 true,真; false,假
     */
    void setEmpty();
    /**
     *  function 返回当前缓冲区已用空间
     *  @return  int   返回已用空间大小(byte)
     */
    int getUsedSize();
    /**
     *  function 返回当前缓冲区可用空间
     *  @return  int   返回空闲空间大小(byte)
     */
    int getFreeSize();
    /**
     *  function 返回缓冲空间大小
     *  @return  int   返回缓冲空间大小
     */
    int getBufSize()
    {
        return m_nBufSize;
    }
    /**
     *  function 返回缓冲空间开始位置
     *  @return  int   返回缓冲空间开始位置
     */
    int getStart()
    {
        return m_nReadPos;
    }
    /**
     *  function 返回缓冲空间结束位置
     *  @return  int   返回缓冲空间结束位置
     */
    int getEnd()
    {
        return m_nWritePos;
    }
    /**
     *  function 设置缓冲区大小
     *  @param   size   新开辟缓冲区大小
     *  @param   copy   是否拷贝已有数据至新缓冲区,默认false,不拷贝.
     *  @return  bool   返回状态
     *                  true,真; false,假

     */
    bool setBufferSize(int size, bool copy = false);
private:
    //空标示符
    bool m_bEmpty;
    //满标示符
    bool m_bFull;
    //缓冲区指针
    VCI_CAN_OBJ * m_pBuf;
    //缓冲区大小
    int m_nBufSize;
    //始位置
    int m_nReadPos;
    //尾位置
    int m_nWritePos;
};


#endif // CANMODULEBUFFER_H
