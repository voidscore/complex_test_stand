#ifndef EQUIPMENT_A_331_H
#define EQUIPMENT_A_331_H

#include "src/core/navigational_equipment.h"

class EquipmentA_331: public NavigationalEquipment
{
public:
    EquipmentA_331();

    ~EquipmentA_331();

    // NavigationalEquipment interface
public:
    void setOperationMode(QJsonObject mode);

    QJsonObject getParameters();
};

#endif // EQUIPMENT_A_331_H
