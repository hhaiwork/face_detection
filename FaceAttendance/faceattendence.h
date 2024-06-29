#ifndef FACEATTENDENCE_H
#define FACEATTENDENCE_H

#include <QMainWindow>
#include <opencv.hpp>
//添加相关文件
#include <QTcpSocket>
#include <QTimer>




using namespace std;
//C++中OpenCV库的命名空间声明，OpenCV库中的所有函数和类引入到当前作用域。以便可以直接使用它们而无需加上前缀cv::
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class FaceAttendence; }
QT_END_NAMESPACE

class FaceAttendence : public QMainWindow
{
    Q_OBJECT


//类的公有接口应当包含所有用户需要调用的函数。为用户提供清晰的接口供用户与类交互。
public:
    FaceAttendence(QWidget *parent = nullptr);
    ~FaceAttendence();
    //定时器事件
    void timerEvent(QTimerEvent *e);

//成员函数：那些仅供类内部使用，对类的用户没有意义的函数，应当声明为private。
//通常将成员变量声明为private,以隐藏类的实现细节。这可以防止外部代码直接修改这些变量，从而保护数据的完整性。
protected slots:
    void recv_data();


//通过槽函数 来控制连接
private slots:
    void timer_connect();
    void stop_connect();
    void start_connect();




private:
    Ui::FaceAttendence *ui;
    //声明捕获视频的类
    VideoCapture cap;
    //haar -- 级联分类器
    cv::CascadeClassifier cascade;
    //创建套接字，定时器
    QTcpSocket msocket;
    QTimer mtimer;


    //标志是否是同一个人脸进入到识别区域
    int flag;

    //保存人类的数据
    cv::Mat faceMat;

};
#endif // FACEATTENDENCE_H
