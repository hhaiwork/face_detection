#include "attendancewin.h"
#include "ui_attendancewin.h"
#include <QDateTime>
#include <QSqlRecord>
#include <QThread>
#include <opencv.hpp>
#include <QSqlQuery>
#include <QSqlError>

AttendanceWin::AttendanceWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AttendanceWin)
{
    ui->setupUi(this);
    connect(&mserver,&QTcpServer::newConnection,this,&AttendanceWin::accept_client);
    mserver.listen(QHostAddress::Any,9999);
    bsize=0;
    //绑定sql模型
    model.setTable("employee");

    //通过线程来优化 提高检测速度
    QThread *thread =new QThread();
    //将对象移动线程中去
    fobj.moveToThread(thread);
    //开启线程
    thread->start();
    //将线程与槽函数相关联
    connect(this,&AttendanceWin::query,&fobj,&QFaceObject::face_query);
    //关联QFaceObject对象里面的send_faceid信号
    connect(&fobj,&QFaceObject::send_faceid,this,&AttendanceWin::recv_faceid);



}

AttendanceWin::~AttendanceWin()
{
    delete ui;
}

void AttendanceWin::accept_client()
{


    //从套接字中读取数据 ，并将数据集解码为QPixmap对象，展示在标签上

    //获取通信的套接字
    msocket=mserver.nextPendingConnection();

    //到达数据会产生信号readyRead
    connect(msocket,&QTcpSocket::readyRead,this,&AttendanceWin::read_data);

}

void AttendanceWin::recv_faceid(int64_t faceid)
{
    qDebug()<<"识别到的人脸id:"<<faceid;
    //查询faceid对应的人脸信息
    //设置模型过滤器
    if(faceid<0)
    {
        QString sdmsg=QString("{\"employeeID\":,\"name\":,\"department\":,\"time\":}");
        msocket->write(sdmsg.toUtf8());//将信息发送给客户端
        return ;
    }
    model.setFilter(QString("faceID=%1").arg(faceid));
    //查询
    model.select();
    if(model.rowCount()==1)
    {
        //工号，姓名，部门，时间
        //{employeeID:%1,name:%2,department:软件,time:%3}
        QSqlRecord record=model.record(0);
        QString sdmsg=QString("{\"employeeID\":\"%1\",\"name\":\"%2\",\"department\":\"软件\",\"time\":\"%3\"}")
                .arg(record.value("employeeID").toString()).arg(record.value("name").toString())
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        //将数据写入到考勤表
        QString insertSql = QString("insert into attendance(employeeID) values('%1')").arg(record.value("employeeID").toString());

        QSqlQuery query;

        if(!query.exec(insertSql))
        {
            QString sdmsg = QString("{\"employeeID\":\" \",\"name\":\"\",\"department\":\"\",\"time\":\"\"}");
            msocket->write(sdmsg.toUtf8());//把打包好的数据发送给客户端
            qDebug()<<query.lastError().text();
            return ;
        }
        else
        {
            msocket->write(sdmsg.toUtf8());//把打包好的数据发送给客户端
        }
    }
}

void AttendanceWin::read_data()
{

    qDebug()<<"读取到了数据";
    QDataStream stream(msocket);
    stream.setVersion(QDataStream::Qt_5_14);

    //通过bsize来存放数据的大小
    if(bsize==0){
        //msoket->bytesAvailable()，
        //(quint64)sizeof(bsize)：这是一个类型转换操作
        //检测数据的完整性，防止数据丢失，控制流大小
        if(msocket->bytesAvailable()<(quint64)sizeof(bsize)) return;
        //将stream中tream 指向的数据流中读取一个与 bsize 变量类型匹配的值
        //QDataStream 提供了一种方便且安全的方式来处理二进制数据的 I/O 操作
        //如果 bsize 是 quint64，则读取一个 quint64 类型的值），并将其存储到 bsize 变量中
        stream>>bsize;
    }
    //msoket->bytesAvailable()获取套接字中可用的字节数
    if(msocket->bytesAvailable()<bsize)//数据还未发送完成，继续等待
    {
        return;

    }
    QByteArray data;
    stream>>data;
    bsize=0;

    QPixmap mmp;
    mmp.loadFromData(data,"jpg");
    //将图像缩放到与标签相同的大小
    mmp=mmp.scaled(ui->picLb->size());
    ui->picLb->setPixmap(mmp);




    //识别人脸
    //将读取到的人脸数据存到一个decode对象之中，在将该对象编码成图像数据
    cv::Mat faceImage;
    std::vector<uchar> decode;
    decode.resize(data.size());
    //将一个容器数据复制到另外一个容器之中（对象）（对象）（复制的数量）
    memcpy(decode.data(),data.data(),data.size());
    faceImage = cv::imdecode(decode, cv::IMREAD_COLOR);

    //int faceid = fobj.face_query(faceImage); //消耗资源较多
    //发出该信号
    emit query(faceImage);





}










