#include "selectwin.h"
#include "ui_selectwin.h"

SelectWin::SelectWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectWin)
{
    ui->setupUi(this);
    model=new QSqlTableModel();
}

SelectWin::~SelectWin()
{
    delete ui;
}



void SelectWin::on_selectRt_clicked()
{
    if(ui->empBb->isChecked())
    {
        model->setTable("employee");
    }
    if(ui->attRb->isChecked())
    {
        model->setTable("attendance");
    }
    //设置过滤器
    // model->setFilter("name='张三'");
    // 查询

    model->select();
    ui->tableView->setModel(model);
}
