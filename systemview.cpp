#include "systemview.h"
#include "ui_systemview.h"

SystemView::SystemView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SystemView)
    , sys(nullptr)
    , HeartBeatState(false)
{
    ui->setupUi(this);

    heartBeatTimer = new QTimer(this);
    heartBeatTimer->setInterval(500);
    heartBeatTimer->setSingleShot(false);
    heartBeatTimer->start();
    connect(heartBeatTimer, SIGNAL(timeout()), this, SLOT(onHeartBeatFire()));
    connect(ui->stop, SIGNAL(clicked(bool)), this, SLOT(onSystemStop(bool)));
    connect(ui->activateUp, SIGNAL(clicked(bool)), this, SLOT(onActivateUp(bool)));
    connect(ui->activateDown, SIGNAL(clicked(bool)), this, SLOT(onActivateDown(bool)));

}

SystemView::~SystemView()
{
    delete ui;
    delete heartBeatTimer;
}

void SystemView::setMbComItem(MbComSystem *sys)
{
    this->sys = sys;
    connect(this->sys, SIGNAL(stateUpdated()), this, SLOT(updateStates()));
    emit this->sys->stateUpdated();
}

void SystemView::onSystemStop(bool state)
{
    if(state){
        ui->stop->setText("Возобновить\nработу");
    }
    else{
        ui->stop->setText("Остановить\nсистему");
    }

    if(sys != nullptr){
        sys->setStop(state);
    }
}

void SystemView::onHeartBeatFire()
{
    HeartBeatState = !HeartBeatState;
    if (sys != nullptr){
        sys->setHeartBeatState(HeartBeatState);
    }

}

void SystemView::updateStates()
{
    if(sys != nullptr){
        //System ready
        if(sys->isSystemReady()){
            ui->isSystemWorking->setCheckState(Qt::Checked);
        }
        else{
            ui->isSystemWorking->setCheckState(Qt::Unchecked);
        }

        //System moving
        if(sys->isSystemMooving()){
            ui->isSystemMoving->setCheckState(Qt::Checked);
        }
        else{
            ui->isSystemMoving->setCheckState(Qt::Unchecked);
        }

        //System Error
        if(sys->isSystemError()){
            ui->IsSystemError->setCheckState(Qt::Checked);
        }
        else{
            ui->IsSystemError->setCheckState(Qt::Unchecked);
        }

        //System Stoped
        if(sys->isSystemEStop()){
            ui->isSystemEStoped->setCheckState(Qt::Checked);
        }
        else{
            ui->isSystemEStoped->setCheckState(Qt::Unchecked);
        }
    }
}

void SystemView::onActivateUp(bool state)
{
    if(state){
        ui->activateUp->setText("Деактивировать верхнюю\nгруппу приводов");
    }
    else{
        ui->activateUp->setText("Активировать верхнюю\nгруппу приводов");
    }

    if(sys != nullptr){
        sys->setActivateUp(state);
    }
}

void SystemView::onActivateDown(bool state)
{
    if(state){
        ui->activateDown->setText("Деактивировать нижнюю\nгруппу приводов");
    }
    else{
        ui->activateDown->setText("Активировать нижнюю\nгруппу приводов");
    }

    if(sys != nullptr){
        sys->setActivateDown(state);
    }
}
