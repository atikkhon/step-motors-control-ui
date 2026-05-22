#include "modbusview.h"
#include "ui_modbusview.h"

ModbusView::ModbusView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModbusView)
    , mb(nullptr)
{
    ui->setupUi(this);
}

ModbusView::~ModbusView()
{
    delete ui;
}

void ModbusView::setModbusCommunicator(ModbusCommunicator *mb)
{
    if(this->mb != nullptr){
        disconnect(ui->connect, SIGNAL(clicked(bool)), this->mb, SLOT(onConnect(bool)));
        disconnect(this->mb, SIGNAL(connectionStateChanged(QModbusDevice::State)), this, SLOT(onConnectionStateChanged(QModbusDevice::State)));
    }
    this->mb = mb;
    connect(ui->connect, SIGNAL(clicked(bool)), this->mb, SLOT(onConnect(bool)));
    connect(this->mb, SIGNAL(connectionStateChanged(QModbusDevice::State)), this, SLOT(onConnectionStateChanged(QModbusDevice::State)));
}

void ModbusView::onConnectionStateChanged(QModbusDevice::State state)
{
    if(state == QModbusDevice::ClosingState || state == QModbusDevice::ConnectingState){
        ui->isConnected->setCheckState(Qt::PartiallyChecked);
    }
    else if(state == QModbusDevice::ConnectedState){
        ui->isConnected->setCheckState(Qt::Checked);
        ui->connect->setChecked(true);
        ui->connect->setText("Отключить");
    }
    else{
        ui->isConnected->setCheckState(Qt::Unchecked);
        ui->connect->setChecked(false);
        ui->connect->setText("Подключить");
    }
}
