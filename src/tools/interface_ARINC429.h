#ifndef INTERFACE_ARINC429_H
#define INTERFACE_ARINC429_H

#include <windows.h>
#include <QString>

// Класс реализует шаблон проектирования SINGLETON.
// Взаимодействие с методами класса осуществляется
// путем получения статического экземпляра вызовом
// метода getInstance()
class InterfaceARINC429
{
private:
    InterfaceARINC429();
    InterfaceARINC429(const InterfaceARINC429&);
    bool operator+(const InterfaceARINC429&);

    HANDLE openDeviceByIndex(int index);

    // Дескриптор устройства
    HANDLE hDevice;

    DWORD nOutput;

    QString lastErrorMessage;

public:
    enum STATUS {
        ARINC_OK,
        ARINC_ERROR,
        ARINC_WARNING,
        CHANNEL_OK,
        CHANNEL_ERROR,
        CHANNEL_WARNING
    };

    // Метод возвращает статический экземпляр
    // класса
    static InterfaceARINC429& getInstance();

    // Метод инициализирует устройство обмена
    // по каналу ARINC-429.
    // На вход передается индекс устройства в системе (по умолчанию: 0);
    STATUS initialize(int deviceIndex);

    // Метод завершает работу устройства обмена
    // по каналу ARINC-429 и освобождает ресурсы
    void close();

    // Метод подготавливает канал, передаваемый
    // в переменную channelNumber для адресного приема
    STATUS startAddressReception(int channelNumber);

    // Останов адресного приема
    STATUS stopAddressReception(int channelNumber);

    // Получение массива данных по указанному каналу
    STATUS popFromChannel(int channelNumber, unsigned long *data, int dataSize);

    // Запись массива данных в указанный канал
    STATUS pushToChannel(int channelNumber, unsigned long *data, int dataSize);

    // Получение статуса работы входного канала
    STATUS getStatusInputChannel(int channelNumber);

    // Получение статуса работы выходного канала
    STATUS getStatusOutputChannel(int channelNumber);

    // Получение сообщения о последней ошибке
    QString getLastErrorMessage();

    // Получить дескриптор устройства
    HANDLE getHandle();

};

#endif // INTERFACE_ARINC429_H
