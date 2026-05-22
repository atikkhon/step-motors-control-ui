#ifndef SYSTEMVIEW_H
#define SYSTEMVIEW_H

#include <QWidget>
#include "mbcomitem.h"
#include <QTimer>

namespace Ui {
class SystemView;
}

class SystemView : public QWidget
{
    Q_OBJECT

public:
    explicit SystemView(QWidget *parent = nullptr);
    ~SystemView();

    void setMbComItem(MbComSystem *sys);

private:
    Ui::SystemView *ui;
    MbComSystem *sys;
    bool HeartBeatState;
    QTimer *heartBeatTimer;

private slots:
    void onSystemStop(bool state);
    void onHeartBeatFire();
    void onActivateUp(bool state);
    void onActivateDown(bool state);

public slots:
    void updateStates();
};

#endif // SYSTEMVIEW_H
