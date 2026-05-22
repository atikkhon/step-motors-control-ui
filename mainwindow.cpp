#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , motors(22)
    , sys(new MbComSystem(this))
    , speed(new MbComSpeed(this))
    , motorViews(22)
{
    ui->setupUi(this);

    modbus = new ModbusCommunicator(0, 120, "127.0.0.1", 502, this);

    modbus->pushDevRead(sys);
    modbus->pushDevWrite(sys);
    modbus->pushDevWrite(speed);

    for (int i = 0; i < motors.size(); i++){
        motors[i] = new MbComMotor(this);
        modbus->pushDevRead(motors[i]);
        modbus->pushDevWrite(motors[i]);
    }
    ui->speedView->setMbComSpeed(speed);
    ui->systemView->setMbComItem(sys);
    ui->plcView->setModbusCommunicator(modbus);

    ui->scrollUpContent->setLayout(new QVBoxLayout(ui->scrollUpContent));
    ui->scrollDownContent->setLayout(new QVBoxLayout(ui->scrollDownContent));
    for(int i = 0; i < motorViews.size(); i++){
        motorViews[i] = new MotorView(this);
        motorViews[i]->setMotorName("Motor " + QString::number(i+1));
        motorViews[i]->setMbComItem(*(motors[i]));
        motorViews[i]->setMinimumSize(100, 100);
        motorViews[i]->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Maximum);
        if(i % 2 == 1){
            ui->scrollUpContent->layout()->addWidget(motorViews[i]);
        }
        else{

            ui->scrollDownContent->layout()->addWidget(motorViews[i]);
        }
    }
}

MainWindow::~MainWindow()
{
    for(int i = 0; i < motorViews.size(); i++){
        delete motorViews[i];
    }
    delete modbus;
    delete speed;
    delete sys;
    for(int i = 0; i < motors.size(); i++){
        delete motors[i];
    }
    delete ui;

}

