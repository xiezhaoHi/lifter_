#pragma once

#include <QThread>

//�������ָ����¥��
class CGoFloor : public QThread
{
public:
	CGoFloor(QString const& ,int const&);
	~CGoFloor();
	void run();
private:
	//����ID ���ڷ��� ��ͬ���� ������BMQ
	QString m_strLifterID;
	//���� ¥��� ����:1-7 ��
	int		m_floorID;
};
