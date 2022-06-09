#ifndef SERIALPORTWRAPPER_H
#define SERIALPORTWRAPPER_H

#include <QObject>
#include <QVariantList>
#include <QVariantMap>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class SerialPortWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortWrapper(QObject *parent = nullptr);

    Q_PROPERTY(QVariantList records MEMBER m_records NOTIFY recordsChanged)
signals:
    void recordsChanged();

private slots:
    void onSerialPortReadReady();

private:
    void _serialDataProcess(QStringList keyValue);
private:
    QSerialPort m_serialPort;

    QString m_commStrBuffer;
    const QString m_dataSplitterChar = ",";
    const QString m_keyValueSplitterChar = ":";
    QVariantList m_records;
    QVariantMap m_oneRecord;
};

#endif // SERIALPORTWRAPPER_H
