#include "motorview.h"
#include "ui_motorview.h"
#include <QTimer>
#include <QLocale>

MotorView::MotorView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MotorView)
    , mbMotor(nullptr)
    , motorNameLabel("Мотор")
    , positionValidator(this)
{
    ui->setupUi(this);

    ui->motorNumber->setText(motorNameLabel);

    positionValidator.setNotation(QDoubleValidator::StandardNotation);
    QLocale lo(QLocale::C);
    lo.setNumberOptions(QLocale::RejectGroupSeparator);
    positionValidator.setLocale(lo);
    ui->setPosition->setValidator(&positionValidator);


    //Acknoledgement
    connect(ui->acknoledge, SIGNAL(clicked()), this, SLOT(onAcknoledge()));
    //Jog Negative
    connect(ui->jogMinus, SIGNAL(pressed()), this, SLOT(onJogMinusPressed()));
    connect(ui->jogMinus, SIGNAL(released()), this, SLOT(onJogMinusReleased()));
    //Jog Positive
    connect(ui->jogPlus, SIGNAL(pressed()), this, SLOT(onJogPlusPressed()));
    connect(ui->jogPlus, SIGNAL(released()), this, SLOT(onJogPlusReleased()));
    //Point-to-point
    connect(ui->moveToPoint, SIGNAL(clicked(bool)), this, SLOT(onMoveToPoint()));
    //Homing
    connect(ui->homing, SIGNAL(clicked(bool)), this, SLOT(onHoming()));
    //Stop
    connect(ui->stop, SIGNAL(toggled(bool)), this, SLOT(onStop(bool)));
    //Set Position
    connect(ui->setPosition, SIGNAL(editingFinished()), this, SLOT(onSetPosition()));

}

MotorView::~MotorView()
{
    delete ui;
}

void MotorView::setMotorName(QString name)
{
    motorNameLabel = name;
    ui->motorNumber->setText(motorNameLabel);
}

void MotorView::setMbComItem(MbComMotor& mbMotor)
{
    this->mbMotor = &mbMotor;
    connect(this->mbMotor, SIGNAL(stateUpdated()), this, SLOT(updateStates()));
    emit this->mbMotor->stateUpdated();
}

void MotorView::onAcknoledge()
{
    if(mbMotor != nullptr){
        mbMotor->ack(true);
        QTimer::singleShot(500, this, &MotorView::resetAcknolegde);
    }
    qDebug() << "onAcknoledge::" << true;
}

void MotorView::resetAcknolegde(){
    if(mbMotor != nullptr){
        mbMotor->ack(false);
    }
    qDebug() << "onAcknoledge::" << false;
}

void MotorView::onJogMinusPressed()
{
    if(mbMotor != nullptr){
        mbMotor->jogBkw(true);
    }
    qDebug() << "onJogMinus::" << true;
}

void MotorView::onJogMinusReleased()
{
    if(mbMotor != nullptr){
        mbMotor->jogBkw(false);
    }
    qDebug() << "onJogMinus::" << false;
}

void MotorView::onJogPlusPressed()
{
    if(mbMotor != nullptr){
        mbMotor->jogFwd(true);
    }
    qDebug() << "onJogPlus::" << true;
}

void MotorView::onJogPlusReleased()
{
    if(mbMotor != nullptr){
        mbMotor->jogFwd(false);
    }
    qDebug() << "onJogPlus::" << false;
}

void MotorView::onMoveToPoint()
{
    if(mbMotor != nullptr){
        mbMotor->goPtP(true);
        QTimer::singleShot(500, this, &MotorView::resetMoveToPoint);
    }
    qDebug() << "onMoveToPoint::" << true;
}

void MotorView::resetMoveToPoint()
{
    if(mbMotor != nullptr){
        mbMotor->goPtP(false);
    }
    qDebug() << "onMoveToPoint::" << false;
}

void MotorView::onHoming()
{
    if(mbMotor != nullptr){
        mbMotor->goHome(true);
        QTimer::singleShot(500, this, &MotorView::resetHoming);
    }
    qDebug() << "onHoming::" << true;
}

void MotorView::resetHoming()
{
    if(mbMotor != nullptr){
        mbMotor->goHome(false);
    }
    qDebug() << "onHoming::" << false;
}

void MotorView::onStop(bool state)
{
    if (state){
        ui->stop->setText("Возобновить");
    }
    else{
        ui->stop->setText("Остановить");
    }

    if(mbMotor != nullptr){
        mbMotor->stop(state);
    }
    qDebug() << "onStop::" << state;
}

void MotorView::onSetPosition()
{

    double position = ui->setPosition->text().toDouble();
    if(mbMotor != nullptr){
        mbMotor->setPosition(position);
    }
    qDebug() << position << "\n";
}

void MotorView::updateStates()
{
    if(mbMotor != nullptr){
        //Activation
        if(mbMotor->isActivated()){
            ui->isActive->setCheckState(Qt::Checked);
        }
        else{
            ui->isActive->setCheckState(Qt::Unchecked);
        }

        //Error
        if(mbMotor->isAlarm()){
            ui->isError->setCheckState(Qt::Checked);
        }
        else{
            ui->isError->setCheckState(Qt::Unchecked);
        }

        //Stopped
        if(mbMotor->isStopped()){
            ui->isStoped->setCheckState(Qt::Checked);
        }
        else{
            ui->isStoped->setCheckState(Qt::Unchecked);
        }

        //Jog Negative
        if(mbMotor->isDoJogBkw()){
            ui->isJogMinus->setCheckState(Qt::Checked);
        }
        else{
            ui->isJogMinus->setCheckState(Qt::Unchecked);
        }

        //Jog Positive
        if(mbMotor->isDoJogFwd()){
            ui->isJogPlus->setCheckState(Qt::Checked);
        }
        else{
            ui->isJogPlus->setCheckState(Qt::Unchecked);
        }

        //Busy
        if(mbMotor->isBusy()){
            ui->isBusy->setCheckState(Qt::Checked);
        }
        else{
            ui->isBusy->setCheckState(Qt::Unchecked);
        }

        //Point-to-Point
        if(mbMotor->isDoPtP()){
            ui->isMoveToPoint->setCheckState(Qt::Checked);
        }
        else{
            ui->isMoveToPoint->setCheckState(Qt::Unchecked);
        }

        //Was Homed
        if(mbMotor->isHomed()){
            ui->isHomed->setCheckState(Qt::Checked);
        }
        else{
            ui->isHomed->setCheckState(Qt::Unchecked);
        }

        //Homing
        if(mbMotor->isDoHome()){
            ui->isHoming->setCheckState(Qt::Checked);
        }
        else{
            ui->isHoming->setCheckState(Qt::Unchecked);
        }

        //Position
        if(mbMotor->isHomed()){
            double position = mbMotor->getCurrentPosition();
            ui->currentPosition->setText(QString::number(position, 'f', 3));
        }
        else{
            ui->currentPosition->setText("###");
        }
    }
}
