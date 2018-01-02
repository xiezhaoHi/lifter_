#include "clientasio.h"
#include<QDebug>
#include"buffer//usebuffer.h"
#include<QThread>

//数据包 格式
enum enum_PACK_DATA
{
    PACK_BEGIN, //开始
    PACK_TYPE, //数据类型
    PACK_SIZE, //数据大小
    PACK_DATA, //数据
    PACK_END, //标识结尾
    PACK_MAX
};
static char* pack_buff[PACK_MAX] = {"being","type","size","data","end"};

CClientASIO* CClientASIO::m_singleton = NULL;



CClientASIO::CClientASIO (const std::string & hostname, unsigned short port,io_service  * io_service_)
{
    m_io_service = io_service_;
    m_socket     = new tcp::socket(*m_io_service);

    tcp::resolver        resolver(*m_io_service);
    boost::system::error_code ec;
    tcp::resolver::query query(hostname, boost::lexical_cast<std::string, unsigned short>(port));
    tcp::resolver::iterator iter = resolver.resolve(query, ec);
    tcp::resolver::iterator end;

    // pick the first endpoint
    if (iter != end && ec == 0)
    {
        tcp::endpoint endpoint = *iter;



        m_socket->connect(endpoint, ec);
        memset(m_data,0,sizeof(m_data));
        m_socket->async_read_some(boost::asio::buffer(m_data,_RECV_BUF_SIZE_),
            boost::bind(&CClientASIO::OnRecv,this,
            boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
        if (ec)
        {

            return;
        }
    }
}

CClientASIO::~CClientASIO()
{
    delete m_socket;
    delete m_io_service;
}
  void CClientASIO::start(void)
{

}

void CClientASIO::PostSend(const std::string & message)
{
    m_socket->async_write_some(boost::asio::buffer(message.c_str(),message.size()),
        boost::bind(&CClientASIO::OnSend,this,
        boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
}

bool CClientASIO::OnRecv(const boost::system::error_code& error,size_t bytes_transferred)
{
        if(!error)
        {
            std::string strRecv = (char *)m_data;
           // qDebug() <<strRecv;
            //处理数据包
            DealData(QString::fromStdString(strRecv));


            memset(m_data,0,sizeof(m_data));
            m_socket->async_read_some(boost::asio::buffer(m_data,_RECV_BUF_SIZE_),
                boost::bind(&CClientASIO::OnRecv,this,
                boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            OnClose(error);
            return false;
        }
        return true;
}

bool CClientASIO::OnSend(const boost::system::error_code& error,size_t bytes_transferred)
{

    if(error)
    {
        OnClose(error);
        return false;
    }
    return true;
}

void CClientASIO::OnClose(const boost::system::error_code& error)
{
    m_socket->close();
}

//解析 数据包
  void   CClientASIO::DealData(QString const& strData)
  {
     // qDebug() << QThread::currentThreadId() << strData;
        QStringList strlist=strData.split("#");
        //粘包时  list 大小 大于 PACK_MAX  需要分包
        QString strTemp;
        //strBegin,strEnd;
        for(int index =0; index+PACK_MAX <= strlist.size(); index+=PACK_MAX)
        {
            //校验数据包
//            strBegin = QString(pack_buff[PACK_BEGIN]);
//            strEnd   = QString(pack_buff[PACK_END]);
            if(QString(pack_buff[PACK_BEGIN]) == strlist[index+PACK_BEGIN]
                    && QString(pack_buff[PACK_END])== strlist[index+PACK_END]
                )
             {
                strTemp = strlist[index+PACK_DATA];
                if(strlist[index+PACK_SIZE] != QString("%1").arg(strTemp.length()))
                {
                    continue;
                }
                // 分数据
                //qDebug() << "接受r" << strTemp;
                UserBuffer::GetInstance()->PushClientServerQueue(strTemp);

            }
        }
  }
