#include "QtShowWork.h"
#include <QDomDocument>
#include "usebuffer.h"
#include <QDebug>
#include<QStandardItemModel>
#include<QMap>
//��λ����cm
static double gSizeR = 10;//Ȧ�İ뾶
static double gSizeP = 1000; //���� �̶�

QtShowWork::QtShowWork(QObject *parent)
	: QObject(parent)
{

}

QtShowWork::~QtShowWork()
{
}
//���� ���ݰ�����
void QtShowWork::analyseAllData()
{
	QtShowWork* p = this;
	QDomDocument doc;
	
	while (UserBuffer::GetInstance()->ReturnClientServerQueueSize()>0)
	{
		QString strData = UserBuffer::GetInstance()->PopClientServerQueue();

		QString error;
		int row = 0, column = 0;
		QString strDir; //����������
		double  dZs, dWz, dZdjl; //������ת�� ��λ�� �ƶ�����
		if (!doc.setContent(strData, false, &error, &row, &column))
		{
			qDebug() << "�������ݴ���:" << strData;
			continue;
		}
		// qDebug() << "ui" << strData;
		QDomElement root = doc.firstChildElement();
		QDomElement child, rootFist;
		if (!root.hasAttribute(QString("type")))
			return;
		QString strBelongs = root.attribute(QString("belongs")); //������
																 /*
																 ��������
																 0001...(ҷ��ʽ����������)
																 0002...(ǿ��ʽ����������)
																 0003...(sc˫��ʩ��������)
																 */
		QMap<QString, QLabel*>* map_id = nullptr/*��ͨ������*/
			, *map_id_aqcd = nullptr;/*��ȫ���㿪����*/;
		QMap<int, QLabel*> *map_cgq = nullptr; /*������ �������� ��Ӧ �ؼ�*/;
		QMap<int, QLabel*> *map_bmq = nullptr; /*������ �������� ��Ӧ �ؼ�*/;
		QStandardItemModel* pModel = nullptr;

		//�̵���DO״̬
		QMap<QPushButton*,QString >* map_button_DO = nullptr;
		QMap<QPushButton*, BtAttribute*>* mapTempAttri = nullptr;//�м�map���� ����
		/*��Դ��ѹ������ʾ����*/
		QLabel*** pppLabelDydlShow = nullptr;
		QPushButton* qiD_button = nullptr; //�����İ�ť
		BmqDataS* pDataBmq = nullptr;
		if (LIFTER_SC_ID_A == strBelongs) //��ʾ ��A
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
		if (LIFTER_SC_ID_B == strBelongs)//��ʾ ��B
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
		int typeFlag = root.attribute(QString("type")).toInt(); //������
		rootFist = root;
		if (!root.hasAttribute(QString("state"))) //û��state ���� ��ʾ �����˳�
		{

			QStringList strList;
			if (device_cgq == typeFlag) //������
			{
				root = rootFist.firstChildElement(QString("jsd")); //���ٶ� �ڵ㿪ʼ
				if (!root.isNull()) //�ڵ���Ч
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

					root = rootFist.firstChildElement(QString("jd")); //�Ƕ� �ڵ㿪ʼ
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
			if (device_jdq == typeFlag) //�̵���
			{
				QMap<QString, int>   device_map; //DI �������豸 ���Ӧ��ֵ 0 1
				root = rootFist.firstChildElement(QString("DI")); //�ҵ�һ���ӽڵ�
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
					//DO ģ�鷵�ص�ǰ�̵���״̬����
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
			if (device_can == typeFlag) //Ӧ��ͨ������ CANģ��
			{
				root = root.firstChildElement(); //�ҵ�һ���ӽڵ�
				while (!root.isNull())
				{
					if (root.hasAttribute(QString("ID")))
						emit showYlData(root.attribute(QString("ID")), root.text(), pModel);
					root = root.nextSiblingElement();
				}
			}
			if (device_bmq == typeFlag) //������
			{

				root = rootFist.firstChildElement(QString("dir")); //��dir�ӽڵ� ����
				if (!root.isNull())
				{
					if (lifter_dir_up == root.text().toInt()) //����
						strDir = "����";
					else if(lifter_dir_down == root.text().toInt())
						strDir = "�½�";
					else
						strDir = "��ֹ";

				}
				root = rootFist.firstChildElement(QString("jd")); //��jd�ӽڵ� �Ƕ�
				if (!root.isNull())
				{

				}

				root = rootFist.firstChildElement(QString("zs")); //��zs�ӽڵ� ת��
				if (!root.isNull())
				{

					dZs = root.text().toDouble();
					dZs = dZs*gSizeR * 2 * 3.14 / 60.0;
				}
				root = rootFist.firstChildElement(QString("jsz")); //��jsz�ӽڵ� ����ֵ
				if (!root.isNull())
				{
					dWz = root.text().toDouble();
					//dWz = dWz * 0.04;
				}
				root = rootFist.firstChildElement(QString("zdjl")); //��zdjl�ӽڵ� �ƶ�����

				if (!root.isNull())
				{
					dZdjl = root.text().toDouble();
				}
				
				pDataBmq->m_iWz = dWz;
				pDataBmq->m_strDir = strDir;
				emit showBmpData(pDataBmq, map_bmq);
				emit showTest4();
			}
			if (device_server_res == typeFlag) //��ʾ ���������ص� ��Ӧ��
			{
				root = rootFist.firstChildElement(QString("ret"));
				if (!root.isNull())
				{
					//��� ������ Ӧ�����
					UserBuffer::GetInstance()->PushServerResponse(root.text().toInt());

				}

			}

			//������Դ ������ʾ
			if (device_dydl == typeFlag)
			{
				root = rootFist.firstChildElement(QString("state")); //״̬��ʶ
				if (!root.isNull())
				{
					QString strStatus = root.text();
					//����
					if ("0" == strStatus)
					{
						root = rootFist.firstChildElement(QString("dataFlag")); //״̬��ʶ
						if (!root.isNull())
						{
							int  dataIndex = root.text().toInt(); //����λ�ñ�ʶ  ��ѹ ��������Ƶ��
							QString strDataA, strDataB, strDataC; //3��a b c

							root = rootFist.firstChildElement(QString("data_a")); //״̬��ʶ
							if (!root.isNull())
							{
								strDataA = root.text();
							}
							root = rootFist.firstChildElement(QString("data_b")); //״̬��ʶ
							if (!root.isNull())
							{
								strDataB = root.text();
							}
							root = rootFist.firstChildElement(QString("data_c")); //״̬��ʶ
							if (!root.isNull())
							{
								strDataC = root.text();
							}
							//������Ч
							if (dataIndex != -1 && dataIndex > DYDATA_SHOW_DY && dataIndex<DYDATA_SHOW_max)
							{
								emit showdyData(dataIndex, strDataA, strDataB, strDataC, pppLabelDydlShow);
							}

						}
					}
					else //�쳣
					{

					}
				}
			}
		}
		else //��ʾ �˳�
		{
			if (device_jdq == typeFlag) //�̵����˳�
			{
				QMap<QString, int>   device_map; //DI �������豸 ���Ӧ��ֵ 0 1
				root = rootFist.firstChildElement(); //�ҵ�һ���ӽڵ�
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

//�������� ����ʾ����
void QtShowWork::consumer()
{

}