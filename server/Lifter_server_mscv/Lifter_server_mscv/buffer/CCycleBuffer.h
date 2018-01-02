
#ifndef CCycleBuffer_H
#define CCycleBuffer_H
class CCycleBuffer
{

public:
	/** 
	 *  function ���캯��
	 *  @param   size    ��������СĬ��1M��
	 *					 Ҳ����ͨ��setBufferSize(int size)�������û�������С��
	 *					 ����һ��Ҫ�ڻ�����Ϊ�յ�ǰ���£�����ᶪʧ�������ݡ�
	 *                 true,�ɹ�; false,ʧ��
	 */
    CCycleBuffer(int size = 1024*512);
	virtual ~CCycleBuffer();
	int write(char* buf, int count);
	int read(char* buf, int count);
	
	/** 
	 *  function �������Ƿ�����?
	 *  @return  bool  ����״̬
	 *                 true,��; false,��
	 */
	bool isFull();
	/** 
	 *  function �������Ƿ��?
	 *  @return  bool  ����״̬
	 *                 true,��; false,��

	 */
	bool isEmpty();
	/** 
	 *  function ���û�����Ϊ��
	 *  @return  bool  ����״̬
	 *                 true,��; false,��
	 */
	void setEmpty();
	/** 
	 *  function ���ص�ǰ���������ÿռ�
	 *  @return  int   �������ÿռ��С(byte)
	 */
	int getUsedSize();
	/** 
	 *  function ���ص�ǰ���������ÿռ�
	 *  @return  int   ���ؿ��пռ��С(byte)
	 */
	int getFreeSize();  
	/** 
	 *  function ���ػ���ռ��С
	 *  @return  int   ���ػ���ռ��С
	 */
	int getBufSize()
	{
		return m_nBufSize;
	}
	/** 
	 *  function ���ػ���ռ俪ʼλ��
	 *  @return  int   ���ػ���ռ俪ʼλ��
	 */
	int getStart()
	{
		return m_nReadPos;
	}
	/** 
	 *  function ���ػ���ռ����λ��
	 *  @return  int   ���ػ���ռ����λ��
	 */
	int getEnd()
	{
		return m_nWritePos;
	}
	/** 
	 *  function ���û�������С
	 *  @param   size   �¿��ٻ�������С
	 *  @param   copy   �Ƿ񿽱������������»�����,Ĭ��false,������.
	 *  @return  bool   ����״̬
	 *                  true,��; false,��

	 */
	bool setBufferSize(int size, bool copy = false);
private:
	//�ձ�ʾ��
	bool m_bEmpty;
	//����ʾ��
	bool m_bFull;
	//������ָ��
	char * m_pBuf;
	//��������С
	int m_nBufSize;
	//ʼλ��
	int m_nReadPos;
	//βλ��
	int m_nWritePos;
};
#endif // CCycleBuffer_H
