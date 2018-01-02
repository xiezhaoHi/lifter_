#ifndef CONNECTIONS_H
#define CONNECTIONS_H


#include <set>
#include <algorithm>
#include <boost/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include<boost/thread.hpp>
#include<QMap>

using namespace boost;


class Mysession;

class Connections {
public:
    bool Add(boost::shared_ptr<Mysession>& con);

    void Remove(shared_ptr<Mysession> con);

    void CloseAll();

    //设备 会话
    QMap<QString,shared_ptr<Mysession>> m_device_map;


    //客户端会话
    QMap<QString,shared_ptr<Mysession>> m_client_map;

private:
    std::set<shared_ptr<Mysession> > values_;

    boost::mutex mtx;

    //分开管理 两种会话


};

#endif // CONNECTIONS_H
