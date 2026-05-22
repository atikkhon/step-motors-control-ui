#ifndef MBCOMITEM_H
#define MBCOMITEM_H
#include <QVector>
#include <QObject>


//=================================================
//============= MbComItem (abstract) ==============
//=================================================

class MbComItem
{
public:
    virtual ~MbComItem() = default;
    virtual bool hasUpdatedData() const = 0;
    virtual quint16 getWriteDataSize() const = 0;
    virtual QVector<quint16> getData() = 0;
    virtual quint16 getParseDataSize() const = 0;
    virtual void parseData(const QVector<quint16>&) = 0;
};


//=================================================
//================ MbComPlaceholder================
//=================================================


class MbComPlaceholder : public MbComItem{
private:
    quint16 numOfWriteRegs;
    quint16 numOfReadRegs;
public:
    MbComPlaceholder();
    MbComPlaceholder(quint16 regsToWrite, quint16 regsToRead);
    MbComPlaceholder(const MbComPlaceholder& copy) = delete;
    virtual ~MbComPlaceholder() override;
    MbComPlaceholder& operator=(const MbComPlaceholder& copy) = delete;

    virtual bool hasUpdatedData() const override;
    virtual quint16 getWriteDataSize() const override;
    virtual QVector<quint16> getData() override;
    virtual quint16 getParseDataSize() const override;
    virtual void parseData(const QVector<quint16> &data) override;

};


//=================================================
//================ MbComMotor======================
//=================================================


class MbComMotor: public QObject, public MbComItem
{
    Q_OBJECT

private:
    quint16 numOfWriteRegs;
    quint16 numOfReadRegs;
    QVector<quint16> writeRegs;
    QVector<quint16> readRegs;
    bool dataUpdated;

    static void setDataBit(quint16& word, quint16 bitNum, bool state);
    static bool readDataBit(const quint16& word, quint16 bitNum);

public:
    MbComMotor(QObject* parent = nullptr);
    MbComMotor(const MbComMotor& copy) = delete;
    virtual ~MbComMotor() override;
    MbComMotor& operator=(const MbComMotor& copy) = delete;

    virtual bool hasUpdatedData() const override;
    virtual quint16 getWriteDataSize() const override;
    virtual QVector<quint16> getData() override;
    virtual quint16 getParseDataSize() const override;
    virtual void parseData(const QVector<quint16> &data) override;


    void setPosition(double position);
    void activate(bool state);
    void jogFwd(bool state);
    void jogBkw(bool state);
    void goHome(bool state);
    void goPtP(bool state);
    void stop(bool state);
    void ack(bool state);


    double getCurrentPosition() const;
    bool isActivated() const;
    bool isBusy() const;
    bool isDoJogFwd() const;
    bool isDoJogBkw() const;
    bool isAlarm() const;
    bool isDoHome() const;
    bool isDoPtP() const;
    bool isHomed() const;
    bool isStopped() const;

signals:
    void stateUpdated();
};


//=================================================
//================ MbComSystem=====================
//=================================================


class MbComSystem: public QObject, public MbComItem
{
    Q_OBJECT
private:
    quint16 numOfWriteRegs;
    quint16 numOfReadRegs;
    QVector<quint16> writeRegs;
    QVector<quint16> readRegs;
    bool dataUpdated;

    static void setDataBit(quint16& word, quint16 bitNum, bool state);
    static bool readDataBit(const quint16& word, quint16 bitNum);

public:
    MbComSystem(QObject* parent = nullptr);
    MbComSystem(const MbComSystem& copy) = delete;
    virtual ~MbComSystem() override;
    MbComSystem& operator=(const MbComSystem& copy) = delete;

    virtual bool hasUpdatedData() const override;
    virtual quint16 getWriteDataSize() const override;
    virtual QVector<quint16> getData() override;
    virtual quint16 getParseDataSize() const override;
    virtual void parseData(const QVector<quint16> &data) override;

    void setHeartBeatState(bool state);
    void setStop(bool state);
    void setActivateUp(bool state);
    void setActivateDown(bool state);

    bool isSystemReady();
    bool isSystemMooving();
    bool isSystemError();
    bool isSystemEStop();

signals:
    void stateUpdated();
};


//=================================================
//================ MbComSpeed======================
//=================================================


class MbComSpeed: public QObject, public MbComItem
{
    Q_OBJECT
private:
    quint16 numOfWriteRegs;
    quint16 numOfReadRegs;
    QVector<quint16> writeRegs;
    QVector<quint16> readRegs;
    bool dataUpdated;

public:
    MbComSpeed(QObject* parent = nullptr);
    MbComSpeed(const MbComSpeed& copy) = delete;
    virtual ~MbComSpeed() override;
    MbComSpeed& operator=(const MbComSpeed& copy) = delete;

    virtual bool hasUpdatedData() const override;
    virtual quint16 getWriteDataSize() const override;
    virtual QVector<quint16> getData() override;
    virtual quint16 getParseDataSize() const override;
    virtual void parseData(const QVector<quint16> &data) override;

    void setSpeed(double speed);

signals:
    void stateUpdated();
};

#endif // MBCOMITEM_H
