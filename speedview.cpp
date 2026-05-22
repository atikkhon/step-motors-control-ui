#include "speedview.h"
#include "ui_speedview.h"

SpeedView::SpeedView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpeedView)
    , speed(nullptr)
    , speedValidator(this)
{
    ui->setupUi(this);

    speedValidator.setRange(0, 5000);
    ui->speed->setValidator(&speedValidator);

    connect(ui->speed, SIGNAL(editingFinished()), this, SLOT(onSetSpeed()));
}

SpeedView::~SpeedView()
{
    delete ui;
}

void SpeedView::setMbComSpeed(MbComSpeed *spd)
{
    this->speed = spd;
}

void SpeedView::onSetSpeed()
{
    if(speed != nullptr){
        speed->setSpeed(ui->speed->text().toInt());
    }
}
