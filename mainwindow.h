#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "mbcomitem.h"
#include "modbuscommunicator.h"
#include "motorview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:


private:
    Ui::MainWindow *ui;

    QVector<MbComMotor*> motors;
    MbComSystem* sys;
    MbComSpeed* speed;
    ModbusCommunicator* modbus;

    QVector<MotorView*> motorViews;

};
#endif // MAINWINDOW_H
