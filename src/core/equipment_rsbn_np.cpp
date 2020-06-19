#include "equipment_rsbn_np.h"

#include "src/tools/interface_ARINC429.h"

#include <QDebug>

EquipmentRSBN_NP::EquipmentRSBN_NP()
{
    InterfaceARINC429& instance = InterfaceARINC429::getInstance();

    instance.startAddressReception(nInputChannelControl);

    instance.startAddressReception(nInputChannelAPD);
}

EquipmentRSBN_NP::~EquipmentRSBN_NP()
{

}

void EquipmentRSBN_NP::setOperationMode(QJsonObject mode)
{
    ulong controlWord[1];

    controlWord[0] = 0x800000FE;

    if (mode.take("Режим") == "Посадка")
        controlWord[0] |= 0x8000800;

    if (mode.take("Тип РМ") == "Направленный")
        controlWord[0] |= 0x4000000;

    if (mode.take("Диапазон") == "МД")
        controlWord[0] |= 0x2000000;

    if (mode.take("Тест") == true)
        controlWord[0] |= 0x40000000;

    int numberFCC = mode.take("ЧКК").toInt();
    int units = 0;
    int tens = 0;
    int hundreds = 0;

    units = numberFCC % 10;
    tens = (numberFCC % 100) / 10;
    hundreds = numberFCC / 100;

    controlWord[0] |= static_cast<unsigned long>(((hundreds << 22) | (tens << 18) | (units << 14)));

    InterfaceARINC429& instance = InterfaceARINC429::getInstance();

    instance.pushToChannel(nOutputChannelControl, controlWord, 1);
}

QJsonObject EquipmentRSBN_NP::getParameters()
{
    InterfaceARINC429 &instance = InterfaceARINC429::getInstance();

    instance.popFromChannel(nInputChannelControl, inputArray, sizeInputArray);

    instance.popFromChannel(nInputChannelAPD, inputArrayAPD, sizeInputArray);

    for (int i = 0; i < countInputAddresses; ++i)
    {
        ulong word32 = (inputArray[inputAddresses[i] - 1] >> 16) |
            (inputArray[inputAddresses[i]] << 16);

        processingWord32(word32);
    }

    QJsonObject result;

    for (int i = 0; i < 2; ++i)
    {
        ulong word32 = (inputArrayAPD[inputAddressesAPD[i] - 1] >> 16) |
            (inputArrayAPD[inputAddressesAPD[i]] << 16);

        unsigned char address = word32 & 0xFF;

        switch (address)
        {
        case 0xA7:

            if ((word32 & 0x1C000000) == 0x8000000)
            {
                result.insert("Статус ПРМГ-ОМД", "2 категория");
            }
            else if ((word32 & 0x1C000000) == 0x4000000)
            {
                result.insert("Статус ПРМГ-ОМД", "1 категория");
            }
            else if ((word32 & 0x1C000000) == 0x0000000)
            {
                result.insert("Статус ПРМГ-ОМД", "Отказ");
            }
            else if ((word32 & 0x1C000000) == 0xC000000)
            {
                result.insert("Статус ПРМГ-ОМД", "Регламент");
            }
            else if ((word32 & 0x1C000000) == 0x10000000)
            {
                result.insert("Статус ПРМГ-ОМД", "Тест");
            }

            break;

        case 0xA6:
            int number = word32 & 0x1FFC0000;

            number = number >> 19;

            result.insert("Опознавание ПРМГ-ОМД", number);
            break;
        }
    }

    if (statusRange)
        result.insert("Дальность", range);

    if (statusAzimuth)
        result.insert("Азимут", azimuth);

    if (statusCourse)
        result.insert("Курс", course);

    if (statusGlide)
        result.insert("Глиссада", glide);

    return result;
}

void EquipmentRSBN_NP::processingWord32(unsigned long word32)
{
    unsigned char address = word32 & 0xFF;
    word32 = word32 >> 8;

    switch (address)
    {
    case 0x9C:
        if ((word32 & 0x600000) == 0x600000 || (word32 & 0x400000) == 0x400000)
        {
            statusRange = true;
            range = (((word32 & 0x1FFFC0)) * coeffRange);
        }
        else
        {
            statusRange = false;
            range = BAD_VALUE;
        }
        break;

    case 0x9D:
        if ((word32 & 0x600000) == 0x600000 || (word32 & 0x400000) == 0x400000)
        {
            statusAzimuth = true;
            azimuth = (((word32 & 0x1FFFC0)) * coeffAzimuth);
        }
        else
        {
            statusAzimuth = false;
            azimuth = BAD_VALUE;
        }
        break;

    case 0x7B:
        if ((word32 & 0x600000) == 0x600000 || (word32 & 0x400000) == 0x400000)
        {
            statusCourse = true;

            if ((word32 & 0x100000) == 0x100000)
            {
                word32 = ~(word32 & 0x1FFF00);
                course = ((-1.0) * (word32 & 0x1FFF00) * coeffCourse);
            }
            else
            {
                course = ((word32 & 0x1FFF00) * coeffCourse);
            }
        }
        else
        {
            statusCourse = false;
            course = BAD_VALUE;
        }
        break;

    case 0x7C:
        if ((word32 & 0x600000) == 0x600000 || (word32 & 0x400000) == 0x400000)
        {
            statusGlide = true;

            if ((word32 & 0x100000) == 0x100000)
            {
                word32 = ~(word32 & 0x1FFF00);
                glide = ((-1.0) * (word32 & 0x1FFF00) * coeffGlide);
            }
            else
            {
                glide = ((word32 & 0x1FFF00) * coeffGlide);
            }
        }
        else
        {
            statusGlide = false;
            glide = BAD_VALUE;
        }
        break;
    }
}
