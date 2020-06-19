#include "main_window.h"
#include "ui_main_window.h"

#include "src/tools/interface_ARINC429.h"

#include <QAction>
#include <QFile>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initToolBar();

    initStatusBar();

    initTimer(100);

    setSignalsAndSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printData(QJsonObject data)
{
    QJsonValue range = data.take("Дальность");
    QJsonValue azimuth = data.take("Азимут");
    QJsonValue course = data.take("Курс");
    QJsonValue glide = data.take("Глиссада");
    QJsonValue statusPRMG_OMD = data.take("Статус ПРМГ-ОМД");
    QJsonValue recognitionPRMG_OMD = data.take("Опознавание ПРМГ-ОМД");

    if (range == QJsonValue::Undefined)
    {
        ui->lineEditValueRange->setText("Нет данных");
        ui->lineEditStatusRange->setStyleSheet(
                    "background-color: rgb(255, 195, 195)");
    }
    else
    {
        ui->lineEditValueRange->setText(QString::number(range.toDouble()));
        ui->lineEditStatusRange->setStyleSheet(
                    "background-color: rgb(195, 255, 195)");
    }

    if (azimuth == QJsonValue::Undefined)
    {
        ui->lineEditValueAzimuth->setText("Нет данных");
        ui->lineEditStatusAzimuth->setStyleSheet(
                    "background-color: rgb(255, 195, 195)");
    }
    else
    {
        ui->lineEditValueAzimuth->setText(QString::number(azimuth.toDouble()));
        ui->lineEditStatusAzimuth->setStyleSheet(
                    "background-color: rgb(195, 255, 195)");
    }

    if (course == QJsonValue::Undefined)
    {
        ui->lineEditValueCourse->setText("Нет данных");
        ui->lineEditStatusCourse->setStyleSheet(
                    "background-color: rgb(255, 195, 195)");
    }
    else
    {
        ui->lineEditValueCourse->setText(QString::number(course.toDouble()));
        ui->lineEditStatusCourse->setStyleSheet(
                    "background-color: rgb(195, 255, 195)");
    }

    if (glide == QJsonValue::Undefined)
    {
        ui->lineEditValueGlide->setText("Нет данных");
        ui->lineEditStatusGlide->setStyleSheet(
                    "background-color: rgb(255, 195, 195)");
    }
    else
    {
        ui->lineEditValueGlide->setText(QString::number(glide.toDouble()));
        ui->lineEditStatusGlide->setStyleSheet(
                    "background-color: rgb(195, 255, 195)");
    }

    if (statusPRMG_OMD == QJsonValue::Undefined)
    {
        ui->lineEditStatusPRMG_OMD->setText("Нет данных");
    }
    else
    {
        ui->lineEditStatusPRMG_OMD->setText(statusPRMG_OMD.toString());
    }

    if (recognitionPRMG_OMD == QJsonValue::Undefined)
    {
        ui->lineEditRecognitionPRMG_OMD->setText("Нет данных");
    }
    else
    {
        ui->lineEditRecognitionPRMG_OMD->setText(QString::number(recognitionPRMG_OMD.toDouble()));
    }
}

void MainWindow::setSignalsAndSlots()
{
    connect(ui->pushButtonSetOperationMode, &QPushButton::clicked,
            this, &MainWindow::onPushButtonSetOperationModeClicked);

    connect(ui->horizontalSliderFrequencyUpdate, &QSlider::valueChanged,
            this, &MainWindow::changeFrequencyUpdate);

    connect(ui->pushButtonInitARINC429, &QPushButton::clicked,
            this, &MainWindow::onPushButtonInitARINC429Clicked);

    connect(ui->pushButtonInitChannelsIO, &QPushButton::clicked,
            this, &MainWindow::onPushButtonInitChannelsIOClicked);

    connect(timer, &QTimer::timeout, this, &MainWindow::timerTick);

    connect(ui->pushButtonSaveReport, &QPushButton::clicked,
            this, &MainWindow::onPushButtonSaveReportClicked);

    connect(ui->pushButtonCalculateErrorRange, &QPushButton::clicked,
            this, &MainWindow::onPushButtonCalculateErrorRange);

    connect(ui->pushButtonCalculateErrorAzimuth, &QPushButton::clicked,
            this, &MainWindow::onPushButtonCalculateErrorAzimuth);

    connect(ui->pushButtonCalculateErrorCourse, &QPushButton::clicked,
            this, &MainWindow::onPushButtonCalculateErrorCourse);

    connect(ui->pushButtonCalculateErrorGlide, &QPushButton::clicked,
            this, &MainWindow::onPushButtonCalculateErrorGlide);

    connect(this, &MainWindow::equipmentTypeChangeRequested,
            &equipmentManager, &NavigationalEquipmentManager::equipmentTypeChange);

    connect(this, &MainWindow::operationModeChangeRequested,
            &equipmentManager, &NavigationalEquipmentManager::operationModeChange);

    connect(this, &MainWindow::frequencyUpdateChangeRequested,
            &equipmentManager, &NavigationalEquipmentManager::frequencyUpdateChange);

    connect(this, &MainWindow::channelsIOChangeRequested,
            &equipmentManager, &NavigationalEquipmentManager::channelsIOChange);

    connect(this, &MainWindow::calculateErrorRequested,
            &equipmentManager, &NavigationalEquipmentManager::calculateError);

    connect(&equipmentManager, &NavigationalEquipmentManager::parametersIsReady,
            this, &MainWindow::printData);
}

void MainWindow::initToolBar()
{
    QAction *action = new QAction("Выход");

    ui->menubar->addAction(action);

    connect(action, &QAction::triggered, this, &MainWindow::closeApplication);
}

void MainWindow::initStatusBar()
{
    labelUser = new QLabel("Оператор:");

    labelUser->setStyleSheet("background-color: white; \
                              border: 1px solid black");

    labelUser->setMinimumSize(120, 0);

    labelConnectionARINC429 = new QLabel("ARINC-429");

    labelConnectionARINC429->setStyleSheet("background-color: white; \
                              border: 1px solid black");

    labelConnectionARINC429->setMinimumSize(120, 0);

    labelConnectionARINC429->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    ui->statusbar->addWidget(labelUser);

    ui->statusbar->addWidget(labelConnectionARINC429);
}

void MainWindow::initTimer(int value)
{
    timer = new QTimer(this);
    timer->start(value);
}

void MainWindow::closeApplication()
{
    QApplication::exit(0);
}

void MainWindow::printDefaultData()
{
    ui->lineEditValueRange->setText("Нет данных");
    ui->lineEditStatusRange->setStyleSheet(
                "background-color: rgb(255, 195, 195)");

    ui->lineEditValueAzimuth->setText("Нет данных");
    ui->lineEditStatusAzimuth->setStyleSheet(
                "background-color: rgb(255, 195, 195)");

    ui->lineEditValueCourse->setText("Нет данных");
    ui->lineEditStatusCourse->setStyleSheet(
                "background-color: rgb(255, 195, 195)");

    ui->lineEditValueGlide->setText("Нет данных");
    ui->lineEditStatusGlide->setStyleSheet(
                "background-color: rgb(255, 195, 195)");
}

void MainWindow::onPushButtonSetOperationModeClicked()
{
    QJsonObject mode;

    mode.insert("Режим", ui->comboBoxMode->currentText());

    mode.insert("Тип РМ", ui->comboBoxRadiobeaconType->currentText());

    mode.insert("Диапазон", ui->comboBoxDiapason->currentText());

    mode.insert("ЧКК", ui->spinBoxNumberFCC->value());

    mode.insert("Тест", ui->checkBoxTestMode->isChecked());

    emit equipmentTypeChangeRequested(ui->comboBoxEquipmentType->currentText());

    emit operationModeChangeRequested(mode);
}

void MainWindow::changeFrequencyUpdate(int value)
{
    ui->labelFrequencyUpdate->setText(QString::number(value));

    emit frequencyUpdateChangeRequested(value);
}

void MainWindow::onPushButtonInitARINC429Clicked()
{
    InterfaceARINC429& instance = InterfaceARINC429::getInstance();

    instance.close();

    if (instance.initialize(0) == InterfaceARINC429::ARINC_OK)
    {
        labelConnectionARINC429->setStyleSheet(
                    "background-color: rgb(195, 255, 195); \
                     border: 1px solid black");
    }
    else
    {
        labelConnectionARINC429->setStyleSheet(
                    "background-color: rgb(255, 195, 195); \
                     border: 1px solid black");
    }
}

void MainWindow::onPushButtonInitChannelsIOClicked()
{
    QJsonObject channels;

    channels.insert("Вход", ui->spinBoxInputChannel->value());

    channels.insert("Выход", ui->spinBoxOutputChannel->value());

    channels.insert("Вход АПД", ui->spinBoxInputChannelAPD->value());

    channels.insert("Выход АПД", ui->spinBoxOutputChannelAPD->value());

    emit channelsIOChangeRequested(channels);
}

void MainWindow::timerTick()
{
    ui->lineEditDateTime->setText(
                QDateTime::currentDateTime().toString("dd.MM.yyyy | HH:mm:ss"));
}

void MainWindow::onPushButtonSaveReportClicked()
{
    QDir dir;

    if (!dir.exists("storage"))
    {
        dir.mkdir("storage");
    }

    QFile file;

    QString fileName = "storage//report_" +
            QDateTime::currentDateTime().toString("dd.MM.yyyy, HHmmss") + ".txt";

    qDebug() << fileName;

    file.setFileName(fileName);

    file.open(QIODevice::WriteOnly | QIODevice::Append);

    if (!file.isOpen())
        return;

    QString equipType = "Тип БНПК: " + ui->comboBoxEquipmentType->currentText();
    QString ctrlEquipType = "Тип КПА: " + ui->comboBoxControlEquipmentType->currentText();

    QString tvRange = "Дальность -> ИЗ: " + QString::number(ui->doubleSpinBoxRangeTrueValue->value());
    QString avgRange = "Дальность -> СЗ: " + ui->lineEditRangeAverageValue->text();
    QString errRange = "Дальность -> ОШ: " + ui->lineEditRangeErrorValue->text();
    QString smplRange = "Дальность -> Отсчеты: " + QString::number(ui->spinBoxRangeSamples->value());

    QString tvAzimuth = "Азимут -> ИЗ: " + QString::number(ui->doubleSpinBoxAzimuthTrueValue->value());
    QString avgAzimuth = "Азимут -> СЗ: " + ui->lineEditAzimuthAverageValue->text();
    QString errAzimuth = "Азимут -> ОШ: " + ui->lineEditAzimuthErrorValue->text();
    QString smplAzimuth = "Азимут -> Отсчеты: " + QString::number(ui->spinBoxAzimuthSamples->value());

    QString tvCourse = "Курс -> ИЗ: " + QString::number(ui->doubleSpinBoxCourseTrueValue->value());
    QString avgCourse = "Курс -> СЗ: " + ui->lineEditCourseAverageValue->text();
    QString errCourse = "Курс -> ОШ: " + ui->lineEditCourseErrorValue->text();
    QString smplCourse = "Курс -> Отсчеты: " + QString::number(ui->spinBoxCourseSamples->value());

    QString tvGlide = "Глиссада -> ИЗ: " + QString::number(ui->doubleSpinBoxGlideTrueValue->value());
    QString avgGlide = "Глиссада -> СЗ: " + ui->lineEditGlideAverageValue->text();
    QString errGlide = "Глиссада -> ОШ: " + ui->lineEditGlideErrorValue->text();
    QString smplGlide = "Глиссада -> Отсчеты: " + QString::number(ui->spinBoxGlideSamples->value());

    file.write("***************ОТЧЕТ***************\n");
    file.write(equipType.toUtf8() + "\n");
    file.write(ctrlEquipType.toUtf8() + "\n\n");
    file.write(tvRange.toUtf8() + "\n");
    file.write(avgRange.toUtf8() + "\n");
    file.write(errRange.toUtf8() + "\n");
    file.write(smplRange.toUtf8() + "\n\n");

    file.write(tvAzimuth.toUtf8() + "\n");
    file.write(avgAzimuth.toUtf8() + "\n");
    file.write(errAzimuth.toUtf8() + "\n");
    file.write(smplAzimuth.toUtf8() + "\n\n");

    file.write(tvCourse.toUtf8() + "\n");
    file.write(avgCourse.toUtf8() + "\n");
    file.write(errCourse.toUtf8() + "\n");
    file.write(smplCourse.toUtf8() + "\n\n");

    file.write(tvGlide.toUtf8() + "\n");
    file.write(avgGlide.toUtf8() + "\n");
    file.write(errGlide.toUtf8() + "\n");
    file.write(smplGlide.toUtf8() + "\n");
    file.write("***********************************\n");
}

void MainWindow::onPushButtonCalculateErrorRange()
{
    ui->progressBarRangeTick->setMaximum(
                ui->spinBoxCourseSamples->value());

    emit calculateErrorRequested("Дальность",
        ui->doubleSpinBoxRangeTrueValue->value(),
        ui->spinBoxRangeSamples->value());
}

void MainWindow::onPushButtonCalculateErrorAzimuth()
{
    ui->progressBarAzimuthTick->setMaximum(
                ui->spinBoxAzimuthSamples->value());

    emit calculateErrorRequested("Азимут",
        ui->doubleSpinBoxAzimuthTrueValue->value(),
        ui->spinBoxAzimuthSamples->value());
}

void MainWindow::onPushButtonCalculateErrorCourse()
{
    ui->progressBarCourseTick->setMaximum(
                ui->spinBoxCourseSamples->value());

    emit calculateErrorRequested("Курс",
        ui->doubleSpinBoxCourseTrueValue->value(),
        ui->spinBoxCourseSamples->value());
}

void MainWindow::onPushButtonCalculateErrorGlide()
{
    ui->progressBarGlideTick->setMaximum(
                ui->spinBoxGlideSamples->value());

    emit calculateErrorRequested("Глиссада",
        ui->doubleSpinBoxGlideTrueValue->value(),
                                 ui->spinBoxGlideSamples->value());
}

void MainWindow::updateProgressBarRangeError(int value)
{
    ui->progressBarRangeTick->setValue(
                ui->progressBarRangeTick->maximum() - value);
}

void MainWindow::updateProgressBarAzimuthError(int value)
{
    ui->progressBarAzimuthTick->setValue(
                ui->progressBarAzimuthTick->maximum() - value);
}

void MainWindow::updateProgressBarCourseError(int value)
{
    ui->progressBarCourseTick->setValue(
                ui->progressBarCourseTick->maximum() - value);
}

void MainWindow::updateProgressBarGlideError(int value)
{
    ui->progressBarGlideTick->setValue(
                ui->progressBarGlideTick->maximum() - value);
}

void MainWindow::printRangeAverageAndError(double average, double error)
{
    ui->lineEditRangeAverageValue->setText(QString::number(average));
    ui->lineEditRangeErrorValue->setText(QString::number(error));
}

void MainWindow::printAzimuthAverageAndError(double average, double error)
{
    ui->lineEditAzimuthAverageValue->setText(QString::number(average));
    ui->lineEditAzimuthErrorValue->setText(QString::number(error));
}

void MainWindow::printCourseAverageAndError(double average, double error)
{
    ui->lineEditCourseAverageValue->setText(QString::number(average));
    ui->lineEditCourseErrorValue->setText(QString::number(error));
}

void MainWindow::printGlideAverageAndError(double average, double error)
{
    ui->lineEditGlideAverageValue->setText(QString::number(average));
    ui->lineEditGlideErrorValue->setText(QString::number(error));
}

