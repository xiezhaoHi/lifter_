#include "QtShowWork.h"
#include <QDomDocument>
#include "usebuffer.h"
#include <QDebug>
#include<QStandardItemModel>
#include<QMap>
//单位都是cm
static double gSizeR = 10;//圈的半径
static double gSizeP = 1000; //脉冲 刻度

QtShowWork::QtShowWork(QObject *parent)
	: QObject(parent)
{

}

QtShowWork::~QtShowWork()
{
}
//解析 数据包队列
void QtShowWork::analyseAllData()
{
	QtShowWork* p = this;
	QDomDocument doc;
	
	while (UserBuffer::GetInstance()->ReturnClientServerQueueSize()>0)
	{
		QString strData = UserBuffer::GetInstance()->PopClientServerQueue();

		QString error;
		int row = 0, column = 0;
		QString strDir; //编码器方向
		double  dZs, dWz, dZdjl; //编码器转速 和位置 制动距离
		if (!doc.setContent(strData, false, &error, &row, &column))
		{
			qDebug() << "解析数据错误:" << strData;
			continue;
		}
		// qDebug() << "ui" << strData;
		QDomElement root = doc.firstChildElement();
		QDomElement child, rootFist;
		if (!root.hasAttribute(QString("type")))
			return;
		QString strBelongs = root.attribute(QString("belongs")); //找类型
																 /*
																 所属电梯
																 0001...(曳引式简易升降机)
																 0002...(强制式简易升降机)
																 0003...(sc双笼施工升降机)
																 */
		QMap<QString, QLabel*>* map_id = nullptr/*普通开关量*/
			, *map_id_aqcd = nullptr;/*安全触点开关量*/;
		QMap<int, QLabel*> *map_cgq = nullptr; /*传感器 数据类型 对应 控件*/;
		QMap<int, QLabel*> *map_bmq = nullptr; /*编码器 数据类型 对应 控件*/;
		QStandardItemModel* pModel = nullptr;

		//继电器DO状态
		QMap<QPushButton*,QString >* map_button_DO = nullptr;
		QMap<QPushButton*, BtAttribute*>* mapTempAttri = nullptr;//中间map变量 属性
		/*电源电压电流显示数据*/
		QLabel*** pppLabelDydlShow = nullptr;
		QPushButton* qiD_button = nullptr; //启动的按钮
		BmqDataS* pDataBmq = nullptr;
		if (LIFTER_SC_ID_A == strBelongs) //表示 笼A
		{
			mapTempAttri = &(m_pMainwindow->m_BtAttribute_left);
			map_button_DO = &(m_pMainwindow->m_button_ID_left);
			map_id = &(m_pMainwindow->m_label_ID_to_ctl_left);
			map_id_aqcd = &(m_pMainwindow->m_label_ID_to_aqcd_left);
			map_cgq = &(m_pMainwindow->m_cgq_to_label_left);
			map_bmq = &(m_pMainwindow->m_bmq_to_label_left);
			pModel = m_pMainwindow->m_yl_tableV_mode_left;
			pppLabelDydlShow = (QLabel***)m_pMainwindow->m_label_dydl_show_left;
			qiD_button = m_pMainwindow->m_ID_button_left[JDQ_DO_flag_qiD];
			pDataBmq = &(m_pMainwindow->m_dataBmq_left);
		}
		if (LIFTER_SC_ID_B == strBelongs)//表示 笼B
		{
			mapTempAttri = &(m_pMainwindow->m_BtAttribute_right);
			map_button_DO = &(m_pMainwindow->m_button_ID_right);
			map_id = &(m_pMainwindow->m_label_ID_to_ctl_right);
			map_id_aqcd = &(m_pMainwindow->m_label_ID_to_aqcd_right);
			map_cgq = &(m_pMainwindow->m_cgq_to_label_right);
			map_bmq = &(m_pMainwindow->m_bmq_to_label_right);
			pModel = m_pMainwindow->m_yl_tableV_mode_right;
			pppLabelDydlShow = (QLabel***)m_pMainwindow->m_label_dydl_show_right;
			qiD_button = m_pMainwindow->m_ID_button_right[JDQ_DO_flag_qiD];
			pDataBmq = &(m_pMainwindow->m_dataBmq_right);
		}
		int typeFlag = root.attribute(QString("type")).toInt(); //找类型
		rootFist = root;
		if (!root.hasAttribute(QString("state"))) //没有state 属性 表示 不是退出
		{

			QStringList strList;
			if (device_cgq == typeFlag) //传感器
			{
				root = rootFist.firstChildElement(QString("jsd")); //加速度 节点开始
				if (!root.isNull()) //节点有效
				{
					child = root.firstChildElement(QString("x"));
					if (!child.isNull())
						strList.append(child.text());
					child = root.firstChildElement(QString("y"));
					if (!child.isNull())
						strList.append(child.text());
					child = root.firstChildElement(QString("z"));
					if (!child.isNull())
						strList.append(child.text());

					root = rootFist.firstChildElement(QString("jd")); //角度 节点开始
					child = root.firstChildElement(QString("x"));
					if (!child.isNull())
						strList.append(child.text());
					child = root.firstChildElement(QString("y"));
					if (!child.isNull())
						strList.append(child.text());
					child = root.firstChildElement(QString("z"));
					if (!child.isNull())
						strList.append(child.text());

					emit showCgqData(strList, *map_cgq);
				}
			}
			if (device_jdq == typeFlag) //继电器
			{
				QMap<QString, int>   device_map; //DI 关联的设备 与对应的值 0 1
				root = rootFist.firstChildElement(QString("DI")); //找第一个子节点
				if (!root.isNull())
				{
					while (!root.isNull())
					{
						if (root.hasAttribute(QString("LINK")))
							device_map[root.attribute(QString("LINK"))] = root.text().toInt();
						root = root.nextSiblingElement();
					}
					emit showJdqData(device_map, map_id, map_id_aqcd, qiD_button);
// 					emit showTest(device_map, *map_id, *map_id_aqcd);
// 					emit showTest4();
// 					emit showTest2(map_id);
				}
				else
				{
					//DO 模块返回当前继电器状态数据
					root = rootFist.firstChildElement(QString("DO"));
					if (!root.isNull())
					{
						while (!root.isNull())
						{
							if (root.hasAttribute(QString("LINK")))
								device_map[root.attribute(QString("LINK"))] = root.text().toInt();
							root = root.nextSiblingElement();
						}
						emit showJdqDataDO(device_map, map_button_DO, mapTempAttri);
					}
					//qDebug() << root.text();
				}
			}
			if (device_can == typeFlag) //应力通道数据 CAN模块
			{
				root = root.firstChildElement(); //找第一个子节点
				while (!root.isNull())
				{
					if (root.hasAttribute(QString("ID")))
						emit showYlData(root.attribute(QString("ID")), root.text(), pModel);
					root = root.nextSiblingElement();
				}
			}
			if (device_bmq == typeFlag) //编码器
			{

				root = rootFist.firstChildElement(QString("dir")); //找dir子节点 方向
				if (!root.isNull())
				{
					if (lifter_dir_up == root.text().toInt()) //正向
						strDir = "上升";
					else if(lifter_dir_down == root.text().toInt())
						strDir = "下降";
					else
						strDir = "静止";

				}
				root = rootFist.firstChildElement(QString("jd")); //找jd子节点 角度
				if (!root.isNull())
				{

				}

				root = rootFist.firstChildElement(QString("zs")); //找zs子节点 转速
				if (!root.isNull())
				{

					dZs = root.text().toDouble();
					dZs = dZs*gSizeR * 2 * 3.14 / 60.0;
				}
				root = rootFist.firstChildElement(QString("jsz")); //找jsz子节点 计数值
				if (!root.isNull())
				{
					dWz = root.text().toDouble();
					//dWz = dWz * 0.04;
				}
				root = rootFist.firstChildElement(QString("zdjl")); //找zdjl子节点 制动距离

				if (!root.isNull())
				{
					dZdjl = root.text().toDouble();
				}
				
				pDataBmq->m_iWz = dWz;
				pDataBmq->m_strDir = strDir;
				emit showBmpData(pDataBmq, map_bmq);
				emit showTest4();
			}
			if (device_server_res == typeFlag) //表示 服务器返回的 回应包
			{
				root = rootFist.firstChildElement(QString("ret"));
				if (!root.isNull())
				{
					//填充 服务器 应答队列
					UserBuffer::GetInstance()->PushServerResponse(root.text().toInt());

				}

			}

			//新增电源 数据显示
			if (device_dydl == typeFlag)
			{
				root = rootFist.firstChildElement(QString("state")); //状态标识
				if (!root.isNull())
				{
					QString strStatus = root.text();
					//正常
					if ("0" == strStatus)
					{
						root = rootFist.firstChildElement(QString("dataFlag")); //状态标识
						if (!root.isNull())
						{
							int  dataIndex = root.text().toInt(); //数组位置标识  电压 、电流、频率
							QString strDataA, strDataB, strDataC; //3相a b c

							root = rootFist.firstChildElement(QString("data_a")); //状态标识
							if (!root.isNull())
							{
								strDataA = root.text();
							}
							root = rootFist.firstChildElement(QString("data_b")); //状态标识
							if (!root.isNull())
							{
								strDataB = root.text();
							}
							root = rootFist.firstChildElement(QString("data_c")); //状态标识
							if (!root.isNull())
							{
								strDataC = root.text();
							}
							//数据有效
							if (dataIndex != -1 && dataIndex > DYDATA_SHOW_DY && dataIndex<DYDATA_SHOW_max)
							{
								emit showdyData(dataIndex, strDataA, strDataB, strDataC, pppLabelDydlShow);
							}

						}
					}
					else //异常
					{

					}
				}
			}
		}
		else //表示 退出
		{
			if (device_jdq == typeFlag) //继电器退出
			{
				QMap<QString, int>   device_map; //DI 关联的设备 与对应的值 0 1
				root = rootFist.firstChildElement(); //找第一个子节点
				while (!root.isNull())
				{
					if (root.hasAttribute(QString("LINK")))
						device_map[root.attribute(QString("LINK"))] = root.text().toInt();
					root = root.nextSiblingElement();
				}
				emit showJdqData(device_map, map_id, map_id_aqcd,qiD_button);

			}
		}

	}
}

void QtShowWork::analyseBmqData()
{

}

//解析数据 并显示数据
void QtShowWork::consumer()
{

}