#ifndef NAVIGATIONAL_EQUIPMENT_MANAGER_H
#define NAVIGATIONAL_EQUIPMENT_MANAGER_H

#include "src/core/equipment_rsbn_np.h"
#include "src/core/equipment_a_331.h"
#include "src/core/equipment_simulator.h"

#include <QJsonObject>
#include <QObject>
#include <QTimer>
#include <QDebug>

class NavigationalEquipmentManager : public QObject
{
    Q_OBJECT
public:
    explicit NavigationalEquipmentManager(QObject *parent = nullptr);

signals:

public slots:
    void equipmentTypeChange(QString type);

    void operationModeChange(QJsonObject mode);

    void frequencyUpdateChange(int value);

    void channelsIOChange(QJsonObject channels);

    void calculateError(QString param, double trueValue, int samples);

private:
    QTimer *timer;

    int timeUpdate;

    NavigationalEquipment *equipment;

private:
    void initTimer(int value);

private slots:
    void timerTick();

signals:
    void parametersIsReady(QJsonObject parameters);
};

#endif // NAVIGATIONAL_EQUIPMENT_MANAGER_H
