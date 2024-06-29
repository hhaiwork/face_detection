#ifndef SELECTWIN_H
#define SELECTWIN_H

#include <QWidget>
//绑定sql模型
#include <QSqlTableModel>


namespace Ui {
class SelectWin;
}

class SelectWin : public QWidget
{
    Q_OBJECT

public:
    explicit SelectWin(QWidget *parent = nullptr);
    ~SelectWin();

private slots:


    void on_selectRt_clicked();

private:
    Ui::SelectWin *ui;
    //声明sql模型
    QSqlTableModel *model;

};

#endif // SELECTWIN_H
