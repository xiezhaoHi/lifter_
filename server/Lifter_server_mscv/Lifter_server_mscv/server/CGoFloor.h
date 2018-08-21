#pragma once

#include <QThread>

//到达电梯指定的楼层
class CGoFloor : public QThread
{
public:
	CGoFloor(QString const& ,int const&);
	~CGoFloor();
	void run();
private:
	//电梯ID 用于分类 不同电梯 关联的BMQ
	QString m_strLifterID;
	//电梯 楼层号 例如:1-7 层
	int		m_floorID;
};
