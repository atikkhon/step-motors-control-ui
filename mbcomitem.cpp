#include "mbcomitem.h"


//===============================================================
//=========== MbComPlaceholder ==================================
//===============================================================

MbComPlaceholder::MbComPlaceholder():numOfWriteRegs(0),numOfReadRegs(0){

}

MbComPlaceholder::MbComPlaceholder(quint16 regsToWrite, quint16 regsToRead):
    numOfWriteRegs(regsToWrite), numOfReadRegs(regsToRead){

}

MbComPlaceholder::~MbComPlaceholder(){

}

bool MbComPlaceholder::hasUpdatedData() const {
    return false;
}

quint16 MbComPlaceholder::getWriteDataSize() const {
    return numOfWriteRegs;
}

QVector<quint16> MbComPlaceholder::getData(){
    QVector<quint16> ret(numOfWriteRegs, 0);
    return ret;
}

quint16 MbComPlaceholder::getParseDataSize() const{
    return numOfReadRegs;
}

void MbComPlaceholder::parseData(const QVector<quint16> &data){
    if (data.size() != numOfReadRegs){
        throw std::invalid_argument("MbComPlaceholder: parse data wrong size!");
    }
}

//===============================================================
//=========== MbComMotor ========================================
//===============================================================

MbComMotor::MbComMotor(QObject* parent):
    QObject(parent),
    numOfWriteRegs(5),
    numOfReadRegs(5)
{
    writeRegs = QVector<quint16>(numOfWriteRegs, 0);
    readRegs = QVector<quint16>(numOfReadRegs, 0);
}

MbComMotor::~MbComMotor(){

}

bool MbComMotor::hasUpdatedData() const{
    return dataUpdated;
}

quint16 MbComMotor::getWriteDataSize() const{
    return numOfWriteRegs;
}

QVector<quint16> MbComMotor::getData() {
    dataUpdated = false;
    return writeRegs;
}

quint16 MbComMotor::getParseDataSize() const {
    return numOfReadRegs;
}

void MbComMotor::parseData(const QVector<quint16> &data){
    readRegs = data;
    emit stateUpdated();
}

void MbComMotor::setPosition(double position){
    qint32 positionInt = position * 1000;
    quint16 data[2];
    memcpy(data, &positionInt, 4);
    writeRegs[2] = data[0];
    writeRegs[1] = data[1];
    dataUpdated = true;

}

void MbComMotor::setDataBit(quint16& word, quint16 bitNum, bool state){
    if (state){
        word |= (0x01 << bitNum);
    }
    else{
        word &= ~(0x01 << bitNum);
    }
}

void MbComMotor::activate(bool state){
    setDataBit(writeRegs[0], 0, state);
    dataUpdated = true;
}

void MbComMotor::jogFwd(bool state){
    setDataBit(writeRegs[0], 1, state);
    dataUpdated = true;
}

void MbComMotor::jogBkw(bool state){
    setDataBit(writeRegs[0], 2, state);
    dataUpdated = true;
}


void MbComMotor::goHome(bool state){
    setDataBit(writeRegs[0], 3, state);
    dataUpdated = true;
}

void MbComMotor::goPtP(bool state){
    setDataBit(writeRegs[0], 4, state);
    dataUpdated = true;
}

void MbComMotor::stop(bool state){
    setDataBit(writeRegs[0], 5, state);
    dataUpdated = true;
}

void MbComMotor::ack(bool state){
    setDataBit(writeRegs[0], 6, state);
    dataUpdated = true;
}

double MbComMotor::getCurrentPosition() const{
    quint16 data[2];
    qint32 positionInt;
    data[0] = readRegs[2];
    data[1] = readRegs[1];
    memcpy(&positionInt, data, 4);

    return double(positionInt)/1000.0;
}

bool MbComMotor::readDataBit(const quint16& word, quint16 bitNum){
    if ((word & (0x01 << bitNum)) != 0){
        return true;
    }
    else{
        return false;
    }
}

bool MbComMotor::isActivated() const{
    return readDataBit(readRegs[0], 0);
}

bool MbComMotor::isBusy() const{
    return readDataBit(readRegs[0], 1);
}

bool MbComMotor::isDoJogFwd() const{
    return readDataBit(readRegs[0], 2);
}

bool MbComMotor::isDoJogBkw() const{
    return readDataBit(readRegs[0], 3);
}

bool MbComMotor::isDoHome() const{
    return readDataBit(readRegs[0], 4);
}

bool MbComMotor::isDoPtP() const{
    return readDataBit(readRegs[0], 5);
}

bool MbComMotor::isAlarm() const{
    return readDataBit(readRegs[0], 6);
}

bool MbComMotor::isHomed() const{
    return readDataBit(readRegs[0], 7);
}

bool MbComMotor::isStopped() const{
    return readDataBit(readRegs[0], 8);
}

//===============================================================
//=========== MbComSystem =======================================
//===============================================================

MbComSystem::MbComSystem(QObject* parent):
    QObject(parent)
    , numOfWriteRegs(1)
    , numOfReadRegs(1)
    , dataUpdated(false)
{
    readRegs = QVector<quint16>(numOfWriteRegs, 0);
    writeRegs = QVector<quint16>(numOfReadRegs, 0);
}

MbComSystem::~MbComSystem()
{

}

void MbComSystem::setDataBit(quint16& word, quint16 bitNum, bool state){
    if (state){
        word |= (0x01 << bitNum);
    }
    else{
        word &= ~(0x01 << bitNum);
    }
}

bool MbComSystem::readDataBit(const quint16& word, quint16 bitNum){
    if ((word & (0x01 << bitNum)) != 0){
        return true;
    }
    else{
        return false;
    }
}

bool MbComSystem::hasUpdatedData() const
{
    return dataUpdated;
}

quint16 MbComSystem::getWriteDataSize() const
{
    return numOfWriteRegs;
}

QVector<quint16> MbComSystem::getData()
{
    dataUpdated = false;
    return writeRegs;
}
quint16 MbComSystem::getParseDataSize() const
{
    return numOfReadRegs;
}

void MbComSystem::parseData(const QVector<quint16> &data)
{
    readRegs = data;
    emit stateUpdated();
}

void MbComSystem::setHeartBeatState(bool state)
{
    setDataBit(writeRegs[0], 0, state);
    dataUpdated = true;
}

void MbComSystem::setStop(bool state)
{
    setDataBit(writeRegs[0], 1, state);
    dataUpdated = true;
}


void MbComSystem::setActivateUp(bool state)
{
    setDataBit(writeRegs[0], 2, state);
    dataUpdated = true;
}

void MbComSystem::setActivateDown(bool state)
{
    setDataBit(writeRegs[0], 3, state);
    dataUpdated = true;
}

bool MbComSystem::isSystemReady()
{
    return readDataBit(readRegs[0], 0);
}

bool MbComSystem::isSystemMooving()
{
    return readDataBit(readRegs[0], 1);
}

bool MbComSystem::isSystemError()
{
    return readDataBit(readRegs[0], 2);
}

bool MbComSystem::isSystemEStop()
{
    return readDataBit(readRegs[0], 3);
}

//===============================================================
//=========== MbComSpeed ========================================
//===============================================================


MbComSpeed::MbComSpeed(QObject* parent):
    QObject(parent)
    , numOfWriteRegs(1)
    , numOfReadRegs(0)
    , dataUpdated(false)
{
    writeRegs = QVector<quint16>(numOfWriteRegs, 0);
    readRegs = QVector<quint16>(numOfReadRegs, 0);
}

MbComSpeed::~MbComSpeed()
{

}

bool MbComSpeed::hasUpdatedData() const
{
    return dataUpdated;
}

quint16 MbComSpeed::getWriteDataSize() const
{
    return numOfWriteRegs;
}

QVector<quint16> MbComSpeed::getData()
{
    dataUpdated = false;
    return writeRegs;
}

quint16 MbComSpeed::getParseDataSize() const
{
    return numOfReadRegs;
}

void MbComSpeed::parseData(const QVector<quint16> &data)
{
    if (numOfReadRegs == data.size()){
        for (int i = 0; i < data.size(); i++){
            readRegs[i] = data[i];
        }
    }
}

void MbComSpeed::setSpeed(double speed)
{
    if(speed >= 0 && speed  <= 5000){
        writeRegs[0] = int(speed);
        dataUpdated = true;
    }
    else{
        throw std::range_error("MbComSpeed::setSpeed: Speed set point out of range!");
    }
}

