#ifndef NAVIGATIONAL_EQUIPMENT_H
#define NAVIGATIONAL_EQUIPMENT_H

#include <QJsonObject>

/*
 * Класс организует взаимодействие Терминала управления с установленным
 * комплектом бортовой навигационно-посадочной аппаратуры (БНПА).
 *
 * Методы класса позволяют устаналвивать режим работы БНПА,
 * получать информацию о наклонной дальности, азимуте,
 * отклонениях по курсу и по глиссаде, а также о состоянии кналов приема.
 *
 * Класс является базовым, для конкретного типа БНПА необходимо создать
 * отлдельный класс и наследовать его от текущего, реализовав при этом
 * все вертуальные методы (интерфейсы).
 *
 * Информационный обмен организован при помощи json.
 *
 * Рекомендуема структура json-объекта для метода getParameters():
 *
 * key: StatusRange, value: bool
 * key: ValueRange,  value: double
 *
 * key: StatusAzimuth, value: bool
 * key: ValueAzimuth,  value: double
 *
 * и так далее.
 *
 * Рекомендуемая структура json-объекта для метода setOperationMode(QJsonObject mode):
 *
 * key: Режим, value: string
 * key: Тип РМ, value: string
 * key: Диапазон, value: string
 * key: ЧКК, value: int
 * key: Тест, value: bool
 */

class NavigationalEquipment
{
protected:
    double range;
    double azimuth;
    double course;
    double glide;

    bool statusRange;
    bool statusAzimuth;
    bool statusCourse;
    bool statusGlide;

    virtual void processingWord32(unsigned long word32) = 0;

public:
    NavigationalEquipment();

    virtual ~NavigationalEquipment();

    const double BAD_VALUE = -9999.9;

public:
    virtual void setOperationMode(QJsonObject mode) = 0;

    virtual QJsonObject getParameters() = 0;

    double getRange() const;
    double getAzimuth() const;
    double getCourse() const;
    double getGlide() const;

    bool getStatusRange() const;
    bool getStatusAzimuth() const;
    bool getStatusCourse() const;
    bool getStatusGlide() const;
};

#endif // NAVIGATIONAL_EQUIPMENT_H
