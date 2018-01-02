#include "businesssession.h"

#include"config/config.h"
#include<QDebug>
#include<QTime>
#include<QDomDocument>
#include<QtMath>
#include<QStringList>
#include<QThread>
#include<QDomComment>

//�ƶ���ʶ �ַ���
#define  BMQ_ZD     "20010003"

//���ݰ� ��ʽ
enum enum_PACK_DATA
{
    PACK_BEGIN, //��ʼ
    PACK_TYPE, //��������
    PACK_SIZE, //���ݴ�С
    PACK_DATA, //����
    PACK_END, //��ʶ��β
    PACK_MAX
};
const char* pack_buff[PACK_MAX] = {"being","type","size","data","end"};

BusinessSession* BusinessSession::m_singleton = NULL;

BusinessSession::BusinessSession()
{
   memset(m_cgq_buff,0,MAX_LEN);
   m_index = 0;



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
 /*�������������ݵ�ͳһ�ӿ�
  * strIp �豸IP
*/

 QString  BusinessSession::InterfaceFun(QString strIp,char* data, int len)
 {

     QString strRet("");
     QString strTemp("");
     int type = Config::GetInstance()->GetDeviceFlag(strIp);
     QString strID = Config::GetInstance()->GetDeviceID(strIp);
	 QTime t;
   switch(type )
   {
   case device_cgq: //������ ����
	   t.start();
       strTemp =  DealCgqData(strIp,data,len);
	    qDebug() << QStringLiteral("cgq3_%1").arg(t.elapsed());
       break;
   case device_bmq: //����������
       strTemp = DealBmqData(strIp,strID,data,len);
	  
       break;
   case device_jdq: //�̵���
      strTemp =  DealJdqData(strIp,data,len);
       break;
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
        * ���·��ͻ���
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
    QString strRet(""); // ��������
    //1.���뻺����
//     if(22 != len)
//         return strRet;
    QString strID = Config::GetInstance()->GetDeviceID(strIp);
    GetDeviceUserBuffer(strID,device_cgq)->Write_buffer(device_cgq,data,len);

    const int constlen = 22;
    char buff[constlen] = {0} ;
//    //�ҵ�һ��0x55��ͷ
//  //�����㷨˵��
//  /*
//   1 �ӻ�������ͷ���� ��ȡ��һ��0x55
//  2  �ٻ�ȡ����һλ & 0x5F �ж��Ƿ���0x5��ͷ
//   3  ���жϺ���һλ�ǲ���0x55 �������ѡȡ��Ϊͷ �ٴ�1 ���
//*/
  bool flag = false;
  //bool zeroFlag = true; //�Ƿ��ȡһ������������
 // QString strTemp; //��ʾ���ݵ�16���Ƶ��ַ���

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
//        ax=((AxH<<8)|AxL)/32768*16g(gΪ�������ٶȣ���ȡ9.8m/s2)
//        ay=((AyH<<8)|AyL)/32768*16g(gΪ�������ٶȣ���ȡ9.8m/s2)
//        az=((AzH<<8)|AzL)/32768*16g(gΪ�������ٶȣ���ȡ9.8m/s2)
        // 0x51: //���ٶ�

          //��λg  g����ȡ9.8m/s

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
            //ƫ���ǣ�z�ᣩYaw=((YawH<<8)|YawL)/32768*180(��)
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

     //�洢 ���� �ͻ��� ���ݰ�
	UserBuffer::GetInstance()->WriteDeviceToDhData(strIp,strDhData);
	QString strTestTime = QString("cgq_start_%1").arg(QTime::currentTime().toString(QString("HH:mm:ss:ms")));

    return strRet;

}

/*������ת��������Ϣ
* strID �豸ID ÿ���豸 ��Ӧһ��ip ��ӦΨһ�豸ID
*
*/
QString BusinessSession::DealBmqData(QString const& strIp,QString const& strID,char* data,int len)
{
    assert(NULL != data);
    QString strRet("");
    /*
     * ����Ǳ����� ����������Ⱥ�˳��
     */



    //1.�������������һ������
        if(0x03 == (0xff&data[1]) && BMQPACKAGE_ONESIZE == len) //��ʾ��һ��ָ��ֵ�����ˣ��ٶ�ȡ�ڶ���ָ���ֵ
        {
            setBmqFirst(strIp, false);
            setBmqMapBuff(strIp,data,len);
            //setBmqDataLen(strIp, len);

            return strRet;
        }
        if(0x04 == (0xff&data[1]) && BMQPACKAGE_TWOSIZE == len) //��ʾ�ڶ���ָ��ֵ�����ˣ��ٶ�ȡ��һ��ָ���ֵ
        {
            setBmqFirst(strIp, true);
            setBmqMapBuff(strIp,data,len);

        }

   //2.crc16 У��

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
    if(mycrcJym == dataJym&& mycrcJym2 == dataJym2 ) //����У��ͨ��
    {
        //        ����80 00 �����λΪ1 ����Ϊ���� �����00 00 ��Ϊ����
        //                  �Ƕȣ�      39 39 �� 146.49����ԽǶȣ���������ϵͳ�к���
        //                  ת�٣�            00 00 17 70 ��6000 �� 60.00rpm
                QString strDir; //���� �Ƕ� ת��

                if(0x80 == (0xff&m_bmqBuff[3]))
                    strDir = "1";//"����";
                else
                    strDir = "0";//"����";


                 double shortJd = (unsigned short) m_bmqBuff[5]<<8 | (0xff&m_bmqBuff[6]);
                double shortZs = ( m_bmqBuff[7]<<24 | m_bmqBuff[8] << 16 | m_bmqBuff[9] << 8 | (0xff&m_bmqBuff[10]));


        //       1����00 00 23 93ת��Ϊʮ����Ϊ��00 00 35 147
        //           2������ֵ = ����00*256+00��*256+35��*256+147 =9107
                unsigned long jsz = (( ((0xff&m_bmqBuff[16])*256 +  (0xff&m_bmqBuff[17]))  * 256 + (0xff&m_bmqBuff[18])))*256 + (0xff&m_bmqBuff[19]);

        //       �����趨����ֵΪx���趨������һȦ�Ŀ̶���p�����ڱ�����1�ϵ�����ֱ��Ϊd������������λ�ƣ�·�̣�Ϊy����λ�ƣ�·�̣��ļ��㷽ʽΪ��
        // y = x*�����ܳ�/(2*p)


                QDateTime current_date_time = QDateTime::currentDateTime();
                QString strDate = current_date_time.toString("yyyy/MM/dd");
                QString current_time = current_date_time.toString("hh:mm:ss.zzz");
                //���� �Ƕ� ת��  ����ֵ


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



                //�ƶ�����
                QString strZdjl;
                 BMQCOf data = Config::GetInstance()->GetBmqConfig(strIp);
                int oldJsz = UserBuffer::GetInstance()->GetBmqBeginZdJsz(strIp);
                if(-1 != oldJsz)
                {
                    if(0 == shortZs) //�ƶ� ���
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
                        //�洢 ���� �ͻ��� ���ݰ�
                        UserBuffer::GetInstance()->WriteDeviceToDhData(strIp,strDhData);



                /*
                 * 2017/6/19
                 * ���ӻ�ȡ�����ݵ�ʱ��� ���� �ٶ�
                 */
                strRet = QString("<dir>%1</dir>").arg(strDir)
                        + QString("<jd>%1</jd>").arg(shortJd)
                        + QString("<zs>%1</zs>").arg(shortZs)
                        +QString("<jsz>%1</jsz>").arg(jsz)
                        +QString("<time>%1</time>").arg(current_date_time.toTime_t())
                        +QString("<zdjl>%1</zdjl>").arg(strZdjl);
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

/*����ÿת��С*/
void BusinessSession::setBmqSize(const QString &strIp, int size)
{

      m_mz_size[strIp] = size;
}
/*��ȡÿת��С*/
int BusinessSession::getBmqSize(const QString &strIp)
{
    if(!m_mz_size.contains(strIp))
        m_mz_size[strIp] = 1;
    return m_mz_size[strIp];
}
/*����ÿת����  �̶�*/
void BusinessSession::setBmqKd(const QString &strIp, int size)
{
      m_mz_kd[strIp] = size;
}

/*��ȡÿת����  �̶�*/
int BusinessSession::getBmqKd(const QString &strIp)
{
    if(!m_mz_kd.contains(strIp))
        m_mz_kd[strIp] = 1;
    return m_mz_kd[strIp];
}

/*����ָ���*/
void BusinessSession::setBmqDataLen(QString const& strIp,int size)
{
      m_bmqDataLen[strIp] = size;
}

/*ָ���*/
int BusinessSession::getBmqDataLen(QString const& strIp)
{
    if(!m_bmqDataLen.contains(strIp))
        m_bmqDataLen[strIp] = 0;
    return m_bmqDataLen[strIp];
}
/*����m_bmqMapBuff*/
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
* ����̵�������
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
    if(len >device_jdq_data_len) //�������ݴ��� ��ֵʱ �ְ�����
    {
        for(int index= 0; index<=len-device_jdq_data_len-3; index+=3) //-3 �ǿ������� �������� 0x4f4b21 �ĳ���
        {
            if(data[0] == 0x4f
                    &&data[1] == 0x4b
                    &&data[2] == 0x21)
            {

                data = data+3; //���ݰ� ʼָ������3λ
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
    unsigned char  chSum1 = 0, chSum2 = 0;// У��1 У��2
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
            * �ƶ�ʱ
            */
           if(strText == BMQ_ZD && 1 == q)
           {
               QString strBmqIp = Config::GetInstance()->GetCgqToBmqIp(strIp);
                UserBuffer::GetInstance()->SetBmqBeginZdJsz(strBmqIp
                                                            ,UserBuffer::GetInstance()->GetBmqNewestJsz(strBmqIp));
           }


            root = root.nextSiblingElement();
        }

       //�� ���ݿ� ������ ������
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

      //�洢 ���� �ͻ��� ���ݰ�
      UserBuffer::GetInstance()->WriteDeviceToDhData(strIp,strDhData);


    }
    //qDebug() << strRet;
    return strRet;
}

/*
 * ��ʼִ��ʵ���߳�
 */
void BusinessSession::StartExperimentThread(QString const& strClientIp,experimentData const& strData)
{
    if(!m_experimentThread_map.contains(strClientIp))
        m_experimentThread_map[strClientIp] = new ExperimentThread(strClientIp);
    m_experimentThread_map[strClientIp]->SetData(strData);
    m_experimentThread_map[strClientIp]->start();

}

/*
 * ��ȡ�̵���ָ��
 * ����˵��: strClientIp �ͻ���ip
 * strID ���ư�ť��ʶ
 * strDeviceIp �����豸IP
 * buff ���ؿ�����������
 * len ���������
 * packCount ���ݰ�����
 */
bool    BusinessSession::GetJdqControlOrder(QString const& strClientIp
                                            , QString const& strID
                                            , QString const& strRec
                                            , QString & strDeviceIp
                                            ,char* buff
                                            ,int & len
                                            , int packCount)
{
    char controlBuf[] = {(char)0xcc,(char)0xdd,(char)0xa1,(char)0x01
                         ,(char)0xff,(char)0xff,(char)0x00,(char)0x00,(char)0x9e,(char)0x3c};
    len = 10;

    /* 0 ��ʾ �豸�󶨵�DO ��λ
     * 1 ��ʾ�豸�󶨵�DO  �պ�
     *
     */

    //��ȡ �豸�󶨵� DO
    QString strDO = Config::GetInstance()->GetDOByDeviceID(strClientIp,strID);
    QString strIp = Config::GetInstance()->GetDeviceIpByID(strClientIp,strID);
    strDeviceIp = strIp;
    if(strDO.isEmpty() || strIp.isEmpty())
    {
        m_lastErr = QString("%1���豸��DOʧ��!ip=%2,do=%3").arg(strID).arg(strIp).arg(strDO);

        return false;
    }
    int iDo = strDO.toInt();
    short t = qPow(2,iDo);
    char sh = t ;
    if(QString("0") == strRec)
    {
            //CC DD A1 01 FF FF FF FF 9E 3C
        controlBuf[4] = (char)0x00 ;
        controlBuf[5] = (char)0x00 ;
        if(iDo>=8)
        {
            sh =t>>8;
            controlBuf[6] = (char)0x00  | sh;
         }
        else
        {

             controlBuf[7] = (char)0x00 | sh;
         }
    }
    else
    {
         controlBuf[4] = (char)0xff ;
          controlBuf[5] = (char)0xff;
        if(iDo>=8)
        {
            sh =t>>8;
            controlBuf[6] = (char)0x00 | sh;
         }
        else
        {

             controlBuf[7] = (char)0x00 | sh;
         }
    }
    char chTemp = 0;
    //У�� ��ȥ ֡ͷ 16λ  ʣ�µ� 6 �� 8λ �������
    for(int index =2; index < 8; ++index)
        chTemp+=controlBuf[index];
    controlBuf[8] = chTemp;
    controlBuf[9] = (char)2*chTemp;

    memcpy(buff+(packCount-1)*len,controlBuf,len);
}

/*
 *
 * ����ͻ��� �������Ŀ������ݰ� ������Ӧ���豸ip �� ����
 *data len Ϊ �ͻ��� ������ ���� �����ݳ���
 * strIp �� buff len �� ���ص������豸ip ������ �����
 */
bool    BusinessSession::DealClientPackg(QString strData, QString const& strClientIp)
{
    char buff[256] = {0};
    int packCount = 0;
    int len = 0;
    QStringList strlist=strData.split("#");
    //ճ��ʱ  list ��С ���� PACK_MAX  ��Ҫ�ְ�
    QString strTemp;
     QDomDocument doc;

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
            //�����յ��� xml
            ++packCount; //��Ч���ݰ� ++
            QString error;
             int row = 0, column = 0;
            if(!doc.setContent(strTemp, false, &error, &row, &column))
            {
                qDebug() << "�������ݴ���:" << strTemp;
                continue ;
               }
            QString strID, strRec; //��ʾ���ݰ��е��豸ID �� ��ֵ
            QDomElement root = doc.firstChildElement();

            QDomElement child,rootFirst;
            rootFirst = root;
            QString strRootID = root.attribute(QString("type")); //������
            if(PCClientPkg_Jdq == strRootID.toInt()) //1.��ʾ �̵��� ��������
            {
                child = root.firstChildElement();
                if(!child.isNull())
                {
                      strID = child.attribute(QString("ID"));
                      strRec = child.text();
                }



                QString strIp;
                GetJdqControlOrder(strClientIp,strID,strRec,strIp,buff,len,packCount);

                //�� �̵����������� ���͵� �������
                if( 0 != buff[0] && 0 != len) //�豸���� �����ݿ��Է���
                {
                        QString strID = Config::GetInstance()->GetDeviceID(strIp);
                        for( int index = 0; index < packCount; ++index)
                        {
                            SendData msg;
                            memcpy(msg.buff,buff+(index*len),len);
                            msg.len = len;
                          UserBuffer::GetInstance()->PushBackSendList(strID,msg);
                        }

                 }

            }

            QString strBegin,strEnd,strType,strTimes;
            if(PCClientPkg_Lxzy == strRootID.toInt()) //2.������ҵʵ�� ���ݰ�
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
                //��ʼ������ҵʵ��
                StartExperimentThread(strClientIp,data);



                //Ӧ�� �ͻ���
                response_data resdata;
                resdata.strIp = strClientIp;
                QString strRet("<?xml version='1.0' encoding ='utf8'?><body type = '4005'>\
                               <ret>1</ret>\
                               </body>");
                strRet = QString("being#%1#%2#").arg(4005).arg(strRet.length())+strRet+QString("#end#");
                resdata.strData = strRet;
                UserBuffer::GetInstance()->PushResponseQueue(strClientIp,resdata);

            }
            if(PCClientPkg_Dhxz == strRootID.toInt()) //3.����ѡ��
            {
                child = rootFirst.firstChildElement(QString("lifter"));
                QString strLifter;
                 if(!child.isNull())
                {
                  strLifter = child.text();
                }
                //������Ϣ�� �ڴ�
                UserBuffer::GetInstance()->WriteDhxzBuffer(strLifter);

                QString strTemp;
                strTemp = QString("<?xml version='1.0' encoding='utf8'?>\
                                  <body ID='%1'><DhxzID>%2</DhxzID></body>").arg(CONFIG_DHXZ).arg(strLifter);
                strTemp  = QString("('%1',\"%2\");").arg(CONFIG_DHXZ).arg(strTemp);

                //������Ϣ�����ݿ�
                UserBuffer::GetInstance()->PushDatabaseQueueAll(strTemp);

                //Ӧ�� �ͻ���
                response_data data;
                data.strIp = strClientIp;
                QString strRet("<?xml version='1.0' encoding ='utf8'?><body type = '4005'>\
                               <ret>1</ret>\
                               </body>");
                strRet = QString("being#%1#%2#").arg(4005).arg(strRet.length())+strRet+QString("#end#");
                data.strData = strRet;
                UserBuffer::GetInstance()->PushResponseQueue(strClientIp,data);

            }
            if(PCClientPkg_Config== strRootID.toInt()) //4.����ѡ��,BMQ�������ݰ�
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
                QMap<int,QString> retMap =  Config::GetInstance()->GetDeviceIDListByLifterID(strLifter,1002);
                int responseRet = 1;
                QString deviceID,strTemp;
                if(retMap.contains(strBmqID.toInt()))
                {
                     deviceID = retMap[strBmqID.toInt()]; //��ȡָ�����ݵ� ָ�� bmqID�� ID

                    strTemp = QString("<?xml version='1.0' encoding='utf8'?>\
                                      <device_name  ID = '%1'>\
                                      <r>%2</r>\
                                      <p>%3</p>\
                                      </device_name>").arg(deviceID).arg(strConfigR).arg(strConfigP);
                    strTemp  = QString("('%1',\"%2\");").arg(deviceID).arg(strTemp);
                    //���豸���� ��
                    UserBuffer::GetInstance()->PushDatabaseQueueAll(strTemp);




                }

                //���� �ͻ��� ������ ����
                strTemp = QString("('3001','%1',\"%2\");").arg(strClientIp).arg(strLifter);

                //��ͻ������� ��
                UserBuffer::GetInstance()->PushDatabaseQueueAll(strTemp,config_client_config);

                  response_data data;
                  data.strIp = strClientIp;
                  QString strRet = QString("<?xml version='1.0' encoding ='utf8'?><body type = '4005'>\
                                 <ret>%1</ret>\
                                 </body>").arg(responseRet);
                  strRet = QString("being#%1#%2#").arg(4005).arg(strRet.length())+strRet+QString("#end#");
                  data.strData = strRet;
                  UserBuffer::GetInstance()->PushResponseQueue(strClientIp,data);

                  //ˢ��config �ڴ�
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
            if(PCClientPkg_CAN_CONTROL== strRootID.toInt()) //5.CANģ��������ݰ�
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

                //��Ӧ�ͻ���
                response_data data;
                data.strIp = strClientIp;
                QString strRet("<?xml version='1.0' encoding ='utf8'?><body type = '4005'>\
                               <ret>1</ret>\
                               </body>");
                strRet = QString("being#%1#%2#").arg(4005).arg(strRet.length())+strRet+QString("#end#");
                data.strData = strRet;
                UserBuffer::GetInstance()->PushResponseQueue(strClientIp,data);
            }
        }
    }
    return true;
}


/*
 *����֪ͨ �ͻ���
 */
QString    BusinessSession::GetExitDevicePackg(QString const& strIp)
{

    QString strRet,strTemp;

    int type = Config::GetInstance()->GetDeviceFlag(strIp);
  switch(type )
  {
  case device_cgq: //������ ����
      strTemp =  DealCgqQuit(strIp);
      break;
  case device_bmq: //����������
      strTemp = DealBmqQuit(strIp);
      break;
  case device_jdq: //�̵���
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
//������
QString BusinessSession::DealCgqQuit(QString const& strIp)
{
    QString strRet;
    strRet = QString("<IP>%1</IP>").arg(strIp);
    return strRet;
}

//������
QString BusinessSession::DealBmqQuit(QString const& strIp)
{
    QString strRet;
    strRet = QString("<IP>%1</IP>").arg(strIp);
    return strRet;
}

//�̵���
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

/*
 * ���ش�����Ϣ
 */
QString BusinessSession::GetLastError(){

    return m_lastErr;

}
/*
 * ͨ�� ��ȡ�����豸 Userbuffer����ָ��
 * ���� �豸id �� �豸���� ��ʶ
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
 * can module д����
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
 * �� can module ����
 */
int    BusinessSession::ReadCanModuleBuffer(QString const& strIp
                                             ,VCI_CAN_OBJ* buff
                                             ,int const& size)
{
    if(!m_canModule_buffer_map.contains(strIp))
        return 0  ;
   return m_canModule_buffer_map[strIp]->read(buff,size);

}
