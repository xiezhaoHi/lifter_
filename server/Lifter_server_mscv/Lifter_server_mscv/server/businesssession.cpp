#include "businesssession.h"

#include"config/config.h"
#include<QDebug>
#include<QTime>
#include<QDomDocument>
#include<QtMath>
#include<QStringList>
#include<QThread>
#include<QDomComment>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
//制动标识 字符串
#define  BMQ_ZD     "20010003"

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
const char* pack_buff[PACK_MAX] = {"being","type","size","data","end"};

BusinessSession* BusinessSession::m_singleton = NULL;

//大小端检测  返回true 为 大端 (低地址存高字节数据) 
//返回false 为 小端(低地址存 低字节数据)
bool checkCpu()
{

	union w
	{
		int a;
		char b;
	}c;
	c.a = 1;
	int b = c.b;
	return (c.b == 1);
}

BusinessSession::BusinessSession()
{
   memset(m_cgq_buff,0,MAX_LEN);
   m_index = 0;

   //大小端记录 true为大端
   m_bigFlag = !checkCpu();
}
 BusinessSession::~BusinessSession()
 {
  QMap<QString,char*>::iterator begin = m_bmqMapBuff.begin(),end = m_bmqMapBuff.end();
  for(;begin != end; ++begin)
  {
        if(begin.value())
        {
            delete begin.value();
            begin.value() = NULL;
        }
  }

  //m_canModule_buffer_map
  QMap<QString,CanModuleBuffer*>::iterator begin_can = m_canModule_buffer_map.begin()
          ,end_can = m_canModule_buffer_map.end();
  for(;begin_can != end_can; ++begin_can)
  {
        if(begin_can.value())
        {
            delete begin_can.value();
            begin_can.value() = NULL;
        }
  }
  //m_userbuffer_map
  QMap<QString,UserBuffer*>::iterator begin_cgq = m_userbuffer_map.begin()
          ,end_cgq = m_userbuffer_map.end();
  for(;begin_cgq != end_cgq; ++begin_cgq)
  {
        if(begin_cgq.value())
        {
            delete begin_cgq.value();
            begin_cgq.value() = NULL;
        }
  }
  //m_experimentThread_map
  QMap<QString,ExperimentThread*>::iterator begin_experiment = m_experimentThread_map.begin()
          ,end_experiment = m_experimentThread_map.end();
  for(;begin_experiment != end_experiment; ++begin_experiment)
  {
        if(begin_experiment.value())
        {
            delete begin_experiment.value();
            begin_experiment.value() = NULL;
        }
  }


 }

 BusinessSession* BusinessSession::GetInstance(void)
{
     if(NULL == m_singleton)
         m_singleton = new BusinessSession();
     return m_singleton;

}

 /*服务器处理数据的统一接口
  * strIp 设备IP
*/
 QString  BusinessSession::InterfaceFun(QString strIp,char* data, int len,int flag)
 {

     QString strRet("");
     QString strTemp("");
     int type = Config::GetInstance()->GetDeviceFlag(strIp);
     QString strID = Config::GetInstance()->GetDeviceID(strIp);
	 QTime t;
   switch(type)
   {
   case device_cgq: //传感器 数据
	   t.start();
       strTemp =  DealCgqData(strIp,data,len);
	    qDebug() << QStringLiteral("cgq3_%1").arg(t.elapsed());
       break;
   case device_bmq: //编码器数据
       strTemp = DealBmqData(strIp,strID,data,len);
	  
       break;
   case device_jdq: //继电器
      strTemp =  DealJdqData(strIp,data,len);
       break;
   case device_dydl: //电源
	   strTemp = DealDydlData(strIp, data, len,flag);
   }
   if(!strTemp.isEmpty())
   {
       strRet += QString("<?xml version='1.0' encoding ='utf8'?>");
       strRet += QString("<body type = '%1' belongs='%2' >").arg(type)
               .arg(Config::GetInstance()->GetLifterIDByDeviceIp(strIp));
       strRet +=strTemp;
       strRet += QString("</body>");
       strRet = QString("being#%1#%2#").arg(type).arg(strRet.length())+strRet+QString("#end#");

       /*
        * 更新发送缓冲
        */
       UserBuffer::GetInstance()->WriteDeviceToClientMap(strIp,strRet);
    }

//   QDateTime current_date_time = QDateTime::currentDateTime();
//   QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
//    qDebug() << current_time << "##" <<  type << "##" << strRet;
   return strRet;
 }

QString  BusinessSession::DealCgqData(QString const& strIp,char* data,int len)
{
    QString strRet(""); // 返回数据
    //1.存入缓冲区
//     if(22 != len)
//         return strRet;
    QString strID = Config::GetInstance()->GetDeviceID(strIp);
    GetDeviceUserBuffer(strID,device_cgq)->Write_buffer(device_cgq,data,len);

    const int constlen = 22;
    char buff[constlen] = {0} ;
//    //找第一个0x55开头
//  //过滤算法说明
//  /*
//   1 从缓冲区开头遍历 获取第一个0x55
//  2  再获取后面一位 & 0x5F 判断是否是0x5开头
//   3  再判断后面一位是不是0x55 如果是则选取其为头 再从1 检测
//*/
  bool flag = false;
  //bool zeroFlag = true; //是否获取一个缓冲区数据
 // QString strTemp; //表示数据的16进制的字符串

  while(GetDeviceUserBuffer(strID,device_cgq)->TestRead_buffer())
  {
        do
      {
            GetDeviceUserBuffer(strID,device_cgq)->Read_buffer(device_cgq,&buff[0],1);
       }while(0x55 != buff[0]);


          GetDeviceUserBuffer(strID,device_cgq)->Read_buffer(device_cgq,&buff[1],1);
         if((buff[1]   == 0x51))
            {
                 GetDeviceUserBuffer(strID,device_cgq)->Read_buffer(device_cgq,&buff[2],1);
                if(0x55 != buff[2]) //3cv,m,m,
                {

                        GetDeviceUserBuffer(strID,device_cgq)->Read_buffer(device_cgq,&buff[3],19);
                        char sum1 = 0 ,sum2;
                        sum1 = buff[0] + buff[1] + buff[3]+ buff[2]+ buff[5]+ buff[4]+ buff[7]+ buff[6]+ buff[9]+ buff[8];
                        sum2 = buff[11] + buff[12] + buff[13]+ buff[14]+ buff[15]+ buff[16]+ buff[17]+ buff[18]+ buff[19]+ buff[20];

                        if(buff[10] == sum1 && buff[21] == sum2)
                        {

                                flag = true;
                                break;
                           //qDebug() <<"transfNum" <<   ++UserBuffer::transfNum;
                        }

                }

        }

}

  if(!flag)
      return strRet;

    QString jdX,jdY,jdZ,jsdX,jsdY,jsdZ;
//  QString str;
//  for(int index =0 ;index <22; ++index)
//    str +=QString(" ") +  QString("%1").arg(buff[index]&0xFF,2,16,QLatin1Char('0'));
//  qDebug() << "show" << str;

        QString strSql;

        int index = 0;


//        0x55
//        0x51
//        AxL
//        AxH
//        AyL
//        AyH
//        AzL
//        AzH
//        TL
//        TH
//        SUM
//        ax=((AxH<<8)|AxL)/32768*16g(g为重力加速度，可取9.8m/s2)
//        ay=((AyH<<8)|AyL)/32768*16g(g为重力加速度，可取9.8m/s2)
//        az=((AzH<<8)|AzL)/32768*16g(g为重力加速度，可取9.8m/s2)
        // 0x51: //加速度

          //单位g  g可以取9.8m/s

        jsdX = QString::number((((short)buff[index+3]<<8)|(0xff&buff[index+2]))/32768.0*16,  'f', 4);
        jsdY = QString::number((((short)buff[index+5]<<8)|(0xff&buff[index+4]))/32768.0*16,   'f', 4);
        jsdZ = QString::number((((short)buff[index+7]<<8)|(0xff&buff[index+6]))/32768.0*16,  'f', 4);

           strRet = QString("<jsd>")
                   +QString("<x>")+ jsdX +"</x>"
                    +QString("<y>")+ jsdY +"</y>"
                    +QString("<z>")+ jsdZ +"</z>"
                    +QString("</jsd>");

//            0x55
//            0x53
//            RollL
//            RollH
//            PitchL
//            PitchH
//            YawL
//            YawH
//            TL
//            TH
//            SUM
            //偏航角（z轴）Yaw=((YawH<<8)|YawL)/32768*180(°)
           index = 11;


           jdX = QString::number(( short)((buff[index+3]<<8)|(0xff&buff[index+2]))/32768.0*180,   'f', 3) ;
           jdY = QString::number(( short)((buff[index+5]<<8)|(0xff&buff[index+4]))/32768.0*180,   'f', 3) ;
           jdZ = QString::number(( short)((buff[index+7]<<8)|(0xff&buff[index+6]))/32768.0*180,    'f', 3);


            strRet +=QString( "<jd>")        
                    +QString("<x>")+jdX +"</x>"
                    +QString("<y>")+jdY  +"</y>"
                    +QString("<z>")+jdZ+"</z>"
                     +QString("</jd>");


            QDateTime current_date_time = QDateTime::currentDateTime();
            QString strDate = current_date_time.toString("yyyy/MM/dd");
            QString current_time = current_date_time.toString("hh:mm:ss.zzz");

            strSql =   QString("('%1','%2','%3','%4','%5','%6','%7','%8','%9')")
                    .arg(strID)
                    .arg(strDate)
                    .arg(current_time)
                    .arg(jdX)
                    .arg(jdY)
                    .arg(jdZ)
                    .arg(jsdX)
                    .arg(jsdY)
                    .arg(jsdZ);


     UserBuffer::GetInstance()->PushDatabaseQueue(device_cgq,strSql);
     QString strDhData;
     strDhData = QString("<data_type ID='%4'><data_speed>%1</data_speed><data_levelness_x>%2</data_levelness_x>\
    <data_levelness_y>%3</data_levelness_y></data_type>")
             .arg(jsdZ)
             .arg(jdX)
             .arg(jdY)
             .arg(strID);

     //存储 动画 客户端 数据包
	UserBuffer::GetInstance()->WriteDeviceToDhData(strIp,strDhData);
	QString strTestTime = QString("cgq_start_%1").arg(QTime::currentTime().toString(QString("HH:mm:ss:ms")));

    return strRet;

}

/*处理旋转编码器信息
* strID 设备ID 每个设备 对应一个ip 对应唯一设备ID
*
*/
QString BusinessSession::DealBmqData(QString const& strIp,QString const& strID,char* data,int len)
{
    assert(NULL != data);
    QString strRet("");
    /*
     * 如果是编码器 设置命令的先后顺序
     */



    //1.把两条数据组成一包数据
        if(0x03 == (0xff&data[1]) && BMQPACKAGE_ONESIZE == len) //表示第一条指令值返回了，再读取第二条指令的值
        {
            setBmqFirst(strIp, false);
            setBmqMapBuff(strIp,data,len);
            //setBmqDataLen(strIp, len);

            return strRet;
        }
        if(0x04 == (0xff&data[1]) && BMQPACKAGE_TWOSIZE == len) //表示第二条指令值返回了，再读取第一条指令的值
        {
            setBmqFirst(strIp, true);
            setBmqMapBuff(strIp,data,len);

        }

   //2.crc16 校验

        if(getBmqDataLen(strIp) != BMQBUFF_SIZE)
        {
           // memset((char*)m_bmqMapBuff,0,BMQBUFF_SIZE);
            setBmqMapBuff(strIp,0,BMQBUFF_SIZE);
            setBmqFirst(strIp, true);
            return "";
        }
    char* m_bmqBuff = getBmqMapBuff(strIp);
    if(NULL == m_bmqBuff)
    {
        //memset(m_bmqMapBuff,0,BMQBUFF_SIZE);
         setBmqMapBuff(strIp,0,BMQBUFF_SIZE);

        setBmqFirst(strIp, true);

        return "";
    }
    int dataLen = getBmqDataLen(strIp);
   unsigned short mycrcJym =  usMBCRC16((unsigned char*)data,len-2) ;
   unsigned short mycrcJym2 = usMBCRC16((unsigned char*)m_bmqBuff,dataLen-2-len);
   unsigned short dataJym = (unsigned short)(data[len-1] << 8) | (0xff&data[len-2]);
  unsigned short dataJym2 = (unsigned short)(m_bmqBuff[dataLen-1-len] << 8) | (0xff&m_bmqBuff[dataLen-2-len]);
    if(mycrcJym == dataJym&& mycrcJym2 == dataJym2 ) //数据校验通过
    {
        //        方向：80 00 ：最高位为1 所以为反向 如果是00 00 则为正向
        //                  角度：      39 39 ： 146.49°相对角度，在升降机系统中忽略
        //                  转速：            00 00 17 70 ：6000 即 60.00rpm
                QString strDir; //方向 角度 转速

                if(0x80 == (0xff&m_bmqBuff[3]))
                    strDir = "1";//"正向";
                else
                    strDir = "0";//"反向";


                 double shortJd = (unsigned short) m_bmqBuff[5]<<8 | (0xff&m_bmqBuff[6]);
                double shortZs = ( m_bmqBuff[7]<<24 | m_bmqBuff[8] << 16 | m_bmqBuff[9] << 8 | (0xff&m_bmqBuff[10]));


        //       1、把00 00 23 93转换为十进制为：00 00 35 147
        //           2、计数值 = （（00*256+00）*256+35）*256+147 =9107
                unsigned long jsz = (( ((0xff&m_bmqBuff[16])*256 +  (0xff&m_bmqBuff[17]))  * 256 + (0xff&m_bmqBuff[18])))*256 + (0xff&m_bmqBuff[19]);

        //       我们设定计数值为x。设定编码器一圈的刻度是p，套在编码器1上的轮子直径为d，轮子做过的位移（路程）为y，则位移（路程）的计算方式为：
        // y = x*轮子周长/(2*p)


                QDateTime current_date_time = QDateTime::currentDateTime();
                QString strDate = current_date_time.toString("yyyy/MM/dd");
                QString current_time = current_date_time.toString("hh:mm:ss.zzz");
                //方向 角度 转速  计数值


              QString strSql = QString("('%1','%2','%3','%4','%5','%6','%7')")
                       .arg(strID)
                       .arg(strDate)
                       .arg(current_time)
                       .arg(strDir)
                       .arg(shortJd)
                       .arg(shortZs)
                       .arg(jsz)
                       ;

                UserBuffer::GetInstance()->SetBmqNewestJsz(strIp,jsz);

                UserBuffer::GetInstance()->PushDatabaseQueue(device_bmq,strSql);



                //制动距离
                QString strZdjl;
                 BMQCOf data = Config::GetInstance()->GetBmqConfig(strIp);
                int oldJsz = UserBuffer::GetInstance()->GetBmqBeginZdJsz(strIp);
                if(-1 != oldJsz)
                {
                    if(0 == shortZs) //制动 完成
                    {

                        if(data.m_d > 0 && data.m_p >0)
                        {
                            if(jsz>oldJsz)
                                strZdjl = QString("%1").arg((jsz - oldJsz)*2*3.14*data.m_d/(2*data.m_p));
                            else
                                 strZdjl = QString("%1").arg( (oldJsz-jsz)*2*3.14*data.m_d/(2*data.m_p) );
                        }
                    }
                }
                QString strDhData = QString("<data_type ID='%5'>\
                                             <data_dir>%1</data_dir>\
                                             <data_braking_length>%2</data_braking_length>\
                                             <data_real_pos>%3</data_real_pos>\
                                             <data_running_speed>%4</data_running_speed>\
                                           </data_type>")
                                             .arg(strDir)
                                             .arg(strZdjl)
                                             .arg(jsz*2*3.14*data.m_d/(2*data.m_p))
                                             .arg(shortZs*data.m_d*2*3.14/60.0)
                                             .arg(strID)
                                             ;
                        //存储 动画 客户端 数据包
                        UserBuffer::GetInstance()->WriteDeviceToDhData(strIp,strDhData);



                /*
                 * 2017/6/19
                 * 增加获取到数据的时间戳 计算 速度
                 */
                strRet = QString("<dir>%1</dir>").arg(strDir)
                        + QString("<jd>%1</jd>").arg(shortJd)
                        + QString("<zs>%1</zs>").arg(shortZs)
                        +QString("<jsz>%1</jsz>").arg(jsz)
                        +QString("<time>%1</time>").arg(current_date_time.toTime_t())
                        +QString("<zdjl>%1</zdjl>").arg(strZdjl);
    }
	else
	{
		QString strTemp;
		for (int index = 0; index < len; ++index)
		{
			strTemp += QString("%1").arg(data[index], 2, 16, QChar('0'));
		}
		m_lastErr = QStringLiteral("数据有误,CRC校验错误!-") + strTemp;
		strRet = "";
	}
    setBmqMapBuff(strIp,0,BMQBUFF_SIZE);
    setBmqFirst(strIp, true);


    return strRet;
}


static const unsigned char aucCRCHi[] = {

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40

};

static const unsigned char aucCRCLo[] = {

    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,

0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,

    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,

    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,

    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,

    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,

    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,

    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,

    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,

    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,

    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,

    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,

    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,

    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,

    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,

    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,

    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,

    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,

    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,

    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,

    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,

    0x41, 0x81, 0x80, 0x40

};

unsigned short BusinessSession::usMBCRC16( unsigned char * pucFrame, unsigned short usLen )
{
    unsigned char ucCRCHi = 0xFF;

    unsigned char ucCRCLo = 0xFF;

    int iIndex;

    while( usLen-- )
    {

        iIndex = ucCRCLo ^ *( pucFrame++ );

        ucCRCLo = ( unsigned char )( ucCRCHi ^ aucCRCHi[iIndex] );

        ucCRCHi = aucCRCLo[iIndex];

    }

    return ( unsigned short )( ucCRCHi << 8 | ucCRCLo );

}
/*m_bmqFirst*/
bool  BusinessSession::getBmqFirst(const QString &strIp)
{
    if(!m_bmqFirst.contains(strIp))
        m_bmqFirst[strIp] = true;
    return m_bmqFirst[strIp];

}

/*m_bmqFirst*/
void  BusinessSession::setBmqFirst(QString const& strIp,bool flag)
{

        m_bmqFirst[strIp] = flag;
}

/*设置每转大小*/
void BusinessSession::setBmqSize(const QString &strIp, int size)
{

      m_mz_size[strIp] = size;
}
/*获取每转大小*/
int BusinessSession::getBmqSize(const QString &strIp)
{
    if(!m_mz_size.contains(strIp))
        m_mz_size[strIp] = 1;
    return m_mz_size[strIp];
}
/*设置每转脉冲  刻度*/
void BusinessSession::setBmqKd(const QString &strIp, int size)
{
      m_mz_kd[strIp] = size;
}

/*获取每转脉冲  刻度*/
int BusinessSession::getBmqKd(const QString &strIp)
{
    if(!m_mz_kd.contains(strIp))
        m_mz_kd[strIp] = 1;
    return m_mz_kd[strIp];
}

/*设置指令长度*/
void BusinessSession::setBmqDataLen(QString const& strIp,int size)
{
      m_bmqDataLen[strIp] = size;
}

/*指令长度*/
int BusinessSession::getBmqDataLen(QString const& strIp)
{
    if(!m_bmqDataLen.contains(strIp))
        m_bmqDataLen[strIp] = 0;
    return m_bmqDataLen[strIp];
}
/*设置m_bmqMapBuff*/
void BusinessSession::setBmqMapBuff(QString const& strIp,char* buff,int size)
{
    if(!m_bmqMapBuff.contains(strIp))
        m_bmqMapBuff[strIp] = new char[BMQBUFF_SIZE];
    if(BMQBUFF_SIZE == size)
    {
        memset(m_bmqMapBuff[strIp],0,size);
        setBmqDataLen(strIp,0);
        return;
    }
    int buffSize = getBmqDataLen(strIp);
    memcpy(m_bmqMapBuff[strIp]+buffSize,buff,size);
    setBmqDataLen(strIp,buffSize+size);
}

/*m_bmqMapBuff*/
char* BusinessSession::getBmqMapBuff(QString const& strIp)
{
    if(m_bmqMapBuff.contains(strIp))
        return m_bmqMapBuff[strIp];
    return NULL;
}

/*
*
*
* 处理继电器数据
*
*
*
*/

QString BusinessSession::DealJdqData(QString strIp,char* data,int len)
{
    QString str ;
    QString strRet;
    if(0xc0 != (0xff&data[2]))
    {
    for(int index = 0;index < len; ++index)
        str += QString("%1").arg(data[index]&0xFF,2,16,QLatin1Char('0'));
    //qDebug() << "all" <<str;
    strRet = QString("<RET>ok</RET>");
   return strRet;
   }
    str = "";
    if(len >device_jdq_data_len) //当包数据大于 此值时 分包处理
    {
        for(int index= 0; index<=len-device_jdq_data_len-3; index+=3) //-3 是控制命令 返回数据 0x4f4b21 的长度
        {
            if(data[0] == 0x4f
                    &&data[1] == 0x4b
                    &&data[2] == 0x21)
            {

                data = data+3; //数据包 始指针右移3位
            }

        }

    }
    if(len < device_jdq_data_len)
    {
        QString strData;
        for(int index = 0; index < len; ++ index)
         {
           strData += QString("%1").arg(data[index]&0xFF,2,16,QLatin1Char('0'));
         }

        return strRet;
    }
    unsigned char  chSum1 = 0, chSum2 = 0;// 校验1 校验2
     for(int index =2; index < len -2; ++index)
     {
        chSum1 += 0xff&data[index];
     }
     chSum2 = chSum1+ chSum1;
     unsigned char t1 =  0xff&data[len-2];
     unsigned char t2 = 0xff&data[len-1];
    if(chSum1 ==t1 && chSum2 == t2)
    {

        short ret  = (data[4] << 8)| (0xff&data[5]);
        unsigned short uret = ret & 0xffff;

        QString strConfig = Config::GetInstance()->GetDeviceConfig(Config::GetInstance()->GetDeviceID(strIp));

       QDomDocument doc;
       QString error;
        int row = 0, column = 0;
       if(!doc.setContent(strConfig, false, &error, &row, &column))
           return strRet;
       QDomElement root = doc.firstChildElement();
       QString strRootID = root.attribute(QString("ID"));
       root = root.firstChildElement(QString("DI_DATA"));
       root = root.firstChildElement();
       while(!root.isNull())
       {
           int numID = root.attribute(QString("ID")).toInt();
           QString strText = root.attribute(QString("LINK"));
           int q = qPow(2,numID-1);
           q = q & uret;
           strRet += QString("<DI ID='%1' LINK='%2'>%3</DI>").arg(
                                                            QString("%1").arg(numID)
                                                                  ,strText
                                                                ,QString("%1").arg(q));
           /*
            * 制动时
            */
           if(strText == BMQ_ZD && 1 == q)
           {
               QString strBmqIp = Config::GetInstance()->GetCgqToBmqIp(strIp);
                UserBuffer::GetInstance()->SetBmqBeginZdJsz(strBmqIp
                                                            ,UserBuffer::GetInstance()->GetBmqNewestJsz(strBmqIp));
           }


            root = root.nextSiblingElement();
        }

       //往 数据库 缓冲区 丢数据
       QDateTime current_date_time = QDateTime::currentDateTime();
       QString strDate = current_date_time.toString("yyyy/MM/dd");
       QString current_time = current_date_time.toString("hh:mm:ss.zzz");


      QString strSql =   QString("('%1','%2','%3',\"%4\")")
               .arg(strRootID)
               .arg(strDate)
               .arg(current_time)
               .arg(strRet)
               ; UserBuffer::GetInstance()->PushDatabaseQueue(device_jdq,strSql);

      QString strDhData = QString("<data_type ID='%1'>%2</data_type>").arg(strRootID).arg(strRet);

      //存储 动画 客户端 数据包
      UserBuffer::GetInstance()->WriteDeviceToDhData(strIp,strDhData);


    }
    //qDebug() << strRet;
    return strRet;
}

/*
* 电源 电压 电流 功率 数据解析
* 协议详见文档 : 程控变频电源ModBus协议（三相V1）.doc
*/
QString BusinessSession::DealDydlData(QString strIp, char* data, int len, int flag)
{
	QString strRet;
	int runFlag=0; //电源运行状态 0正常 其他异常
	QString strID = Config::GetInstance()->GetDeviceID(strIp);
	short dataFlag = -1; //0 电压 1电流 2频率
	unsigned int tempData[DY_DATA3_max] = { 0 }; //高字节和低字节数据  高地址  低地址
	//crc校验
	unsigned short mycrcJym = usMBCRC16((unsigned char*)data, len - 2);

	unsigned short dataJym = (unsigned short)(data[len - 1] << 8) | (0xff & data[len - 2]);
	if (mycrcJym == dataJym) //CRC校验通过 数据有效
	{
		//03 功能码 读取寄存器值
		if (data[DY_RES_1] == DY_FUNCODE_03)
		{
			//电源 电压数据
			if (flag == DY_DATA_FLAG_DY)
			{
				//数据 字节数
				const unsigned int indexFlag = 2;
				int dataNum = data[indexFlag]/2; //两个字节标识一个完整的数据
				
				for (int index=1,ind=0; index <= dataNum && ind<DY_DATA3_max; ++index,++ind)
				{
					tempData[ind] = (unsigned short)(data[indexFlag + index++]<<8 
						| (0xff & data[indexFlag + index])); //高字节数据
				}
				dataFlag = 0;
			}
			//电源 电流数据
			if (flag == DY_DATA_FLAG_DL)
			{
				//数据 字节数
				const unsigned int indexFlag = 2;
				int dataNum = data[indexFlag] / 4; //四个字节标识一个完整的数据
				
				unsigned short tempDataH,tempDataL;//  高位、低位排列数据 
				for (int index = 1, ind = 0; index <= dataNum && ind<DY_DATA3_max; ++index, ++ind)
				{
					tempDataL = (unsigned short)(data[indexFlag + index++] << 8 
						| (0xff & data[indexFlag + index++])) ; //高字节数据
					tempDataH = (unsigned short)(data[indexFlag + index++] << 8
						| (0xff & data[indexFlag + index])); //高字节数据
					tempData[ind] = (unsigned int)( tempDataH << 8 | 0xff&tempDataL);
				}
				dataFlag = 1;
			}
			//电源 功率数据
			if (flag = DY_DATA_FLAG_GL)
			{
				//数据 字节数
				const unsigned int indexFlag = 2;
				int dataNum = data[indexFlag] / 4; //四个字节标识一个完整的数据
			
				unsigned short tempDataH, tempDataL;//  高位、低位排列数据 
				for (int index = 1, ind = 0; index <= dataNum && ind<DY_DATA3_max; ++index, ++ind)
				{
					tempDataL = (unsigned short)(data[indexFlag + index++] << 8 
						| (0xff & data[indexFlag + index++])); //高字节数据
					tempDataH = (unsigned short)(data[indexFlag + index++] << 8 
						| (0xff & data[indexFlag + index])); //高字节数据
					tempData[ind] = (unsigned int)(tempDataH << 8 | 0xff & tempDataL);
				}
				dataFlag = 2;
			}

		}
		//06 功能码 写取寄存器值
		if (data[DY_RES_1] == DY_FUNCODE_06)
		{
			dataFlag = -1;
		}

		//异常
		if (data[DY_RES_1] >= DY_FUNCODE_80)
		{
			runFlag = data[DY_RES_1+1];
		}
		
		strRet = QString("<data_a>%1</data_a><data_b>%2</data_b><data_c>%3</data_c>< state>%4</state>")
			.arg(tempData[DY_DATA3_A]).arg(tempData[DY_DATA3_B]).arg(tempData[DY_DATA3_C])
			.arg(runFlag);


		

		//标识 数据为 采集数据
		if (dataFlag != -1)
		{
			//往 数据库 缓冲区 丢数据
			QDateTime current_date_time = QDateTime::currentDateTime();
			QString strDate = current_date_time.toString("yyyy/MM/dd");
			QString current_time = current_date_time.toString("hh:mm:ss.zzz");
			QString strSql;
			strSql = QString("('%1','%2','%3','%4','%5','%6')")
				.arg(strID)
				.arg(strDate)
				.arg(current_time)
				.arg(tempData[DY_DATA3_A])
				.arg(tempData[DY_DATA3_B])
				.arg(tempData[DY_DATA3_C]);

			UserBuffer::GetInstance()->PushDatabaseQueue(device_dydl, strSql);
			
			QString strDhData = QString("<data_type ID='%1'>%2</data_type>")
				.arg(strID).arg(strRet);

			//存储 动画 客户端 数据包
			UserBuffer::GetInstance()->WriteDeviceToDhData(strIp, strDhData);
		}
		
	}
	else
	{
		QString strTemp;
		for (int index = 0; index < len; ++index)
		{
			strTemp += QString("%1").arg(data[index], 2, 16, QChar('0'));
		}
		m_lastErr = QStringLiteral("数据有误,CRC校验错误!-")+ strTemp;
	}
	return strRet;
}

/*
 * 开始执行实验线程
 */
void BusinessSession::StartExperimentThread(QString const& lifterID,experimentData const& strData)
{
    if(!m_experimentThread_map.contains(lifterID))
        m_experimentThread_map[lifterID] = new ExperimentThread(lifterID);
    m_experimentThread_map[lifterID]->SetData(strData);
    m_experimentThread_map[lifterID]->start();

}

/*
 * 获取继电器指令
 * 参数说明: strBelongs 客户端所属电梯
 * strID 控制按钮标识
 * strDeviceIp 返回设备IP
 * buff 返回控制命令数据
 * len 返回命令长度
 * packCount 数据包个数
 */
bool    BusinessSession::GetJdqControlOrder(JDQ_DEAL_DATA & dealData
                                            , QString & strDeviceIp
                                            ,char* buff
                                            ,int & len
                                            , int packCount)
{

	QString const strBelongs = dealData.m_strBelongs;
	QString const strID = dealData.m_strID;
	QString const strRec = dealData.m_strRec;
	int  openFlag = dealData.m_openFlag;
	//一直保持 状态
    char controlBuf[] = {(char)0xcc,(char)0xdd,(char)0xa1,(char)0x01
                         ,(char)0xff,(char)0xff,(char)0x00,(char)0x00,(char)0x9e,(char)0x3c};
   
	//点动时
	if (openFlag == 1)
	{
		controlBuf[2] = (char)0x33;
	}
	len = 10;

    /* 0 表示 设备绑定的DO 复位
     * 1 表示设备绑定的DO  闭合
     *
     */

    //获取 设备绑定的 DO
    QString strDO = Config::GetInstance()->GetDOByDeviceID(strBelongs,strID);
    QString strIp = Config::GetInstance()->GetDeviceIpByID(strBelongs,strID);
    strDeviceIp = strIp;
    if(strDO.isEmpty() || strIp.isEmpty())
    {
        m_lastErr = QString("%1该设备绑定DO失败!ip=%2,do=%3").arg(strID).arg(strIp).arg(strDO);

        return false;
    }
    int iDo = strDO.toInt();
	union MyUnion
	{
		short t;
		char ch[2];
	}my;
	my.t = qPow(2,iDo-1);
    


    if(QString("0") == strRec)
    {
            //CC DD A1 01 FF FF FF FF 9E 3C
        controlBuf[4] = (char)0x00 ;
        controlBuf[5] = (char)0x00 ;
		//大端
		if (m_bigFlag)
		{
			controlBuf[6] = my.ch[0];
			controlBuf[7] = my.ch[1];
		}
		else
		{
			controlBuf[6] = my.ch[1];
			controlBuf[7] = my.ch[0];
		}
    }
    else
    {
         controlBuf[4] = (char)0xff ;
         controlBuf[5] = (char)0xff;
		 //大端
		 if (m_bigFlag)
		 {
			 controlBuf[6] = my.ch[0];
			 controlBuf[7] = my.ch[1];
		 }
		 else
		 {
			 controlBuf[6] = my.ch[1];
			 controlBuf[7] = my.ch[0];
		 }
    }
    char chTemp = 0;
    //校验 除去 帧头 16位  剩下的 6 个 8位 数字相加
    for(int index =2; index < 8; ++index)
        chTemp+=controlBuf[index];
    controlBuf[8] = chTemp;
    controlBuf[9] = (char)2*chTemp;

    memcpy(buff+(packCount-1)*len,controlBuf,len);
}

/*
 *
 * 处理客户端 发送来的控制数据包 返回相应的设备ip 和 命令
 *data len 为 客户端 发来的 数据 和数据长度
 * strIp 和 buff len 是 返回的数据设备ip 和命令 命令长度
 * 返回 0 通用,解析数据完成
 * 返回 -1 通用,解析错误哦
 * 返回 1, (动画)显示客户端 登陆
 * 返回 2, (动画)显示客户端 下线
 * lifterID 返回 客户端关联的电梯ID
 */
int     BusinessSession::DealClientPackg(QString strData, QString const& strClientIp
	, QString & lifterID, QString & clientID)
{
    char buff[256] = {0};
    int packCount = 0;
    int len = 0;
    QStringList strlist=strData.split("#");
    //粘包时  list 大小 大于 PACK_MAX  需要分包
    QString strTemp;
     QDomDocument doc;
	 int ret = 0; //默认是正常的
    for(int index =0; index+PACK_MAX <= strlist.size(); index+=PACK_MAX)
    {

        if(QString(pack_buff[PACK_BEGIN]) == strlist[index+PACK_BEGIN]
                && QString(pack_buff[PACK_END])== strlist[index+PACK_END]
            )
         {
            strTemp = strlist[index+PACK_DATA];
            if(strlist[index+PACK_SIZE] != QString("%1").arg(strTemp.length()))
            {
                continue;
            }
            //解析收到的 xml
            ++packCount; //有效数据包 ++
            QString error;
             int row = 0, column = 0;
            if(!doc.setContent(strTemp, false, &error, &row, &column))
            {
                qDebug() << "解析数据错误:" << strTemp;
				ret = -1;
                continue ;
            }
            QString strID, strRec; //表示数据包中的设备DO值  和 数值
            QDomElement root = doc.firstChildElement();

            QDomElement child,rootFirst;
            rootFirst = root;
            QString strRootID = root.attribute(QString("type")); //找类型
            if(PCClientPkg_Jdq == strRootID.toInt()) //1.表示 继电器 控制命令
            {
                child = root.firstChildElement(QString("dev"));
                if(!child.isNull())
                {
                      strID = child.attribute(QString("ID"));
                      strRec = child.text();
                }
				//双笼a-b标识
				QString strLifterID;
				child = root.firstChildElement(QString("lifter"));
				if (!child.isNull())
				{
					
					strLifterID = child.text();
				}
				//默认开关长时间保持状态
				int openFlag = 0; 
				child = root.firstChildElement(QString("openFlag"));
				if (!child.isNull())
				{
					openFlag = child.text().toInt();
				}
				JDQ_DEAL_DATA dealData;
				dealData.m_strBelongs = strLifterID;
				dealData.m_strID = strID;
				dealData.m_strRec = strRec;
				dealData.m_openFlag= openFlag;
                QString strIp;
                GetJdqControlOrder(dealData,strIp,buff,len,packCount);

				
				
                //把 继电器控制命令 发送到 处理队列
                if( 0 != buff[0] && 0 != len) //设备登入 有数据可以发送
                {
                        QString strID = Config::GetInstance()->GetDeviceID(strIp);
                       // for( int index = 0; index < packCount; ++index)
                        {
							int indexNum = packCount - 1;
                            SendData msg;
                            memcpy(msg.buff,buff+(indexNum*len),len);
                            msg.len = len;
                          UserBuffer::GetInstance()->PushBackSendList(strID,msg);
						  QByteArray ary(msg.buff, msg.len);
						  QString strAryT = ary.toHex().toStdString().c_str();
						  qDebug() << dealData.m_strID << ":" << strAryT;
                        }

                 }

            }
            QString strBegin,strEnd,strType,strTimes;
            if(PCClientPkg_Lxzy == strRootID.toInt()) //2.连续作业实验 数据包
            {
              child = rootFirst.firstChildElement(QString("begin"));
               if(!child.isNull())
              {
                strBegin = child.text();
              }

               child = rootFirst.firstChildElement(QString("end"));
                if(!child.isNull())
               {
                 strEnd = child.text();
               }

                child = rootFirst.firstChildElement(QString("times"));
                 if(!child.isNull())
                {
                  strType = child.attribute(QString("type"));
                  strTimes = child.text();
                }

                experimentData data;
                data.m_begin = strBegin.toInt();
                data.m_end = strEnd.toInt();
                data.m_type = strType.toInt();
                data.m_times = strTimes.toInt();
                //开始连续作业实验
                StartExperimentThread(lifterID,data);



                //应答 客户端
                response_data resdata;
                resdata.strIp = strClientIp;
                QString strRet("<?xml version='1.0' encoding ='utf8'?><body type = '4005'>\
                               <ret>1</ret>\
                               </body>");
                strRet = QString("being#%1#%2#").arg(4005).arg(strRet.length())+strRet+QString("#end#");
                resdata.strData = strRet;
                UserBuffer::GetInstance()->PushResponseQueue(strClientIp,resdata);

            }
            if(PCClientPkg_Dhxz == strRootID.toInt()) //3.动画选择
            {
                child = rootFirst.firstChildElement(QString("lifter"));
                QString strLifter;
                 if(!child.isNull())
                {
                  strLifter = child.text();
                }
                //配置信息存 内存
                UserBuffer::GetInstance()->WriteDhxzBuffer(strLifter);

                QString strTemp;
                strTemp = QString("<?xml version='1.0' encoding='utf8'?>\
                                  <body ID='%1'><DhxzID>%2</DhxzID></body>").arg(CONFIG_DHXZ).arg(strLifter);
                strTemp  = QString("('%1',\"%2\");").arg(CONFIG_DHXZ).arg(strTemp);

                //配置信息存数据库
                UserBuffer::GetInstance()->PushDatabaseQueueAll(strTemp);

                //应答 客户端
                response_data data;
                data.strIp = strClientIp;
                QString strRet("<?xml version='1.0' encoding ='utf8'?><body type = '4005'>\
                               <ret>1</ret>\
                               </body>");
                strRet = QString("being#%1#%2#").arg(4005).arg(strRet.length())+strRet+QString("#end#");
                data.strData = strRet;
                UserBuffer::GetInstance()->PushResponseQueue(strClientIp,data);

            }
            if(PCClientPkg_Config== strRootID.toInt()) //4.电梯选择,BMQ配置数据包
            {

                QString strLifter,strBmqID,strConfigR,strConfigP;
                child = rootFirst.firstChildElement(QString("lifter"));
                if(!child.isNull())
               {
                 strLifter = child.text();
               }
                child = rootFirst.firstChildElement(QString("bmqID"));
                if(!child.isNull())
               {
                 strBmqID = child.text();
               }
                child = rootFirst.firstChildElement(QString("configR"));
                if(!child.isNull())
               {
                 strConfigR = child.text();
               }
                child = rootFirst.firstChildElement(QString("configP"));
                if(!child.isNull())
               {
                 strConfigP = child.text();
               }
                QMap<int,QString> retMap =  Config::GetInstance()->GetDeviceIDListByLifterID(strLifter, device_bmq);
                int responseRet = 1;
                QString deviceID,strTemp;
                if(retMap.contains(strBmqID.toInt()))
                {
                     deviceID = retMap[strBmqID.toInt()]; //获取指定电梯的 指定 bmqID的 ID

                    strTemp = QString("<?xml version='1.0' encoding='utf8'?>\
                                      <device_name  ID = '%1'>\
                                      <r>%2</r>\
                                      <p>%3</p>\
                                      </device_name>").arg(deviceID).arg(strConfigR).arg(strConfigP);
                    strTemp  = QString("('%1',\"%2\");").arg(deviceID).arg(strTemp);
                    //存设备配置 表
                    UserBuffer::GetInstance()->PushDatabaseQueueAll(strTemp);




                }

                //配置 客户端 关联的 电梯
                strTemp = QString("('3001','%1',\"%2\");").arg(strClientIp).arg(strLifter);

                //存客户端配置 表
                UserBuffer::GetInstance()->PushDatabaseQueueAll(strTemp,config_client_config);

                  response_data data;
                  data.strIp = strClientIp;
                  QString strRet = QString("<?xml version='1.0' encoding ='utf8'?><body type = '4005'>\
                                 <ret>%1</ret>\
                                 </body>").arg(responseRet);
                  strRet = QString("being#%1#%2#").arg(4005).arg(strRet.length())+strRet+QString("#end#");
                  data.strData = strRet;
                  UserBuffer::GetInstance()->PushResponseQueue(strClientIp,data);

                  //刷新config 内存
                  MemoryConfig config;
                  BMQCOf bmqcof;
                  bmqcof.m_d = strConfigR.toDouble();
                  bmqcof.m_p = strConfigP.toDouble();
                  config.m_bmqcof = bmqcof;
                  config.m_clientIp = strClientIp;
                  config.m_deviceID = deviceID;
                  config.m_strBelongs = strLifter;
                  Config::GetInstance()->SetLifterConfig(config);
            }
            if(PCClientPkg_CAN_CONTROL== strRootID.toInt()) //5.CAN模块控制数据包
            {
                //deviceIp
                QString strdeviceIp,strFlag;
                child = rootFirst.firstChildElement(QString("deviceIp"));
                if(!child.isNull())
               {
                 strdeviceIp = child.text();
               }
                child = rootFirst.firstChildElement(QString("choose"));
                if(!child.isNull())
               {
                 strFlag = child.text();
               }
                CCanModuleServer::GetInstance()->ControlCANModule(strdeviceIp,strFlag.toInt());

                //回应客户端
                response_data data;
                data.strIp = strClientIp;
                QString strRet("<?xml version='1.0' encoding ='utf8'?><body type = '4005'>\
                               <ret>1</ret>\
                               </body>");
                strRet = QString("being#%1#%2#").arg(4005).arg(strRet.length())+strRet+QString("#end#");
                data.strData = strRet;
                UserBuffer::GetInstance()->PushResponseQueue(strClientIp,data);
            }
			if (PCClientPkg_onlineStatus == strRootID.toInt())//  6.客户端在线状态 0 离线 1上线  2 心跳
			{
				QString strClientID, strLifterID, strMsg; //客户端ID 电梯ID 消息类型
				child = rootFirst.firstChildElement(QString("lifterID"));
				if (!child.isNull())
				{
					strLifterID = child.text();
				}
				child = rootFirst.firstChildElement(QString("clientID"));
				if (!child.isNull())
				{
					strClientID = child.text();
				}
				child = rootFirst.firstChildElement(QString("msg"));
				if (!child.isNull())
				{
					strMsg = child.text();
				}
				//msg:1 上线 0下线 2心跳
				if (strMsg == "1") //上线
				{
					//记录 客户端的信息
					clientID = strClientID;
					lifterID = strLifterID;
					ret = 1;
				}
				if (strMsg == "0") //下线
				{
					ret = 2;
				}
				//回应客户端
				response_data data;
				data.strIp = strClientIp;
				QString strRet("<?xml version='1.0' encoding ='utf8'?><body type = '4005'>\
                               <ret>1</ret>\
                               </body>");
				strRet = QString("being#%1#%2#").arg(4005).arg(strRet.length()) + strRet + QString("#end#");
				data.strData = strRet;
				UserBuffer::GetInstance()->PushResponseQueue(strClientIp, data);
			}


        }
    }
    return ret;
}


/*
 *下线通知 客户端
 */
QString    BusinessSession::GetExitDevicePackg(QString const& strIp)
{

    QString strRet,strTemp;

    int type = Config::GetInstance()->GetDeviceFlag(strIp);
  switch(type )
  {
  case device_cgq: //传感器 数据
      strTemp =  DealCgqQuit(strIp);
      break;
  case device_bmq: //编码器数据
      strTemp = DealBmqQuit(strIp);
      break;
  case device_jdq: //继电器
     strTemp =  DealJdqQuit(strIp);
      break;
  }
   if(!strTemp.isEmpty())
   {
       strRet += QString("<?xml version='1.0' encoding ='utf8'?>");
       strRet += QString("<body type = '%1' state='0'>").arg(type);
       strRet +=strTemp;
       strRet += QString("</body>");
       strRet = QString("being#%1#%2#").arg(type).arg(strRet.length())+strRet+QString("#end#");
    }

   return strRet;
}
//传感器
QString BusinessSession::DealCgqQuit(QString const& strIp)
{
    QString strRet;
    strRet = QString("<IP>%1</IP>").arg(strIp);
    return strRet;
}

//编码器
QString BusinessSession::DealBmqQuit(QString const& strIp)
{
    QString strRet;
    strRet = QString("<IP>%1</IP>").arg(strIp);
    return strRet;
}

//继电器
QString BusinessSession::DealJdqQuit(QString const& strIp)
{

    QString strConfig = Config::GetInstance()->GetDeviceConfig(Config::GetInstance()->GetDeviceID(strIp));
    QString strRet;
   QDomDocument doc;
   QString error;
    int row = 0, column = 0;
   if(!doc.setContent(strConfig, false, &error, &row, &column))
       return strRet;
   QDomElement root = doc.firstChildElement();
   QString strRootID = root.attribute(QString("ID"));
   root = root.firstChildElement(QString("DI_DATA"));
   root = root.firstChildElement();
   QString strText;
   while(!root.isNull())
   {
       int numID = root.attribute(QString("ID")).toInt();
        strText = root.attribute(QString("LINK"));
//       int q = qPow(2,numID-1);
//       q = q & uret;
       strRet += QString("<DI ID='%1' LINK='%2'>%3</DI>").arg(
                                                        QString("%1").arg(numID)
                                                              ,strText
                                                            ,QString("%1").arg(2));
        root = root.nextSiblingElement();
    }
   return strRet;
}


//电源
QString BusinessSession::DealDydlQuit(QString const& strIp)
{
	QString strRet;
	strRet = QString("<IP>%1</IP>").arg(strIp);
	return strRet;
}
/*
 * 返回错误信息
 */
QString BusinessSession::GetLastError(){
	QString strTemp = m_lastErr;
	m_lastErr.clear();
    return m_lastErr;

}
/*
 * 通用 获取各个设备 Userbuffer对象指针
 * 参数 设备id 和 设备类型 标识
 */
UserBuffer* BusinessSession::GetDeviceUserBuffer(QString const& strId, int flag)
{
    //if(device_cgq == flag)
    {
        if(!m_userbuffer_map.contains(strId))
            m_userbuffer_map[strId] = new UserBuffer();
        return m_userbuffer_map[strId];
    }

}
/*
 * can module 写缓存
 */
void    BusinessSession::WriteCanModuleBuffer(QString const& strIp
                                              ,VCI_CAN_OBJ* const& buff
                                              ,int const& size)
{
    if(!m_canModule_buffer_map.contains(strIp))
        m_canModule_buffer_map[strIp] = new CanModuleBuffer();
    m_canModule_buffer_map[strIp]->write(buff,size);
}

/*
 * 读 can module 缓存
 */
int    BusinessSession::ReadCanModuleBuffer(QString const& strIp
                                             ,VCI_CAN_OBJ* buff
                                             ,int const& size)
{
    if(!m_canModule_buffer_map.contains(strIp))
        return 0  ;
   return m_canModule_buffer_map[strIp]->read(buff,size);

}
