#include "interface_ARINC429.h"
#include <setupapi.h>
#include <winioctl.h>
#include <stdio.h>
#include <objbase.h>
#include <initguid.h>
#include "intrfacePCI3.h"
#include "ioctlPCI3.h"

#include <QDebug>


InterfaceARINC429::InterfaceARINC429() { }

HANDLE InterfaceARINC429::openDeviceByIndex(int index)
{
    GUID ClassGuid = GUID_DEVINTERFACE_PCI429_3WIN;

    HANDLE hDev;
    HDEVINFO hDevinfo;
    SP_DEVICE_INTERFACE_DATA	DeviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA	pDetail = nullptr;
    DWORD  ReqSize;

    DWORD pError = 0;

    hDevinfo = SetupDiGetClassDevs(LPGUID(& ClassGuid),
        nullptr,
        nullptr,
        DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

    if (hDevinfo == INVALID_HANDLE_VALUE)
    {
        pError = GetLastError();
        lastErrorMessage = "In function openDeviceByIndex(): ERROR: " + QString::number(pError);
        return INVALID_HANDLE_VALUE;
    }

    DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData);

    if (!SetupDiEnumDeviceInterfaces(hDevinfo,
        nullptr,
        LPGUID(& ClassGuid),
        DWORD(index),
        &DeviceInterfaceData))

    {
        pError = GetLastError();
        lastErrorMessage = "In function openDeviceByIndex(): ERROR: " + QString::number(pError);
        return INVALID_HANDLE_VALUE;
    }

    SetupDiGetDeviceInterfaceDetail(hDevinfo,
        &DeviceInterfaceData,
        nullptr,
        0,
        &ReqSize,
        nullptr);

    pDetail = PSP_INTERFACE_DEVICE_DETAIL_DATA(new char[ReqSize]);

    if (!pDetail)
    {
        pError = ERROR_NOT_ENOUGH_MEMORY;
        lastErrorMessage = "In function openDeviceByIndex(): ERROR: " + QString::number(pError);
        return INVALID_HANDLE_VALUE;
    }
    pDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

    if (!SetupDiGetDeviceInterfaceDetail(hDevinfo,
        &DeviceInterfaceData,
        pDetail,
        ReqSize,
        &ReqSize,
        nullptr)
        )
    {
        pError = GetLastError();
        lastErrorMessage = "In function openDeviceByIndex(): ERROR: " + QString::number(pError);
        return INVALID_HANDLE_VALUE;
    }

    SetupDiDestroyDeviceInfoList(hDevinfo);

    hDev = CreateFile(
        pDetail->DevicePath,
        0,//GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hDev == INVALID_HANDLE_VALUE)
    {
        pError = GetLastError();
        lastErrorMessage = "In function openDeviceByIndex(): ERROR: " + QString::number(pError);
        return INVALID_HANDLE_VALUE;
    }

    pError = ERROR_SUCCESS;

    return hDev;
}

InterfaceARINC429 &InterfaceARINC429::getInstance()
{
    static InterfaceARINC429 instance;

    return instance;
}

InterfaceARINC429::STATUS InterfaceARINC429::initialize(int deviceIndex)
{
    hDevice = openDeviceByIndex(deviceIndex);

    if (hDevice == INVALID_HANDLE_VALUE)
    {
        lastErrorMessage = "Could't open device by index: " + QString::number(deviceIndex);
        return STATUS::ARINC_ERROR;
    }

    struct
    {
        UCHAR freqSI[8];
        UCHAR freqSO[8];
    } bufInput;

    struct
    {
        UCHAR SI;
        UCHAR SO;
    } bufOutput;

    for (int i = 0; i < 8; ++i)
        bufInput.freqSI[i] = bufInput.freqSO[i] = 1;

    DeviceIoControl(hDevice, DRV2K_PCI429_3_INIT,
        &bufInput, 16, &bufOutput, 2, &nOutput, nullptr);

    if ((bufOutput.SI + bufOutput.SO) == 16)
        return STATUS::ARINC_OK;
    else
    {
        lastErrorMessage = "In function initialize(): ERROR: \
            Could't initialize channels I/O : " + QString::number(deviceIndex);

        return STATUS::ARINC_ERROR;
    }
}

void InterfaceARINC429::close()
{
    DeviceIoControl(hDevice, DRV2K_PCI429_3_STOP,
        nullptr, 0, nullptr, 0, &nOutput, nullptr);

    CloseHandle(hDevice);
}

InterfaceARINC429::STATUS InterfaceARINC429::startAddressReception(int channelNumber)
{
    struct
    {
        USHORT ChanNumber;
        USHORT ArrayNumber;
        USHORT InterrParamAddr;
        USHORT StopParamAddr;
    } bufInput;

    USHORT bufOutput;

    bufInput.ChanNumber = USHORT(channelNumber);
    bufInput.ArrayNumber = 1;
    bufInput.InterrParamAddr = 0xFF;
    bufInput.StopParamAddr = 0xFF;

    DeviceIoControl(hDevice, DRV2K_PCI429_3_SI_A_PUSK,
        &bufInput, 8, &bufOutput, 2,
        &nOutput, nullptr);

    if (bufOutput == 0)
    {
        return STATUS::CHANNEL_OK;
    }
    else if (bufOutput == 1)
    {
        lastErrorMessage = "In function startAddressReception(): ERROR: Wrong channel number";
        return STATUS::CHANNEL_ERROR;
    }
    else if (bufOutput == 2)
    {
        lastErrorMessage = "In function startAddressReception(): ERROR: Wrong buffer number";
        return STATUS::CHANNEL_ERROR;
    }
}

InterfaceARINC429::STATUS InterfaceARINC429::stopAddressReception(int channelNumber)
{
    USHORT ChanNumber = USHORT(channelNumber);
    USHORT bufOutput;

    DeviceIoControl(hDevice, DRV2K_PCI429_3_SI_STOP,
        &ChanNumber, 2, &bufOutput, 2,
        &nOutput, nullptr);

    if (bufOutput == 0)
    {
        return STATUS::CHANNEL_OK;
    }
    else
    {
        lastErrorMessage = "In function stopAddressReception(): ERROR: Wrong channel number";
        return STATUS::CHANNEL_ERROR;
    }
}

InterfaceARINC429::STATUS InterfaceARINC429::popFromChannel(int channelNumber, unsigned long *data, int dataSize)
{
    struct
    {
        USHORT ChanNumber;
        USHORT ArrayNumber;
    } bufInput;

    struct BUFOUTPUT
    {
        USHORT error;
        unsigned long param[256];
    } bufOutput;

    bufInput.ChanNumber = USHORT(channelNumber);
    bufInput.ArrayNumber = 1;

    DeviceIoControl(hDevice, DRV2K_PCI429_3_SI_BUFER,
        &bufInput, 4, &bufOutput, 1026,
        &nOutput, nullptr);

    for (int i = 0; i < dataSize; ++i)
        data[i] = bufOutput.param[i];

    if (bufOutput.error == 1)
    {
        lastErrorMessage = "In function popFromChannel(): ERROR: Wrong channel number";
        return STATUS::CHANNEL_ERROR;
    }
    else if (bufOutput.error == 2)
    {
        lastErrorMessage = "In function popFromChannel(): ERROR: Wrong array number";
        return STATUS::CHANNEL_ERROR;
    }

    return STATUS::CHANNEL_OK;
}

InterfaceARINC429::STATUS InterfaceARINC429::pushToChannel(int channelNumber, unsigned long *data, int dataSize)
{
    struct
    {
        USHORT ChanNumber;
        USHORT ArrayDim;
        USHORT Period;
        USHORT InterrMask;
        unsigned long param[256];
    } bufInput;


    USHORT bufOutput = 0;

    bufInput.ChanNumber = USHORT(channelNumber);
    bufInput.ArrayDim = 1;
    bufInput.Period = 1;
    bufInput.InterrMask = 0;

    for (int i = 0; i < dataSize; ++i)
        bufInput.param[i] = data[i];

    DWORD nbufInputSize = sizeof(bufInput);

    DeviceIoControl(hDevice, DRV2K_PCI429_3_SO_C_PUSK,
        &bufInput, nbufInputSize, &bufOutput, 2,
            &nOutput, nullptr);

    if (bufOutput == 1)
    {
        lastErrorMessage = "In function pushToChannel(): ERROR: Wrong channel number";
        return STATUS::CHANNEL_ERROR;
    }
    else if (bufOutput == 2)
    {
        lastErrorMessage = "In function pushToChannel(): ERROR: Wrong period";
        return STATUS::CHANNEL_ERROR;
    }

    /*DeviceIoControl(hDevice, DRV2K_PCI429_3_SO_STOP,
        &bufInput.ChanNumber, 2, &bufOutput, 2,
        &nOutput, nullptr);*/

    return STATUS::CHANNEL_OK;
}

InterfaceARINC429::STATUS InterfaceARINC429::getStatusInputChannel(int channelNumber)
{
    USHORT ChanNumber = USHORT(channelNumber);

    struct
    {
        UCHAR SIstate;
        UCHAR pkNumber;
    } bufOutput;

    DeviceIoControl(hDevice, DRV2K_PCI429_3_SI_STATE,
        &ChanNumber, 2, &bufOutput, 2,
        &nOutput, nullptr);

    if (bufOutput.SIstate == 0)
    {
        lastErrorMessage = "In function pushToChannel(): WARNING: Channel is stopped";
        return STATUS::CHANNEL_WARNING;
    }
    else if (bufOutput.SIstate == 1)
    {
        return STATUS::CHANNEL_OK;
    }
    else
    {
        lastErrorMessage = "In function pushToChannel(): WARNING: Wrong channel number";
        return STATUS::CHANNEL_ERROR;
    }
}

InterfaceARINC429::STATUS InterfaceARINC429::getStatusOutputChannel(int channelNumber)
{
    USHORT ChanNumber = USHORT(channelNumber);

    struct
    {
        UCHAR SOstate;
        UCHAR pkNumber;
    } bufOutput;

    DeviceIoControl(hDevice, DRV2K_PCI429_3_SO_STATE,
        &ChanNumber, 2, &bufOutput, 2,
        &nOutput, nullptr);

    if (bufOutput.SOstate == 0)
    {
        lastErrorMessage = "In function pushToChannel(): WARNING: Channel is stopped";
        return STATUS::CHANNEL_WARNING;
    }
    else if (bufOutput.SOstate == 1)
    {
        return STATUS::CHANNEL_OK;
    }
    else
    {
        lastErrorMessage = "In function pushToChannel(): WARNING: Wrong channel number";
        return STATUS::CHANNEL_ERROR;
    }
}

QString InterfaceARINC429::getLastErrorMessage()
{
    return lastErrorMessage;
}

HANDLE InterfaceARINC429::getHandle()
{
    return hDevice;
}
