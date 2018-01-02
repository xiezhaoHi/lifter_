#include "canmodulebuffer.h"
#include <assert.h>
#include <memory.h>
#include <QDebug>

CanModuleBuffer::CanModuleBuffer(int size)

{
    m_nBufSize = size;
    m_nReadPos = 0;
    m_nWritePos = 0;
    m_pBuf = new VCI_CAN_OBJ[m_nBufSize];
    m_bEmpty = true;
    m_bFull = false;
}
CanModuleBuffer::~CanModuleBuffer()
{
    delete[] m_pBuf;
}
void CanModuleBuffer::can_memcpy(VCI_CAN_OBJ * buffDst,int begin,VCI_CAN_OBJ * const &buffSrc,int size)
{




//    VCI_CAN_OBJ T;
//    T = buffDst[0];
//    buffDst[0] = buffSrc[0];
    for(int index = begin, indexSrc = 0; index < begin+size && indexSrc < size; ++index,++indexSrc)
    {
        buffDst[index] = buffSrc[indexSrc];
    }


}

/*
 *	��������  �򻺳���д�����ݣ�����ʵ��д����ֽ���
 */
int CanModuleBuffer::write(VCI_CAN_OBJ* const& buf, const int &count)
{
    if(count <= 0)
        return 0;
    m_bEmpty = false;
    // ���������������ܼ���д��
    if(m_bFull)
    {
        return 0;
    }
    else if(m_nReadPos == m_nWritePos) // ������Ϊ��ʱ
    {
        /*                          == �ڴ�ģ�� ==
                 (empty)             m_nReadPos                (empty)
        |----------------------------------|-----------------------------------------|
                                    m_nWritePos        m_nBufSize
        */
        int leftcount = m_nBufSize - m_nWritePos;
        if(leftcount > count)
        {
            can_memcpy(m_pBuf,m_nWritePos, buf, count);

            m_nWritePos += count;
            m_bFull = (m_nWritePos == m_nReadPos);
            return count;
        }
        else
        {
            can_memcpy(m_pBuf,m_nWritePos, buf, leftcount);
            m_nWritePos = (m_nReadPos > count - leftcount) ? count - leftcount : m_nWritePos;
            can_memcpy(m_pBuf,0, buf + leftcount, m_nWritePos);
            m_bFull = (m_nWritePos == m_nReadPos);
            return leftcount + m_nWritePos;
        }
    }
    else if(m_nReadPos < m_nWritePos) // ��ʣ��ռ��д��
    {
        /*                           == �ڴ�ģ�� ==
            (empty)                 (data)                     (empty)
        |-------------------|----------------------------|---------------------------|
                    m_nReadPos                m_nWritePos       (leftcount)
        */
        // ʣ�໺������С(��д��λ�õ�������β)

        int leftcount = m_nBufSize - m_nWritePos;
        if(leftcount > count)   // ���㹻��ʣ��ռ���
        {
            can_memcpy(m_pBuf,m_nWritePos, buf, count);
            m_nWritePos += count;
            m_bFull = (m_nReadPos == m_nWritePos);
            assert(m_nReadPos <= m_nBufSize);
            assert(m_nWritePos <= m_nBufSize);
            return count;
        }
        else       // ʣ��ռ䲻��
        {
            // �������ʣ��ռ䣬�ٻ�ͷ�ҿռ���
            can_memcpy(m_pBuf,m_nWritePos, buf, leftcount);

            m_nWritePos = (m_nReadPos >= count - leftcount) ? count - leftcount : m_nReadPos;
            can_memcpy(m_pBuf,0, buf + leftcount, m_nWritePos);
            m_bFull = (m_nReadPos == m_nWritePos);
            assert(m_nReadPos <= m_nBufSize);
            assert(m_nWritePos <= m_nBufSize);
            return leftcount + m_nWritePos;
        }
    }
    else
    {
        /*                          == �ڴ�ģ�� ==
            (unread)                 (read)                     (unread)
        |-------------------|----------------------------|---------------------------|
                     m_nWritePos    (leftcount)    m_nReadPos
        */
        int leftcount = m_nReadPos - m_nWritePos;
        if(leftcount >= count)
        {
            // ���㹻��ʣ��ռ���
            can_memcpy(m_pBuf,m_nWritePos, buf, count);
            m_nWritePos += count;
            m_bFull = (m_nReadPos == m_nWritePos);
            assert(m_nReadPos <= m_nBufSize);
            assert(m_nWritePos <= m_nBufSize);
            return count;
        }
        else
        {
            // ʣ��ռ䲻��ʱҪ�������������
            can_memcpy(m_pBuf,m_nWritePos, buf, leftcount);
            m_nWritePos += leftcount;
            bool t = m_bFull = (m_nReadPos == m_nWritePos);
            if(!m_bFull)
                bool t2 = t;
            assert(m_nReadPos <= m_nBufSize);
            assert(m_nWritePos <= m_nBufSize);
            return leftcount;
        }
    }
}
/*
 *	��������  �ӻ����������ݣ�����ʵ�ʶ�ȡ���ֽ���
 */
int CanModuleBuffer::read(VCI_CAN_OBJ *buf, int const &count)
{
    if(count <= 0)
        return 0;
    m_bFull = false;
    if(m_bEmpty)       // �������գ����ܼ�����ȡ����
    {
        return 0;
    }
    else if(m_nReadPos == m_nWritePos)   // ��������ʱ
    {
        /*                          == �ڴ�ģ�� ==
         (data)          m_nReadPos                (data)
        |--------------------------------|--------------------------------------------|
             m_nWritePos         m_nBufSize
        */
        int leftcount = m_nBufSize - m_nReadPos;
        if(leftcount > count)
        {
            can_memcpy(buf, 0,m_pBuf+m_nReadPos,count);
            m_nReadPos += count;
            m_bEmpty = (m_nReadPos == m_nWritePos);
            return count;
        }
        else
        {
            can_memcpy(buf, 0,m_pBuf + m_nReadPos, leftcount);
            m_nReadPos = (m_nWritePos > count - leftcount) ? count - leftcount : m_nWritePos;
            can_memcpy(buf + leftcount,0, m_pBuf, m_nReadPos);
            m_bEmpty = (m_nReadPos == m_nWritePos);
            return leftcount + m_nReadPos;
        }
    }
    else if(m_nReadPos < m_nWritePos)   // дָ����ǰ(δ�����������ӵ�)
    {
        /*                          == �ڴ�ģ�� ==
            (read)                 (unread)                      (read)
        |-------------------|----------------------------|---------------------------|
                    m_nReadPos                m_nWritePos                     m_nBufSize
        */
        int leftcount = m_nWritePos - m_nReadPos;
        int c = (leftcount > count) ? count : leftcount;
        can_memcpy(buf,0, m_pBuf + m_nReadPos, c);
        m_nReadPos += c;
        m_bEmpty = (m_nReadPos == m_nWritePos);
        assert(m_nReadPos <= m_nBufSize);
        assert(m_nWritePos <= m_nBufSize);
        return c;
    }
    else          // ��ָ����ǰ(δ�����ݿ����ǲ����ӵ�)
    {
        /*                          == �ڴ�ģ�� ==
              (unread)                (read)                      (unread)
        |-------------------|----------------------------|---------------------------|
                     m_nWritePos                  m_nReadPos                  m_nBufSize

        */
        int leftcount = m_nBufSize - m_nReadPos;
        if(leftcount > count)   // δ������������ֱ�Ӷ�ȡ����
        {
            can_memcpy(buf,0, m_pBuf + m_nReadPos, count);
            m_nReadPos += count;
            m_bEmpty = (m_nReadPos == m_nWritePos);
            assert(m_nReadPos <= m_nBufSize);
            assert(m_nWritePos <= m_nBufSize);
            return count;
        }
        else       // δ�����������㣬��ص�������ͷ��ʼ��
        {
            can_memcpy(buf,0, m_pBuf + m_nReadPos, leftcount);
            m_nReadPos = (m_nWritePos >= count - leftcount) ? count - leftcount : m_nWritePos;
            can_memcpy(buf + leftcount, 0,m_pBuf, m_nReadPos);
            m_bEmpty = (m_nReadPos == m_nWritePos);
            assert(m_nReadPos <= m_nBufSize);
            assert(m_nWritePos <= m_nBufSize);
            return leftcount + m_nReadPos;
        }
    }
}

bool CanModuleBuffer::isFull()
{
    return m_bFull;
}
bool CanModuleBuffer::isEmpty()
{
    return m_bEmpty;
}

void CanModuleBuffer::setEmpty()
{
    m_nReadPos = 0;
    m_nWritePos = 0;
    m_bEmpty = true;
    m_bFull = false;
}

/**
 * ��ȡ��������Ч���ݳ���
 */
int CanModuleBuffer::getUsedSize()
{
    if(m_bEmpty)
    {
        return 0;
    }
    else if(m_bFull)
    {
        return m_nBufSize;
    }
    else if(m_nReadPos < m_nWritePos)
    {
        return m_nWritePos - m_nReadPos;
    }
    else
    {
        return m_nBufSize - m_nReadPos + m_nWritePos;
    }
}
/**
 * ��ȡ���������пռ����ݳ���
 */
int CanModuleBuffer::getFreeSize()
{
    if(m_bEmpty)
    {
        return m_nBufSize;
    }
    else if(m_bFull)
    {
        return 0;
    }
    else if(m_nReadPos > m_nWritePos)
    {
        return m_nReadPos - m_nWritePos;
    }
    else
    {
        return m_nBufSize - m_nWritePos + m_nReadPos;
    }
}

 /*
  *	@brief  ���û�������С
  *			  ��û�л�����,���½�������;�����л�����,���½�������������copy��־�����о����ݿ���
  */
 bool CanModuleBuffer::setBufferSize(int size, bool copy)
 {
     errno_t err;
     //�����пռ䣬�ͷŵ��������»������������п���������
     if(m_pBuf!=0)
     {

         //�¿��ٻ�����
         VCI_CAN_OBJ* buf = new VCI_CAN_OBJ[size];
         //����Ҫ����
         if(copy)
         {
             //����������
            can_memcpy(buf, size, m_pBuf, m_nBufSize);
             if(err!=0)
                 qDebug("CanModuleBuffer::setBufferSize,���û��������������������»��������������ǿ��ٿռ�̫С��");
         }
         //�ͷžɻ�����
         delete m_pBuf;
         //ָ���»�����
         m_pBuf = buf;
         //���û�������С
         m_nBufSize = size;
     }
     //��û�л��������򿪱ٻ�����
     else
     {
         //�¿��ٻ�����
         m_pBuf = new VCI_CAN_OBJ[size];
         //��ֵ��������С
         m_nBufSize = size;
     }

     if(m_pBuf!=0)
        return true;
     else
        return false;
 }
