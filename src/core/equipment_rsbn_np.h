#ifndef EQUIPMENT_RSBN_NP_H
#define EQUIPMENT_RSBN_NP_H

#include "src/core/navigational_equipment.h"

class EquipmentRSBN_NP: public NavigationalEquipment
{
private:
    double coeffRange   = 500.0 / 1024.0 / 1024.0;
    double coeffAzimuth = 180.0 / 1024.0 / 1024.0;
    double coeffCourse  = 42.53 / 1024.0 / 512.0;
    double coeffGlide   = 75.38 / 1024.0 / 512.0;

    int countInputAddresses  = 6;
    int countOutputAddresses = 8;

    int nInputChannelControl = 1;
    int nOutputChannelControl = 2;
    int nInputChannelAPD = 4;

    unsigned int controlWord = 0x8000000;

    ushort inputAddresses[6] = {
        0x9C, 0x9D,
        0x7B, 0x7C,
        0xF9, 0xFA,
    };

    ushort outputAddresses[8] = {
        0x7F, 0x84,
        0x86, 0xD0,
        0xD5, 0x60,
        0x73, 0x74
    };

    ushort inputAddressesAPD[2] = {
        0xA6, 0xA7
    };

    int sizeInputArray = 256;

    unsigned long inputArray[256];

    unsigned long inputArrayAPD[256];

public:
    EquipmentRSBN_NP();

    ~EquipmentRSBN_NP();

    // NavigationalEquipment interface
public:
    void setOperationMode(QJsonObject mode);

    QJsonObject getParameters();

    // NavigationalEquipment interface
protected:
    void processingWord32(unsigned long word32);
};

#endif // EQUIPMENT_RSBN_NP_H
