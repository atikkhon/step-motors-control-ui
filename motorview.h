#ifndef MOTORVIEW_H
#define MOTORVIEW_H

#include <QWidget>
#include <QString>
#include <QDoubleValidator>
#include "mbcomitem.h"

namespace Ui {
class MotorView;
}

class MotorView : public QWidget
{
    Q_OBJECT

public:
    explicit MotorView(QWidget *parent = nullptr);
    ~MotorView();

    void setMotorName(QString name);
    void setMbComItem(MbComMotor& mbMotor);


private slots:
    void onAcknoledge();
    void onJogMinusPressed();
    void onJogMinusReleased();
    void onJogPlusPressed();
    void onJogPlusReleased();
    void onMoveToPoint();
    void onHoming();
    void onStop(bool state);
    void onSetPosition();

private:
    Ui::MotorView *ui;
    MbComMotor *mbMotor;
    QString motorNameLabel;
    QDoubleValidator positionValidator;


    void resetAcknolegde();
    void resetMoveToPoint();
    void resetHoming();

public slots:
    void updateStates();
};

#endif // MOTORVIEW_H
