#ifndef EQUIPMENT_SIMULATOR_H
#define EQUIPMENT_SIMULATOR_H

#include "src/core/navigational_equipment.h"

class EquipmentSimulator: public NavigationalEquipment
{
public:
    EquipmentSimulator();

    ~EquipmentSimulator();

    // NavigationalEquipment interface
public:
    void setOperationMode(QJsonObject mode);

    QJsonObject getParameters();

    // NavigationalEquipment interface
protected:
    void processingWord32(unsigned long word32);
};

#endif // EQUIPMENT_SIMULATOR_H
