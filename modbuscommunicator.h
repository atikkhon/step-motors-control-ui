#ifndef MODBUSCOMMUNICATOR_H
#define MODBUSCOMMUNICATOR_H
#include <QVector>
#include "mbcomitem.h"
#include <QModbusDataUnit>
#include <QModbusTcpClient>
#include <QTimer>
#include <QModbusReply>

class ModbusCommunicator: public QObject
{
    Q_OBJECT
private:
    int readOffset;
    QVector<MbComItem*> devsRead;
    int regsReadCount;
    int writeOffset;
    QVector<MbComItem*> devsWrite;
    QString addr;
    int port;
    QModbusTcpClient *client;
    QTimer *pollingRead;

public:
    ModbusCommunicator(QObject *parent = nullptr);
    ModbusCommunicator(const ModbusCommunicator& other) = delete;
    ~ModbusCommunicator();
    ModbusCommunicator(int readOffset, int writeOffset, QString addr, int port, QObject *parent = nullptr);
    ModbusCommunicator& operator=(const ModbusCommunicator& other) = delete;

    void pushDevWrite(MbComItem* dev);
    void pushDevRead(MbComItem* dev);

    bool connectPLC();
    void disconnectPLC();

private:
    bool hasWriteRequest();
    QModbusDataUnit getWriteRequest();
    void parseReadRequest(const QModbusDataUnit& data);
    QModbusDataUnit getReadRequest();

private slots:
    void sendReadRequest();
    void onReadFinished();
    void onWriteFinished();
    void onConnectionStateChanged(QModbusDevice::State state);

public slots:
    void onConnect(bool connect);

signals:
    void connectionStateChanged(QModbusDevice::State state);
};

#endif // MODBUSCOMMUNICATOR_H
