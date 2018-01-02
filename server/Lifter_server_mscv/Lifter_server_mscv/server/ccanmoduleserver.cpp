#include "ccanmoduleserver.h"
#include"buffer/usebuffer.h"
#include<QTime>
#include"server/server.h"
#include<QDateTime>
#include<QWaitCondition>
//读can设备数据的长度
#define CAN_REC_DATALEN  50
 QWaitCondition m_controlWait; //can模块控制 等待 和唤醒
QMutex m_controlMutex;
 CCanModuleServer*   CCanModuleServer::m_singleton = NULL;
 CANModuleLib*       CANModuleLib::m_singleton = NULL;
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
 CANModuleLib::CANModuleLib()
 {
     m_VCI_CloseDevice		=NULL;
     m_VCI_GetReceiveNum =NULL;
         m_VCI_GetReference	=NULL;
       m_VCI_InitCAN		=NULL;
         m_VCI_OpenDevice	=NULL;
         m_VCI_ReadBoardInfo=NULL;
         m_VCI_ReadCANStatus=NULL;
       m_VCI_ReadErrInfo	=NULL;
       m_VCI_Receive		=NULL;
         m_VCI_ResetCAN		=NULL;
         m_VCI_SetReference	=NULL;
         m_VCI_StartCAN		=NULL;
          m_VCI_Transmit	=NULL	;
          m_lib = NULL;
 }


 void CANModuleLib::UnLoadLib()
 {
//     if(m_lib)
//        m_lib->unload();
     if(m_singleton)
     {
        delete m_singleton;
        m_singleton = NULL;
     }
 }

 /*
  * 单实例化
  */
CANModuleLib* CANModuleLib::GetInstance()
{
    if(NULL == m_singleton)
        m_singleton = new CANModuleLib;
    return m_singleton;
}

/*
 * 初始化CANmodule 库
 */
bool  CANModuleLib::InitLib()
{

    if (m_VCI_CloseDevice 	||
            m_VCI_GetReceiveNum  	||
            m_VCI_GetReference	 	||
            m_VCI_InitCAN		 	||
                 m_VCI_OpenDevice	||
            m_VCI_ReadBoardInfo  	||
            m_VCI_ReadCANStatus  	||
            m_VCI_ReadErrInfo	 	||
            m_VCI_Receive		 	||
            m_VCI_ResetCAN		 	||
            m_VCI_SetReference	 	||
            m_VCI_StartCAN		 	||
             m_VCI_Transmit			)
    {
        return true;
    }

     QString strDLLPath = "ControlCAN.dll";
    m_lib = new QLibrary(strDLLPath);
    if (m_lib->load())
    {

        m_VCI_CloseDevice	   =  (VCI_CloseDevice	)m_lib->resolve("VCI_CloseDevice");

        m_VCI_GetReceiveNum   =  (VCI_GetReceiveNum )m_lib->resolve("VCI_GetReceiveNum");
        m_VCI_GetReference	   =  (VCI_GetReference )m_lib->resolve("VCI_GetReference");
        m_VCI_InitCAN		   =  (VCI_InitCAN		)m_lib->resolve("VCI_InitCAN");
             m_VCI_OpenDevice= (VCI_OpenDevice)m_lib->resolve("VCI_OpenDevice");
        m_VCI_ReadBoardInfo   =  (VCI_ReadBoardInfo )m_lib->resolve("VCI_ReadBoardInfo");
        m_VCI_ReadCANStatus   =  (VCI_ReadCANStatus )m_lib->resolve("VCI_ReadCANStatus");
        m_VCI_ReadErrInfo   =  (VCI_ReadErrInfo)m_lib->resolve("VCI_ReadErrInfo");
        m_VCI_Receive   =  (VCI_Receive)m_lib->resolve("VCI_Receive");
        m_VCI_ResetCAN   =  (VCI_ResetCAN)m_lib->resolve("VCI_ResetCAN");
        m_VCI_SetReference   =  (VCI_SetReference )m_lib->resolve("VCI_SetReference");
        m_VCI_StartCAN   =  (VCI_StartCAN)m_lib->resolve("VCI_StartCAN");
         m_VCI_Transmit= (VCI_Transmit         )m_lib->resolve("VCI_Transmit");
        if (!m_VCI_CloseDevice 	||
                !m_VCI_GetReceiveNum  	||
                !m_VCI_GetReference	 	||
                !m_VCI_InitCAN		 	||
                !	 m_VCI_OpenDevice	||
                !m_VCI_ReadBoardInfo  	||
                !m_VCI_ReadCANStatus  	||
                !m_VCI_ReadErrInfo	 	||
                !m_VCI_Receive		 	||
                !m_VCI_ResetCAN		 	||
                !m_VCI_SetReference	 	||
                !m_VCI_StartCAN		 	||
                ! m_VCI_Transmit			)
        {
              m_lastError = QString("初始化失败!")+strDLLPath;

              return false;
        }
        m_func_map[CANMODULE_VCI_CloseDevice]		=	 (void*)m_VCI_CloseDevice		;

        m_func_map[CANMODULE_VCI_GetReceiveNum]     =   (void*) m_VCI_GetReceiveNum	;
        m_func_map[CANMODULE_VCI_GetReference ]     =    (void*)m_VCI_GetReference		;
        m_func_map[CANMODULE_VCI_InitCAN		]   =    (void*)m_VCI_InitCAN			;
        m_func_map[CANMODULE_VCI_OpenDevice	]       =   (void*) m_VCI_OpenDevice		;
        m_func_map[CANMODULE_VCI_ReadBoardInfo]     =   (void*) m_VCI_ReadBoardInfo	;
        m_func_map[CANMODULE_VCI_ReadCANStatus]     =   (void*) m_VCI_ReadCANStatus	;
        m_func_map[CANMODULE_VCI_ReadErrInfo	]   =  (void*) m_VCI_ReadErrInfo		;
        m_func_map[CANMODULE_VCI_Receive		]   =  (void*) m_VCI_Receive			;
        m_func_map[CANMODULE_VCI_ResetCAN	  	]   =   (void*) m_VCI_ResetCAN			;
        m_func_map[CANMODULE_VCI_SetReference ]     =   (void*) m_VCI_SetReference		;
        m_func_map[CANMODULE_VCI_StartCAN	  	]   =   (void*) m_VCI_StartCAN			;
        m_func_map[CANMODULE_VCI_Transmit     ]     =  (void*) m_VCI_Transmit			;
        return true;

    }
    else
    {
        m_lastError = m_lib->errorString();
        return false;
     }
	return true;
}

/*
 * 获取dll 的函数指针
 */
void* CANModuleLib::GetLibFunc(int flag)
{
    if(InitLib() && m_func_map.contains(flag))
    {
         //VCI_CloseDevice p = (VCI_CloseDevice)m_func_map[flag];
      return   m_func_map[flag];
    }
    return NULL;
}

 QString CANModuleLib::GetLastError()
 {
    return m_lastError;
 }


QString CCanModuleServer::GetLastError()
{
    return m_lastError;
}
 CCanModuleServer*   CCanModuleServer::GetInstance()
 {
    if(NULL == m_singleton)
        m_singleton = new CCanModuleServer();
    return m_singleton;
 }


 bool    CCanModuleServer::StartCANModule(int index,CANDATA const& canData)
 {

    m_dataConThread[index] = new DataConnectionThread(canData);  //数据采集
    m_analyzeSaveDB[index] = new DataAnalyzeSaveDB(canData.strIp,Config::GetInstance()->GetDeviceID(canData.strIp)); //数据存 数据库
    m_analyzeThread[index] = new DataAnalyzeThread(canData.strIp,(m_dataConThread[index]->m_gSaveFifo_buf)); //数据解析

    //保存 获取can模块数据的 线程 指针
    m_analyzeThread[index]->m_connectThread = m_dataConThread[index];

    if(!m_dataConThread[index]->InitCANModule())
    {
       m_lastError = m_dataConThread[index]->GetLastError();
       qDebug() << m_lastError;
        //return false;
     }
    m_dataConThread[index]->start();
    m_analyzeThread[index]->start();
    m_analyzeSaveDB[index]->start();

    m_controlThread_map[canData.strIp] = m_dataConThread[index];

    return true;
 }
void    CCanModuleServer::StopCANModuel()
{
    for(int index = 0; index <CANMODULE_MAX;++index)
    {
        //DataConnectionThread* p = m_dataConThread[index];
        if(m_dataConThread[index])
           {
            m_dataConThread[index]->stop();
            m_dataConThread[index]->quit();
            m_dataConThread[index]->wait();
           }
        if(m_analyzeThread[index])
        {
            m_analyzeThread[index]->stop();
            m_analyzeThread[index]->quit();
            m_analyzeThread[index]->wait();
         }
        if(m_analyzeSaveDB[index])
        {
            m_analyzeSaveDB[index]->stop();
            m_analyzeSaveDB[index]->quit();
            m_analyzeSaveDB[index]->wait();
         }
    }
    m_controlThread_map.clear();
    CANModuleLib::GetInstance()->UnLoadLib();

}

CCanModuleServer::CCanModuleServer()
{
   // memset(m_dataConThread,0,CANMODULE_MAX);
    for(int index = 0; index <CANMODULE_MAX;++index)
    {
         m_dataConThread[index] = NULL;
         m_analyzeThread[index] = NULL;
         m_analyzeSaveDB[index] = NULL;
//        if(m_dataConThread[index])
//            m_dataConThread[index]->stop();
    }
}


/*
 * 启动 或 停止 指定
 * 设备ip 的CAN设备
 * 0 停止 1启动
 *
 */
 void    CCanModuleServer::ControlCANModule(QString const& strIp,int flag)
 {
    if(m_controlThread_map.contains(strIp))
    {
        m_controlThread_map[strIp]->ControlCANModule(flag);
    }
 }



DataConnectionThread::DataConnectionThread(CANDATA const& canData)
    :m_initFlag(false)
    ,m_strIp(canData.strIp)
    ,m_port(canData.port)
    ,m_cannum(canData.cannum)
    ,m_devind(canData.devind)
    ,m_connect(canData.connect)
    ,m_devtype(canData.devtype)
    ,m_stopFlag(true)
    ,m_controlStop(false)
{

    m_VCI_CloseDevice		=NULL;
    m_VCI_GetReceiveNum =NULL;
        m_VCI_GetReference	=NULL;
      m_VCI_InitCAN		=NULL;
        m_VCI_OpenDevice	=NULL;
        m_VCI_ReadBoardInfo=NULL;
        m_VCI_ReadCANStatus=NULL;
      m_VCI_ReadErrInfo	=NULL;
      m_VCI_Receive		=NULL;
        m_VCI_ResetCAN		=NULL;
        m_VCI_SetReference	=NULL;
        m_VCI_StartCAN		=NULL;
         m_VCI_Transmit	=NULL	;

    memset(gCAN_BUF,0,CANBUFF_SIZE);

    /*
     * CAN模块 ID 对应的 通道 标识 初始化
     */

    memset(m_gSaveFifo_buf,0,STRAINGAUGES_CH_NUM);
    for(int index =0; index < STRAINGAUGES_CH_NUM;++index)
    {
        m_gSaveFifo_buf[index].cnt = 0;
        m_gSaveFifo_buf[index].id = 0;
        memset(m_gSaveFifo_buf[index].BUF,0,FIFO_LEN);
    }
}

  QString DataConnectionThread::GetLastError()//获取最后一个错误信息
  {
      return m_lastError;
  }

  bool    DataConnectionThread::InitCANModule() //初始化CAN MODULE 连接
  {
      qDebug() << "开始初始化CAN MODULE 连接";


      m_VCI_CloseDevice		=(VCI_CloseDevice	)	CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_CloseDevice	);
      m_VCI_GetReceiveNum	=(VCI_GetReceiveNum	)CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_GetReceiveNum);
      m_VCI_GetReference		=(VCI_GetReference	)CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_GetReference );
      m_VCI_InitCAN			=(VCI_InitCAN		)	CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_InitCAN		);
      m_VCI_OpenDevice		=(VCI_OpenDevice	)	CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_OpenDevice	);
      m_VCI_ReadBoardInfo	=(VCI_ReadBoardInfo	)CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_ReadBoardInfo);
      m_VCI_ReadCANStatus	=(VCI_ReadCANStatus	)CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_ReadCANStatus);
     m_VCI_ReadErrInfo		=(VCI_ReadErrInfo	) 	CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_ReadErrInfo	);
     m_VCI_Receive			=(VCI_Receive		) 	CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_Receive		);
      m_VCI_ResetCAN			=(VCI_ResetCAN		)CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_ResetCAN	  	);
      m_VCI_SetReference		=(VCI_SetReference	)CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_SetReference );
      m_VCI_StartCAN			=(VCI_StartCAN		)CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_StartCAN	  	);
     m_VCI_Transmit			=(VCI_Transmit      ) CANModuleLib::GetInstance()->GetLibFunc(CANMODULE_VCI_Transmit     );

        if (!m_VCI_CloseDevice 	||
                !m_VCI_GetReceiveNum  	||
                !m_VCI_GetReference	 	||
                !m_VCI_InitCAN		 	||
                !	 m_VCI_OpenDevice	||
                !m_VCI_ReadBoardInfo  	||
                !m_VCI_ReadCANStatus  	||
                !m_VCI_ReadErrInfo	 	||
                !m_VCI_Receive		 	||
                !m_VCI_ResetCAN		 	||
                !m_VCI_SetReference	 	||
                !m_VCI_StartCAN		 	||
                ! m_VCI_Transmit)

            {
                m_lastError = CANModuleLib::GetInstance()->GetLastError();
                return false;
            }

      if(m_VCI_OpenDevice(m_devtype,m_devind,0)!=STATUS_OK)
          {
          //(*VCI_ReadErrInfo)(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_ERR_INFO pErrInfo);
          VCI_ERR_INFO errinfo;
           m_VCI_ReadErrInfo(m_devtype,m_devind,m_cannum,&errinfo);
              m_lastError= QString("打开设备失败!_errorcode = %1").arg(errinfo.ErrCode);

              return false;
          }

          m_VCI_SetReference(m_devtype,m_devind,0,CMD_DESIP,(PVOID)m_strIp.toStdString().c_str());
          m_VCI_SetReference(m_devtype,m_devind,0,CMD_DESPORT,(PVOID)&m_port);

          if(m_VCI_StartCAN(m_devtype,m_devind,0)!=STATUS_OK)
          {
              m_lastError= QString("打开设备失败!");
              return false;
          }

          m_initFlag = true;
          return true;
  }


  bool      DataConnectionThread::ControlCANModule(int flag) //控制CAN模块 0 暂停 1启动
  {
      if(0 == flag)
      {
          if(m_VCI_CloseDevice)
              m_VCI_CloseDevice(m_devtype,m_devind);

          m_controlStop = true;
      }
      if(1 == flag)
      {
          m_controlStop = false;
          m_controlWait.wakeAll();
      }
	  return true;
  }

  void gettime(const unsigned char *buf,strainDeviceData_T *device)
  {
      int strain_temp;
      time_t time_sec_temp;
      //取时间
      time_sec_temp = (buf[0]<<24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
      device->time = localtime(&time_sec_temp); /*转换为struct tm结构的当地时间*/

      //取毫秒
      device->time_ms = (buf[4]<<8) | buf[5];
      device->time_ms = (device->time_ms>>4) & 0x0fff;

      //取应变量
      strain_temp = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
      if (strain_temp & 0x80000)
      {
          //负数
          device->strain = strain_temp | 0xfffe0000;
      }
      else
      {
          //正数
          device->strain = strain_temp & (~0xfffe0000);
      }

  }

#ifdef NOUSE
  void DataConnectionThread::AnalsyData(device_buf_T const& buff)
  {

          int i,j;
          static int ch = STRAINGAUGES_CH1;
          double temp = 0;
          strainDeviceData_T tempStrain = { 0 };
          static int lastMs[10] = { 0 };
          QString textTemp;
          int dispHz = 30;
        //  gFifo_buf[ch].cnt = 0;//计数器清零
          struct tm *time;

          int nowIndex;


              //for (ch = STRAINGAUGES_CH1; ch < STRAINGAUGES_CH_NUM; ch++)

                  nowIndex = buff.cnt;
                  //计算数据速率
                  for (i = 0; i < 4; i++)
                  {
                      gettime(buff.BUF[nowIndex-1].Data, &tempStrain);
                      if (nowIndex)
                          nowIndex--;
                      else
                          nowIndex = FIFO_LEN;
                      lastMs[i] = tempStrain.time_ms;
                  }
                  j = 0;
                  temp = 0;
                  for (i = 0; i < 3; i++)
                  {
                      if (lastMs[i] > lastMs[i + 1])
                          temp += lastMs[i] - lastMs[i + 1];
                      else
                          break;
                      j++;
                  }
                  if (j > 2)
                  {
                      temp /= j;
                      temp = 1000 / temp;
                      /*temp += 0.5;*/
                      dispHz = temp;
                  }

                  //更新频率
                  textTemp +=QString("##") + QString("%1").arg(dispHz);


                  if (buff.cnt == 0)
                  {
                      gettime(buff.BUF[FIFO_LEN - 1].Data, &tempStrain);
                  }
                  else
                  {
                      gettime(buff.BUF[buff.cnt - 1].Data, &tempStrain);
                  }
            // tmpstr= QString("%1").arg( frameinfo[i].ID&0xFF,8,16,QLatin1Char('0'));
                  //更新ID
                  textTemp +=QString("##") + QString("%1").arg(buff.id&0xFF,8,16,QLatin1Char('0'));

                  //更新日期
                  textTemp+=QString("##") + QString("%1#%2#%3").arg( tempStrain.time->tm_year + 1900)
                                                                     .arg(1 + tempStrain.time->tm_mon)
                                                                    .arg(tempStrain.time->tm_mday);

                  //更新时间
                  textTemp+=QString("##") + QString("%1#%2#%3").arg(tempStrain.time->tm_hour)
                                                                    .arg( tempStrain.time->tm_min)
                                                                    .arg( tempStrain.time->tm_sec);

                  //更新毫秒
                  textTemp +=QString("##") + QString("%1").arg(tempStrain.time_ms);

                  //更新应变量
                  textTemp +=QString("##") + QString("%1").arg(tempStrain.strain);

                  qDebug() << textTemp;
                  /*
                   * <id></id>
                   * <date></date>
                   * <time></time>
                   * <ms>/<ms>
                   * <data></data>
                   *
                   *
                   *
                   */






      int gTD = 0; //标识通道ID  1-N
//      QString strID;
//      switch(gCAN_BUF[0])
//      {
//          case UP_DATA:
//              temp = (int *)(&gCAN_BUF[2]);
//              textTemp = QString("%1").arg(*temp);
//             gTD =  m_ID_map[strID]*STRAINGAUGES_CH_NUM + gCAN_BUF[1];
//              break;
//      }

return;

    QString strRet = QString("<?xml version='1.0' encoding ='utf8'?><body type = '1004'><td ID='%1'>%2</td></body>").arg(gTD).arg(textTemp);

    /*
     * 插入 数据库 存储队列
     */
     QTime curTime;
    QString strSql =   QString("('3','yl','\%1','sm',\"%2\",'1')").arg(curTime.currentTime().toString(),strRet);
      UserBuffer::GetInstance()->PushDatabaseQueue(detection_jgyl,strSql);
//        //

      //分发
       strRet = QString("being#%1#%2#").arg("1004").arg(strRet.length())+strRet+QString("#end#");

       /*
        * 更新发送缓冲
        */
       UserBuffer::GetInstance()->WriteDeviceToClientMap(m_strIp,strRet);

}
#endif
void DataAnalyzeThread::run()
{


        static int ch = STRAINGAUGES_CH1;
        strainDeviceData_T tempStrain ;
        QString textTemp;

        QString  strID,strData,strDate,strTime,strMs;
        /*标识ID
         * 应变力数据
         * 日期  y:m:d
         * 时间  h:m:s
         * 毫秒
         */


       int nowIndex;
       device_buf_T gSaveFifo_buf;
       QString strSendData; //拼装多个数据包
    while(m_stopFlag)
    {


        strSendData  = "";
        QString strDhData;
        for(int index = STRAINGAUGES_CH1;index<STRAINGAUGES_CH_NUM;++index)
        {

               // gSaveFifo_buf = UserBuffer::GetInstance()->PopCanDeviceQueue();
                gSaveFifo_buf = m_gSaveFifo_buf[index];
                textTemp = "";
                if(0 == gSaveFifo_buf.id)
                {
                    msleep(10);
                    continue;
                }
                nowIndex = gSaveFifo_buf.cnt;

             //计算更新频率，暂时不需要
            /*
                //计算数据速率
                for (i = 0; i < 4; i++)
                {
                    gettime(m_gSaveFifo_buf[ch].BUF[nowIndex-1].Data, &tempStrain);
                    if (nowIndex)
                        nowIndex--;
                    else
                        nowIndex = FIFO_LEN;
                    lastMs[i] = tempStrain.time_ms;
                }

                j = 0;
                temp = 0;
                for (i = 0; i < 3; i++)
                {
                    if (lastMs[i] > lastMs[i + 1])
                        temp += lastMs[i] - lastMs[i + 1];
                    else
                        break;
                    j++;
                }
                if (j > 2)
                {
                    temp /= j;
                    temp = 1000 / temp;

                    dispHz = temp;
                }

                //更新频率
                textTemp +=QString("##") + QString("%1").arg(dispHz);

    */
                if (gSaveFifo_buf.cnt == 0)
                {
                    gettime(gSaveFifo_buf.BUF[FIFO_LEN - 1].Data, &tempStrain);
                }
                else
                {
                    gettime(gSaveFifo_buf.BUF[gSaveFifo_buf.cnt - 1].Data, &tempStrain);
                }
          // tmpstr= QString("%1").arg( frameinfo[i].ID&0xFF,8,16,QLatin1Char('0'));
                //更新ID
               // textTemp +=QString("##") + QString("%1").arg(m_gSaveFifo_buf[ch].id&0xFF,8,16,QLatin1Char('0'));
                 strID =QString("%1").arg(gSaveFifo_buf.id);

                 //更新日期 年月日
                strDate=QString("%1/%2/%3").arg( tempStrain.time->tm_year + 1900)
                                                                   .arg(1 + tempStrain.time->tm_mon,2,10,QLatin1Char('0'))
                                                                  .arg(tempStrain.time->tm_mday,2,10,QLatin1Char('0'));


                //更新时间 时分秒
                strTime=QString("%1:%2:%3").arg(tempStrain.time->tm_hour,2,10,QLatin1Char('0'))
                                                                  .arg( tempStrain.time->tm_min,2,10,QLatin1Char('0'))
                                                                  .arg( tempStrain.time->tm_sec,2,10,QLatin1Char('0'))
                                                                  ;

                //更新毫秒
                strMs=QString("%1").arg(tempStrain.time_ms);

                //更新应变量
                strData=QString("%1").arg(tempStrain.strain);

                ch = (gSaveFifo_buf.id - START_ID) % 4;

                QString strRet = QString("<?xml version='1.0' encoding ='utf8'?><body type = '1004' belongs='%3'><td ID='%1'>%2</td></body>")
                        .arg(ch).arg(strData).arg(Config::GetInstance()->GetLifterIDByDeviceIp(m_strIp));

                //分发
                 strRet = QString("being#%1#%2#").arg("1004").arg(strRet.length())+strRet+QString("#end#");
                strSendData += strRet;
                strDhData +=  QString("<passageway state='1' ID='%1' >%2</passageway>").arg(ch,2,10,QLatin1Char('0')).arg(strData);

//不自动设置
//                /*
//                 * 判断 时期 看是否需要同步
//                 * dd.MM.yyyy
//                 */

//                QDateTime curTime = QDateTime::currentDateTime();
//                if(curTime.toString(QString("yyyy/MM/dd")) != strDate)
//                {
//                     TongDate(360);
//                }

        }

        strDhData = QString("<data_type ID='%1'>%2</data_type>").arg(Config::GetInstance()->GetDeviceID(m_strIp)).arg(strDhData);
        //存储 动画 客户端 数据包
        UserBuffer::GetInstance()->WriteDeviceToDhData(m_strIp,strDhData);
        /*
         * 更新发送缓冲
         */
        UserBuffer::GetInstance()->WriteDeviceToClientMap(m_strIp,strSendData);

         msleep(10);
    }
}


//同步时间
bool DataAnalyzeThread::TongDate(UINT32 id)
{
    // TODO: 在此添加控件通知处理程序代码
    VCI_CAN_OBJ frameinfo;
   // char szFrameID[9] = "00000168";
   // unsigned char FrameID[4] = { 0,0,0,0 };
    unsigned char Data[8], DTEMP[8];
    char szData[25];
    BYTE datalen = 0;
    int sjybl;
    time_t time_sec_temp;
    //设置帧ID转换成10进制存入FrameID备用
   // strtodata((unsigned char*)szFrameID, FrameID, 4, 0);

    datalen = 5;
    frameinfo.DataLen = datalen;
    Data[0] = SET_TIME;

    //取时间
    time_sec_temp = time(NULL);
    Data[1] = (time_sec_temp >> 24) & 0xff;
    Data[2] = (time_sec_temp >> 16) & 0xff;
    Data[3] = (time_sec_temp >> 8) & 0xff;
    Data[4] = time_sec_temp & 0xff;

    memcpy(&frameinfo.Data, Data, datalen);
    frameinfo.RemoteFlag = 0;
    frameinfo.ExternFlag = 0;
    if (frameinfo.ExternFlag == 1)
    {
//        frameinfo.ID = ((DWORD)FrameID[0] << 24) + ((DWORD)FrameID[1] << 16) + ((DWORD)FrameID[2] << 8) +
//            ((DWORD)FrameID[3]);
        frameinfo.ID = id;
    }
    else
    {
        //frameinfo.ID = ((DWORD)FrameID[2] << 8) + ((DWORD)FrameID[3]);
        frameinfo.ID = id;
    }

    //同步时间
    if (m_connectThread->m_VCI_Transmit!=NULL &&
            m_connectThread->m_VCI_Transmit(m_connectThread->m_devtype, m_connectThread->m_devind, m_connectThread->m_cannum, &frameinfo, 1) == 1)
    {

        msleep(100);
        datalen = 3;
        frameinfo.DataLen = datalen;
        Data[0] = SET_RATE;
        Data[1] = 0;
        Data[2] = 1000;
        memcpy(&frameinfo.Data, Data, datalen);
        //设置同步速率
        if(m_connectThread->m_VCI_Transmit(m_connectThread->m_devtype, m_connectThread->m_devind, m_connectThread->m_cannum, &frameinfo, 1) == 1)
          {
            msleep(100);
            UINT32 PASWD = 0x12345678;

            datalen = 5;
            frameinfo.DataLen = datalen;
            Data[0] = SET_CLEAR;

            //取时间
            Data[1] = (PASWD >> 24) & 0xff;
            Data[2] = (PASWD >> 16) & 0xff;
            Data[3] = (PASWD >> 8) & 0xff;
            Data[4] = PASWD & 0xff;

            memcpy(&frameinfo.Data, Data, datalen);
            //清0
            if(m_connectThread->m_VCI_Transmit(m_connectThread->m_devtype, m_connectThread->m_devind, m_connectThread->m_cannum, &frameinfo, 1) == 1)
                return true;
            }
    }
    VCI_ERR_INFO errinfo;
    m_connectThread->m_VCI_ReadErrInfo(m_connectThread->m_devtype,m_connectThread->m_devind,m_connectThread->m_cannum,&errinfo);
    return false;
}

void DataAnalyzeSaveDB::run()
{
    const int canBuffLen = CAN_REC_DATALEN;
    QString  strID,strData,strDate,strTime,strMs;
    int retCanBuffLen = -1;
    VCI_CAN_OBJ  canBuff[canBuffLen] = {0} ;
    strainDeviceData_T tempStrain ;
    while(m_stopFlag)
    {
        retCanBuffLen = BusinessSession::GetInstance()->ReadCanModuleBuffer(m_strIp,canBuff,canBuffLen);
         /*
        * 插入 数据库 存储队列
        */
        for(int index = 0; index < retCanBuffLen; ++index)
        {
            QDateTime currentT = QDateTime::currentDateTime();


            gettime(canBuff[index].Data, &tempStrain);

            if(0 == tempStrain.time)
                continue;

      // tmpstr= QString("%1").arg( frameinfo[i].ID&0xFF,8,16,QLatin1Char('0'));
            //更新ID
           // textTemp +=QString("##") + QString("%1").arg(m_gSaveFifo_buf[ch].id&0xFF,8,16,QLatin1Char('0'));
             strID =QString("%1").arg(canBuff[index].ID);

             //更新日期 年月日
            strDate=QString("%1/%2/%3").arg( tempStrain.time->tm_year + 1900)
                                                               .arg(1 + tempStrain.time->tm_mon,2,10,QLatin1Char('0'))
                                                              .arg(tempStrain.time->tm_mday,2,10,QLatin1Char('0'));

            //更新时间 时分秒
            strTime=QString("%1:%2:%3").arg(tempStrain.time->tm_hour,2,10,QLatin1Char('0'))
                                                              .arg( tempStrain.time->tm_min,2,10,QLatin1Char('0'))
                                                              .arg( tempStrain.time->tm_sec,2,10,QLatin1Char('0'))
                                                              ;

            //更新毫秒
            strMs=QString("%1").arg(tempStrain.time_ms);

            //更新应变量
            strData=QString("%1").arg(tempStrain.strain);
            QString strSql =   QString("('%1','%2','%3','%4','%5','%6','%7','%8')")
                    .arg(m_strDeviceID)
                    .arg(strID)
                    .arg(strDate)
                    .arg(strTime)
                    .arg(strMs)
                    .arg(strData)
                    .arg(currentT.toString("yyyy/MM/dd"))
                    .arg(currentT.toString("hh:mm:ss.zzz"))
                    ;
            UserBuffer::GetInstance()->PushDatabaseQueue(device_can,strSql);



        }
        msleep(10);
    }
}

  void DataConnectionThread::run()
 {

     try{



         //初始化失败继续等待初始化
         while(m_stopFlag&&!m_initFlag)
         {
			 if (!InitCANModule())
			 {
				 qDebug() << GetLastError();
				 
			 }
			 ::Sleep(3000);
         }



         const int buffLen = CAN_REC_DATALEN;
         VCI_CAN_OBJ frameinfo[buffLen];
         VCI_ERR_INFO errinfo;
         int len=1;
         int i=0;

         unsigned int ch = 0;
         int errorCount = 0;
         while(m_stopFlag)
         {
             while(m_controlStop)
                 msleep(1000);

             len=m_VCI_Receive(m_devtype,m_devind,m_cannum,frameinfo,buffLen,200);

             if(len<=0)
             {
                 //注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
                 //千万不能省略这一步（即使你可能不想知道错误码是什么）
                 m_VCI_ReadErrInfo(m_devtype,m_devind,m_cannum,&errinfo);
                 ++errorCount;
                 if(errorCount>=10)
                 {
                     if(m_VCI_CloseDevice)
                         m_VCI_CloseDevice(m_devtype,m_devind);
                    if(InitCANModule())
                        errorCount = 0;
                 }

             }
             else
             {

                 if (((frameinfo[i].ID - START_ID) >= 0) && ((frameinfo[i].ID - START_ID) <= MAX_CHANNEL))
                    BusinessSession::GetInstance()->WriteCanModuleBuffer(m_strIp,frameinfo,len);

                  errorCount = 0;
                 for (i = 0; i < len; i++)
                 {

                     if (((frameinfo[i].ID - START_ID) >= 0) && ((frameinfo[i].ID - START_ID) <= MAX_CHANNEL))
                     {
                         ch = (frameinfo[i].ID - START_ID) % 4;
                         m_gSaveFifo_buf[ch].id = frameinfo[i].ID;
                         for (int j = 0; j < 8; j++)
                             m_gSaveFifo_buf[ch].BUF[m_gSaveFifo_buf[ch].cnt].Data[j] = frameinfo[i].Data[j];

                         m_gSaveFifo_buf[ch].cnt++;
                         if (m_gSaveFifo_buf[ch].cnt >= FIFO_LEN)
                             m_gSaveFifo_buf[ch].cnt = 0;
                     }
                 }
               }

             }

     }catch(...)
    {
         throw -1;
     }
 }

