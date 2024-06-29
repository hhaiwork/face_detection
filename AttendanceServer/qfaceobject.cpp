#include "qfaceobject.h"
#include <QDebug>
QFaceObject::QFaceObject(QObject *parent) : QObject(parent)
{
    //初始化
    seeta::ModelSetting FDmode("E:\\code\\study_data_project\\Arm+Qt+opencv\\SeetaFace\\bin\\model\\fd_2_00.dat");
    seeta::ModelSetting PDmode("E:\\code\\study_data_project\\Arm+Qt+opencv\\SeetaFace\\bin\\model\\pd_2_00_pts5.dat");
    seeta::ModelSetting FRmode("E:\\code\\study_data_project\\Arm+Qt+opencv\\SeetaFace\\bin\\model\\fr_2_10.dat");
    this->fengineptr= new seeta::FaceEngine(FDmode,PDmode,FRmode);

    //导入已有的人脸数据库
    this->fengineptr->Load("./face.db");

}


QFaceObject::~QFaceObject()
{
    delete fengineptr;
}


//注册数据 主要过程为格式的转换
int64_t QFaceObject::face_register(cv::Mat &faceImage){
    //将数据转换为seetaface能够识别的图片格式
    SeetaImageData simage;
    simage.data = faceImage.data;
    simage.width = faceImage.cols;
    simage.height = faceImage.rows;
    simage.channels = faceImage.channels();
    //register返回一个int_64的数
    //this->fengineptr

    int64_t faceid=this->fengineptr->Register(simage);
    if(faceid>=0){
        //保存的路径，传入字符串保存的路径
        fengineptr->Save("./face.db");

    }
    return  faceid;
}


int QFaceObject::face_query(cv::Mat &faceImage)
{
    //把opencv的Mat数据转为seetaface的数据
    qDebug()<<"进行人脸比对";
    SeetaImageData simage;
    simage.data = faceImage.data;
    simage.width = faceImage.cols;
    simage.height = faceImage.rows;
    simage.channels = faceImage.channels();
    //提供一个similarity变量
    float similarity=0;
    //&similarity表示取地址
    //similarity表示变量本身，如果similarity 是一个指针类型的变量， similarity 就是指向其保存的地址值，基本数据类型或者其他非指针类型，则直接代表它的值。

    int64_t faceid = fengineptr->Query(simage,&similarity);//运行时间比较长

    qDebug()<<"查询"<<faceid<<similarity;
    if(similarity>0.5)
    {
        //emit用于触发信号的发射
        //发出信号
        emit send_faceid(faceid);
    }
    else
    {
        emit send_faceid(-1);
    }


    return faceid;
}





