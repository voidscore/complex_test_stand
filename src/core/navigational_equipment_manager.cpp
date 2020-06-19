#include "navigational_equipment_manager.h"

NavigationalEquipmentManager::NavigationalEquipmentManager(
        QObject *parent) : QObject(parent)
{
    // По умолчанию время обновления таймера
    // составляет 400 мс
    timeUpdate = 400;

    initTimer(timeUpdate);

    equipment = nullptr;
}

void NavigationalEquipmentManager::equipmentTypeChange(QString type)
{
    qDebug() << "Equipment type change";

    if (equipment != nullptr)
    {
        delete equipment;
        equipment = nullptr;
    }

    if (type == "РСБН-НП")
    {
        qDebug() << "РСБН-НП";
        equipment = new EquipmentRSBN_NP();
        return;
    }
}

void NavigationalEquipmentManager::operationModeChange(QJsonObject mode)
{
    if (equipment != nullptr)
    {
        equipment->setOperationMode(mode);
    }
}

void NavigationalEquipmentManager::frequencyUpdateChange(int value)
{
    qDebug() << "Frequency update change";

    // Устанавливаем новое значение времени
    // срабатывания таймера
    timeUpdate = value;

    // Перезапускаем таймер
    timer->stop();

    timer->start(timeUpdate);
}

void NavigationalEquipmentManager::channelsIOChange(QJsonObject channels)
{
    qDebug() << "Channels IO change";
}

void NavigationalEquipmentManager::calculateError(QString param, double trueValue, int samples)
{
    qDebug() << "Calculate error";
}

void NavigationalEquipmentManager::initTimer(int value)
{
    timer = new QTimer(this);

    timer->start(value);

    connect(timer, &QTimer::timeout, this,
            &NavigationalEquipmentManager::timerTick);
}

void NavigationalEquipmentManager::timerTick()
{
    if (equipment != nullptr)
    {
        QJsonObject data = equipment->getParameters();

        emit parametersIsReady(data);
    }
}
