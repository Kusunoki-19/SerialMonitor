#include "SerialPortWrapper.h"

#include <QDebug>
SerialPortWrapper::SerialPortWrapper(QObject *parent)
    : QObject{parent}
{

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) { //availablePorts()で利用可能なすべてのシリアルポートが取得できる
        qDebug() << "----------------------------";
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "----------------------------";

    }
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) { //availablePorts()で利用可能なすべてのシリアルポートが取得できる


//        if (info.portName().contains("tty.usbmodem1101", Qt::CaseInsensitive)) {
        if (info.portName().contains("cu.usbmodem1101", Qt::CaseInsensitive)) {
            qDebug() << "target port:" << info.description();
            qDebug() << "target port:" << info.portName();
            qDebug() << "target port:" << info.manufacturer();
            m_serialPort.setPort(info);
            m_serialPort.setBaudRate(QSerialPort::Baud115200);
            //            connect(&m_serialPort, &QSerialPort::readyRead, this,[=]() {
            //                qDebug() << "text:" <<
            //            });

            connect(&m_serialPort, &QSerialPort::readyRead, this,&SerialPortWrapper::onSerialPortReadReady);
            // Example use QSerialPort
            if (m_serialPort.open(QIODevice::ReadWrite)) {
                qDebug() << "successeded to open";
            }
            else {
                qDebug() << "failed to open";
                qDebug() << m_serialPort.error();
                qDebug() << m_serialPort.errorString();
            }
            break;
        }
    }
}

void SerialPortWrapper::onSerialPortReadReady()
{
    //qDebug() << __func__;
    // バッファに積む.
    m_commStrBuffer.push_back(QString(m_serialPort.readAll()));

    // ","で分割、最後の要素はまだ、区切りを認識できていない文字列なので次の文字列をうけとるまで残す
    //qDebug() << "m_commStrBuffer:" << m_commStrBuffer;
    QStringList records = m_commStrBuffer.split(m_dataSplitterChar);
    m_commStrBuffer = records.last();
    //qDebug() << "m_commStrBuffer trimmed:" << m_commStrBuffer;
    records.removeLast();
    //qDebug() << "records:" << records;

    for (int i = 0; i < records.length() ; i++) {
        // data分割charを含んでいなければ壊れたデータとしてスルー.
        const QString & record = records[i];
        if (!record.contains(m_keyValueSplitterChar)) {
            //qDebug() << "analyzeError(no key value splitter char):" << record;
        }
        QStringList keyValue = record.split(m_keyValueSplitterChar);
        if (keyValue.length() != 2) {
            //qDebug() << "analyzeError(invalid element length):" << keyValue;
        }
        //qDebug() << "keyValue:"<<keyValue;
        _serialDataProcess(keyValue);
    }
}

void SerialPortWrapper::_serialDataProcess(QStringList keyValue)
{

    if (keyValue[0] == "dataStart") {
        //qDebug() << "preRecord:" << m_oneRecord;
        m_records.push_back(m_oneRecord);
        m_oneRecord = QVariantMap();
        if (m_records.length() > 500) {
            m_records.removeFirst();
        }
        //qDebug() << "records:" << m_records;
        emit recordsChanged();
    }
    if (keyValue[0] == "diffMs") {
        const int currentDiffMs = keyValue[1].toInt(nullptr,10);
        //qDebug() << "diffMs:" <<currentDiffMs;
        m_oneRecord.insert("diffMs",currentDiffMs);
    }
    if (keyValue[0] == "gx") {
        const double gx =  keyValue[1].toDouble();
        //qDebug() << "gx:" <<gx;
        m_oneRecord.insert("gx",gx);
    }
    if (keyValue[0] == "gy") {
        const double gy =  keyValue[1].toDouble();
        //qDebug() << "gy:" <<gy;
        m_oneRecord.insert("gy",gy);
    }
    if (keyValue[0] == "gz") {
        const double gz =  keyValue[1].toDouble();
        //qDebug() << "gz:" <<gz;
        m_oneRecord.insert("gz",gz);
    }
}
