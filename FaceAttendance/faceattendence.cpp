#include "faceattendence.h"
#include "ui_faceattendence.h"
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>


FaceAttendence::FaceAttendence(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FaceAttendence)
{
    ui->setupUi(this);
    //打开摄像头
    cap.open(0);//windows
    startTimer(100);

    //导入级联分类器文件
    cascade.load("E:\\code\\study_data_project\\Arm+Qt+opencv\\opencv452\\etc\\haarcascades\\haarcascade_frontalface_alt2.xml");


    //绑定定时器槽函数
    connect(&mtimer,&QTimer::timeout,this,&FaceAttendence::timer_connect);


    connect(&msocket,&QTcpSocket::disconnected,this, &FaceAttendence::start_connect);
    connect(&msocket,&QTcpSocket::connected,this, &FaceAttendence::stop_connect);

    //关联接收数据的槽函数
    connect(&msocket, &QTcpSocket::readyRead,this, &FaceAttendence::recv_data);

    mtimer.start(5000);


    flag=0;
    ui->widgetLb->hide();

}

FaceAttendence::~FaceAttendence()
{
    delete ui;
}

void FaceAttendence::timerEvent(QTimerEvent *e)
{
    //采集数据
    Mat srcImage;
    if(cap.grab())
    {
        cap.read(srcImage);//读取一帧的数据
    }

//    //把图片大小设与显示窗口一样大
//    cv::resize(srcImage,srcImage,Size(480,480));

    vector<Rect> faceRects;
    //检测函数  图片使用灰度增加检测速度
    Mat grayImage;
    cvtColor(srcImage,grayImage,COLOR_BGRA2GRAY);
    cascade.detectMultiScale(grayImage,faceRects);
    if(faceRects.size()>0&&flag>=0)
    {
        //绘制人脸
        Rect rect = faceRects.at(0);
        //未检测到人脸，把人脸移动到中心位置

        //移动人脸框
        ui->headpicLb->move(rect.x,rect.y);

        //发送人脸数据 mat->jpg->转换为QbyteArray
        if(flag>2)
        {
            //转换为jpg格式
            //存储编码后的数据
            std::vector<uchar> buf;
            imencode(".jpg",srcImage,buf);
            //转换为QbyteArray
            // 传入存储图像的数据指针， 和存储图像的数据大小  buf.data 返回指向容器的第一个元素的指针
            QByteArray byte((const char*)buf.data(),buf.size());

            //准备发送
            quint64 backsize = byte.size();
            QByteArray sendData;
            //Qt中用于序列化和反序列化数据的工具类，将数据以二进制形式进行读写。
            QDataStream stream(&sendData,QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_14);
            stream<<backsize<<byte;
            //发送
            msocket.write(sendData);
            flag=-2;
            faceMat = srcImage(rect);
            cv::imwrite("./face.jpg",faceMat);
        }
        flag++;
    }
    if(faceRects.size() == 0)
      {
          //把人脸框移动到中心位置
          ui->headpicLb->move(100,60);
          ui->widgetLb->hide();
          flag=0;
      }
    if(srcImage.data == nullptr) return;
       //把opencv里面的Mat格式数据（BGR）转Qt里面的QImage(RGB)
       cvtColor(srcImage,srcImage, COLOR_BGR2RGB);
       QImage image(srcImage.data,srcImage.cols, srcImage.rows,srcImage.step1(),QImage::Format_RGB888);
       QPixmap mmp = QPixmap::fromImage(image);
       ui->videoLb->setPixmap(mmp);

}






void FaceAttendence::recv_data()
{
    //通过json字符串的形式发送的数据{employeeID:%1,name:%2,department:软件,time:%3}
    //读取数据
    QByteArray array = msocket.readAll();
    qDebug()<<array;
    //对数据进行解析
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(array,&err);
    if(err.error != QJsonParseError::NoError)
    {
        qDebug()<<"json数据错误";
        return;
    }

    QJsonObject obj = doc.object();
    QString employeeID = obj.value("employeeID").toString();
    QString name = obj.value("name").toString();
    QString department = obj.value("department").toString();
    QString timestr = obj.value("time").toString();

    ui->numberEdit->setText(employeeID);
    ui->nameEdit->setText(name);
    ui->departmentEdit->setText(department);
    ui->timeEdit->setText(timestr);

    //通过样式来显示图片
    ui->headLb->setStyleSheet("border-radius:75px;border-image: url(./face.jpg);");
    ui->widgetLb->show();
}

void FaceAttendence::timer_connect()
{
    //连接服务器
    msocket.connectToHost("10.70.109.52",9999);
}

void FaceAttendence::stop_connect()
{
    mtimer.stop();
    qDebug()<<"成功连接服务器";
}

void FaceAttendence::start_connect()
{
    mtimer.start(5000);//启动定时器
    qDebug()<<"断开连接";
}
