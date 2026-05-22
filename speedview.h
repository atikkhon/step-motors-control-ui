#ifndef SPEEDVIEW_H
#define SPEEDVIEW_H

#include <QWidget>
#include <QIntValidator>
#include "mbcomitem.h"

namespace Ui {
class SpeedView;
}

class SpeedView : public QWidget
{
    Q_OBJECT

public:
    explicit SpeedView(QWidget *parent = nullptr);
    ~SpeedView();

    void setMbComSpeed(MbComSpeed *spd);

private:
    Ui::SpeedView *ui;
    MbComSpeed *speed;
    QIntValidator speedValidator;
private slots:
    void onSetSpeed();
};

#endif // SPEEDVIEW_H
