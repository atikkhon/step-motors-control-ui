#ifndef MODBUSVIEW_H
#define MODBUSVIEW_H

#include <QWidget>
#include "modbuscommunicator.h"

namespace Ui {
class ModbusView;
}

class ModbusView : public QWidget
{
    Q_OBJECT

public:
    explicit ModbusView(QWidget *parent = nullptr);
    ~ModbusView();

    void setModbusCommunicator(ModbusCommunicator *mb);
private:
    Ui::ModbusView *ui;
    ModbusCommunicator *mb;

private slots:
    void onConnectionStateChanged(QModbusDevice::State state);


};

#endif // MODBUSVIEW_H
