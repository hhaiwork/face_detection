#ifndef ATTENDANCEWIN_H
#define ATTENDANCEWIN_H

#include "qfaceobject.h"
#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QSqlTableModel>
#include <QSqlRecord>

QT_BEGIN_NAMESPACE
namespace Ui { class AttendanceWin; }
QT_END_NAMESPACE

class AttendanceWin : public QMainWindow
{
    Q_OBJECT

public:
    AttendanceWin(QWidget *parent = nullptr);
    ~AttendanceWin();


//提供一个信号，当信号被触发时，会将图像矩阵传递给槽函数
signals:
    //引用传递  直接使用原始对象的地址，通过引用可以规避不必要的拷贝操作
    //无&则为值传递，会创建一个参数的副本
    void query(cv::Mat& image);

protected slots:
    void accept_client();
    void recv_faceid(int64_t faceid);
    void read_data();

private:
    Ui::AttendanceWin *ui;
    QTcpServer mserver;
    QTcpSocket *msocket;
    quint64 bsize;
    //通过引入线程来提升运行效率
    QFaceObject fobj;
    QSqlTableModel model;

};
#endif // ATTENDANCEWIN_H
