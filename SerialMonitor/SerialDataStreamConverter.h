#ifndef SERIALDATASTREAMCONVERTER_H
#define SERIALDATASTREAMCONVERTER_H

#include <map>
#include <string>

class SerialDataConverter
{
public:
    explicit SerialDataConverter();
    ~SerialDataConverter();

    enum DataID {
        ACCX,
        ACCY,
        ACCZ,
        GyroX,
        GyroY,
        GyroZ,
    };

    void registerCallback();

    std::string serialStringFromDataList(const std::map<DataID,int> &dataList);
    std::map<DataID,int> dataListFromSerialString(const std::string &serialString);
};

#endif // SERIALDATASTREAMCONVERTER_H
