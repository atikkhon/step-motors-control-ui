#include "modbuscommunicator.h"
#include <QVariant>

ModbusCommunicator::ModbusCommunicator(QObject *parent):
    QObject(parent)
    ,  readOffset(0)
    , devsRead(0)
    , regsReadCount(-1)
    , writeOffset(0)
    , devsWrite(0)
    , addr("127.0.0.1")
    , port(502)
    , client(nullptr)

{
    pollingRead = new QTimer(parent);
    pollingRead->setInterval(500);
    pollingRead->setSingleShot(false);
    connect(pollingRead, SIGNAL(finished()), this, SLOT(sendReadRequest));
}

ModbusCommunicator::~ModbusCommunicator()
{
    delete client;
    delete pollingRead;
}

ModbusCommunicator::ModbusCommunicator(int readOffset, int writeOffset, QString addr, int port, QObject *parent):
    QObject(parent)
    , readOffset(readOffset)
    , devsRead()
    , regsReadCount(-1)
    , writeOffset(writeOffset)
    , devsWrite()
    , addr(addr)
    , port(port)
    , client(nullptr)
{
    pollingRead = new QTimer(parent);
    pollingRead->setInterval(500);
    pollingRead->setSingleShot(false);
    connect(pollingRead, SIGNAL(timeout()), this, SLOT(sendReadRequest()));
}

void ModbusCommunicator::pushDevWrite(MbComItem* dev)
{
    devsWrite.push_back(dev);
}

void ModbusCommunicator::pushDevRead(MbComItem* dev)
{
    devsRead.push_back(dev);
    regsReadCount = -1;
}

bool ModbusCommunicator::hasWriteRequest()
{
    for(const auto &item: devsWrite){
        if(item->hasUpdatedData()){
            return true;
        }
    }
    return false;
}

QModbusDataUnit ModbusCommunicator::getWriteRequest()
{
    int startAddress = -1;
    int regsCounter = 0;
    quint16 size = 0;
    QVector<MbComItem*>::iterator startIter = devsWrite.begin();
    QVector<MbComItem*>::iterator stopIter = devsWrite.end();


    for(auto iter = devsWrite.begin(); iter != devsWrite.end(); iter++){
        if((*iter) != nullptr){
            if((*iter)->hasUpdatedData() && startAddress == -1){
                startAddress = regsCounter;
                startIter = iter;
            }

            regsCounter += (*iter)->getWriteDataSize();

            if((*iter)->hasUpdatedData() && startAddress != -1){
                size = regsCounter - startAddress;
                stopIter = iter;
            }
        }
    }
    if (startAddress != -1){
        regsCounter = 0;
        QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters, writeOffset + startAddress, size);
        for(auto iter = startIter; iter <= stopIter || iter != devsWrite.end(); iter++){
            if((*iter) != nullptr){
                for(const quint16 &reg: (*iter)->getData()){
                    dataUnit.setValue(regsCounter, reg);
                    regsCounter++;
                }
            }
        }
        return dataUnit;
    }
    else{
        return QModbusDataUnit();
    }
}

void ModbusCommunicator::parseReadRequest(const QModbusDataUnit& data)
{
    qsizetype regReqCount = data.valueCount();
    qsizetype regNumber = 0;

    for(auto dev : devsRead){
        QVector<quint16> vec(dev->getParseDataSize());
        for(quint16 i = 0; i < dev->getParseDataSize(); i++){
            vec[i] = data.value(regNumber);
            regNumber++;
            if(regNumber > regReqCount){
                throw std::out_of_range("ModbusCommunicator::parseReadRequest");
            }
        }
        dev->parseData(vec);
    }
}

bool ModbusCommunicator::connectPLC()
{
    if(client == nullptr){
        client = new QModbusTcpClient(this);
        connect(client, SIGNAL(stateChanged(QModbusDevice::State)), this, SLOT(onConnectionStateChanged(QModbusDevice::State)));
    }

    client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, addr);
    client->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);

    bool isConnected = client->connectDevice();

    return isConnected;
}

void ModbusCommunicator::disconnectPLC()
{
    if(client != nullptr){
        client->disconnectDevice();
    }
}

QModbusDataUnit ModbusCommunicator::getReadRequest()
{
    if(regsReadCount < 0){
        regsReadCount = 0;
        for(auto dev: devsRead){
            if(dev != nullptr){
                regsReadCount += dev->getParseDataSize();
            }
        }
    }

    QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters, readOffset, regsReadCount);

    return dataUnit;
}

void ModbusCommunicator::sendReadRequest()
{
    if(client->state() == QModbusDevice::ConnectedState){
        QModbusReply* reply = client->sendReadRequest(getReadRequest(), 1);
        connect(reply, SIGNAL(finished()), this, SLOT(onReadFinished()));
    }

}

void ModbusCommunicator::onReadFinished()
{
    QModbusReply *reply = qobject_cast<QModbusReply*>(sender());
    if (reply->error() == QModbusDevice::NoError){
        QModbusDataUnit dataUnit = reply->result();
        parseReadRequest(dataUnit);
        reply->deleteLater();
    }
    if(hasWriteRequest() && client->state() == QModbusDevice::ConnectedState){
        QModbusDataUnit dataUnit = getWriteRequest();
        QModbusReply *writeReply = client->sendWriteRequest(dataUnit, 1);
        connect(writeReply, SIGNAL(finished()), this, SLOT(onWriteFinished()));
    }

}

void ModbusCommunicator::onWriteFinished(){
    QModbusReply *reply = qobject_cast<QModbusReply*>(sender());
    reply->deleteLater();
}

void ModbusCommunicator::onConnectionStateChanged(QModbusDevice::State state)
{
    if(state == QModbusDevice::ConnectedState){
        pollingRead->start();
        qDebug() << "Started";
    }
    else{
        pollingRead->stop();
        qDebug() << "Stoped";
    }
    emit this->connectionStateChanged(state);
}

void ModbusCommunicator::onConnect(bool connect)
{
    if(connect){
        this->connectPLC();
    }
    else{
        this->disconnectPLC();
    }
}
