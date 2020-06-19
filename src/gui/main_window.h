#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "src/core/navigational_equipment_manager.h"

#include <QMainWindow>
#include <QJsonObject>
#include <QLabel>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QLabel *labelUser;

    QLabel *labelConnectionARINC429;

    QTimer *timer;

    NavigationalEquipmentManager equipmentManager;

private:
    void setSignalsAndSlots();

    void initToolBar();

    void initStatusBar();

    void initTimer(int value);

private slots:
    void closeApplication();

    void printData(QJsonObject data);

    void printDefaultData();

    void onPushButtonSetOperationModeClicked();

    void changeFrequencyUpdate(int value);

    void onPushButtonInitARINC429Clicked();

    void onPushButtonInitChannelsIOClicked();

    void timerTick();

    void onPushButtonSaveReportClicked();

    void onPushButtonCalculateErrorRange();

    void onPushButtonCalculateErrorAzimuth();

    void onPushButtonCalculateErrorCourse();

    void onPushButtonCalculateErrorGlide();

    void updateProgressBarRangeError(int value);

    void updateProgressBarAzimuthError(int value);

    void updateProgressBarCourseError(int value);

    void updateProgressBarGlideError(int value);

    void printRangeAverageAndError(double average, double error);

    void printAzimuthAverageAndError(double average, double error);

    void printCourseAverageAndError(double average, double error);

    void printGlideAverageAndError(double average, double error);

signals:
    void equipmentTypeChangeRequested(QString type);

    void operationModeChangeRequested(QJsonObject mode);

    void frequencyUpdateChangeRequested(int value);

    void channelsIOChangeRequested(QJsonObject channels);

    void calculateErrorRequested(QString param, double trueValue, int samples);
};

#endif // MAIN_WINDOW_H
