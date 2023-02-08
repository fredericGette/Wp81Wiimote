/*
* See https://github.com/jloehr/Wiimote-HIDAPI
*/

#include "pch.h"
#include "Wiimote.h"

#pragma comment(lib,"bluetoothapis.lib") 
#pragma comment(lib,"RPCRT4.lib") 
#pragma comment(lib,"hid.lib") 

void Wiimote::Debug(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[100];
	vsnprintf_s(buffer, sizeof(buffer), format, args);

	OutputDebugStringA(buffer);

	va_end(args);
}

void Wiimote::DebugBtAddress(BLUETOOTH_ADDRESS address)
{
	char buffer[18];
	ZeroMemory(buffer, sizeof(buffer));
	BtAddressString(address, buffer, sizeof(buffer));
	Debug("%s (%I64u)", buffer, address.ullLong);
}

void Wiimote::BtAddressString(BLUETOOTH_ADDRESS address, char*buffer, int bufferSize)
{
	ZeroMemory(buffer, bufferSize);
	sprintf_s(buffer, bufferSize, (char*)"%02X:%02X:%02X:%02X:%02X:%02X", address.rgBytes[5], address.rgBytes[4], address.rgBytes[3], address.rgBytes[2], address.rgBytes[1], address.rgBytes[0]);
}

void Wiimote::DebugSystemTime(SYSTEMTIME time)
{
	Debug("%d-%02d-%02d %02d:%02d:%02d.%03d",
		time.wYear,
		time.wMonth,
		time.wDay,
		time.wHour,
		time.wMinute,
		time.wSecond,
		time.wMilliseconds);
}

void Wiimote::Detect(BLUETOOTH_DEVICE_INFO * device_info)
{
	Debug("### Start using BluetoothAPIs ###\n");

	Debug("\nFind local bluetooth radio...\n");

	// Get handle of the first local bluetooth radio
	BLUETOOTH_FIND_RADIO_PARAMS radio_params;
	ZeroMemory(&radio_params, sizeof(radio_params));
	radio_params.dwSize = sizeof(BLUETOOTH_FIND_RADIO_PARAMS);
	HANDLE radio_handle;
	HBLUETOOTH_RADIO_FIND radio_search_result = BluetoothFindFirstRadio(&radio_params, &radio_handle);
	if (radio_search_result == NULL)
	{
		Debug("Error BluetoothFindFirstRadio: %d\n", GetLastError());
		throw "Please check that Bluetooth is on.";
	}

	// Get info from the handle
	BLUETOOTH_RADIO_INFO radio_info;
	ZeroMemory(&radio_info, sizeof(radio_info));
	radio_info.dwSize = sizeof(radio_info);
	DWORD radio_info_result = BluetoothGetRadioInfo(radio_handle, (BLUETOOTH_RADIO_INFO*)&radio_info);

	Debug("\tLocal Bluetooth radio name: "); OutputDebugString(radio_info.szName); Debug("\n");
	Debug("\tManufacturer: (0x001D=Qualcomm) %04X\n", radio_info.manufacturer);
	Debug("\tManufacturer SubVersion: 0x%X\n", radio_info.lmpSubversion);
	Debug("\tClass of device: Major (0x2=PHONE) 0x%X - Minor (0x3=SMART) 0x%X - Service 0x%04X\n", GET_COD_MAJOR(radio_info.ulClassofDevice), GET_COD_MINOR(radio_info.ulClassofDevice), GET_COD_SERVICE(radio_info.ulClassofDevice));
	Debug("\tLocal address: "); DebugBtAddress(radio_info.address); Debug("\n");

	Debug("\nFind remote bluetooth device...\n");

	BLUETOOTH_DEVICE_SEARCH_PARAMS device_search_params;
	ZeroMemory(&device_search_params, sizeof(device_search_params));
	device_search_params.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
	device_search_params.fReturnAuthenticated = true;
	device_search_params.fReturnRemembered = true;
	device_search_params.fReturnUnknown = true;
	device_search_params.fReturnConnected = true;
	device_search_params.fIssueInquiry = true;
	device_search_params.cTimeoutMultiplier = 4;
	device_search_params.hRadio = radio_handle;

	HBLUETOOTH_DEVICE_FIND device_search = BluetoothFindFirstDevice(&device_search_params, device_info);
	if (device_search == NULL) {
		Debug("Error BluetoothFindFirstDevice: %d\n", GetLastError());
		throw "Wiimote not detect. Please check that Wiimote is on. Press buttons 1+2.";
	}

	do
	{
		Debug("\tDevice found: "); OutputDebugString(device_info->szName); Debug("\n");
		Debug("\tDevice address: "); DebugBtAddress(device_info->Address); Debug("\n");
		Debug("\tAuthenticated: %s (%d)\n", (device_info->fAuthenticated != FALSE) ? "True" : "False", device_info->fAuthenticated);
		Debug("\tRemembered: %s (%d)\n", (device_info->fRemembered != FALSE) ? "True" : "False", device_info->fRemembered);
		Debug("\tConnected: %s (%d)\n", (device_info->fConnected != FALSE) ? "True" : "False", device_info->fConnected);
		Debug("\tLastSeen UTC: "); DebugSystemTime(device_info->stLastSeen); Debug("\n");
		Debug("\tLastUsed UTC: "); DebugSystemTime(device_info->stLastUsed); Debug("\n");
		Debug("\tClass Of Device: Major (0x5=PERIPHERAL) 0x%X - Minor (0x1=JOYSTICK) 0x%X - Service 0x%04X\n", GET_COD_MAJOR(device_info->ulClassofDevice), GET_COD_MINOR(device_info->ulClassofDevice), GET_COD_SERVICE(device_info->ulClassofDevice));
		Debug("\n");
	} while (wcsncmp(device_info->szName, L"Nintendo", 8) != 0 && BluetoothFindNextDevice(device_search, device_info));

	if (wcsncmp(device_info->szName, L"Nintendo", 8) != 0)
	{
		throw "Wiimote not detect. Please check that Wiimote is on. Press buttons 1+2.";
	}
}

void Wiimote::RegisterPairingHandle(BLUETOOTH_DEVICE_INFO * device_info)
{
	Debug("Register Authentication handler...\n");
	HBLUETOOTH_AUTHENTICATION_REGISTRATION hRegHandle = 0;
	DWORD dwRet = BluetoothRegisterForAuthenticationEx(device_info, &hRegHandle, (PFN_AUTHENTICATION_CALLBACK_EX)&BluetoothAuthCallback, NULL);
	if (dwRet != ERROR_SUCCESS)
	{
		Debug("BluetoothRegisterForAuthenticationEx ret %d\n", dwRet);
	}
}

bool Wiimote::BluetoothAuthCallback(LPVOID pvParam, PBLUETOOTH_AUTHENTICATION_CALLBACK_PARAMS  pAuthCallbackParams)
{
	Debug("Request authentication from device: "); OutputDebugString(pAuthCallbackParams->deviceInfo.szName); Debug("\n");
	Debug("Device address: "); DebugBtAddress(pAuthCallbackParams->deviceInfo.Address); Debug("\n");

	Debug("authenticationMethod: %d\n", pAuthCallbackParams->authenticationMethod);
	Debug("authenticationRequirements: %d\n", pAuthCallbackParams->authenticationRequirements);
	Debug("ioCapability: %d\n", pAuthCallbackParams->ioCapability);
	Debug("Numeric_Value: %d\n", pAuthCallbackParams->Numeric_Value);
	Debug("Passkey: %d\n", pAuthCallbackParams->Passkey);

	BLUETOOTH_AUTHENTICATE_RESPONSE response = { 0 };
	response.authMethod = pAuthCallbackParams->authenticationMethod;
	response.bthAddressRemote = pAuthCallbackParams->deviceInfo.Address;
	response.negativeResponse = false;
	response.pinInfo.pin[0] = pAuthCallbackParams->deviceInfo.Address.rgBytes[0];
	response.pinInfo.pin[1] = pAuthCallbackParams->deviceInfo.Address.rgBytes[1];
	response.pinInfo.pin[2] = pAuthCallbackParams->deviceInfo.Address.rgBytes[2];
	response.pinInfo.pin[3] = pAuthCallbackParams->deviceInfo.Address.rgBytes[3];
	response.pinInfo.pin[4] = pAuthCallbackParams->deviceInfo.Address.rgBytes[4];
	response.pinInfo.pin[5] = pAuthCallbackParams->deviceInfo.Address.rgBytes[5];
	response.pinInfo.pinLength = 6;
	DWORD dwRet = BluetoothSendAuthenticationResponseEx(NULL, &response);
	if (dwRet != ERROR_SUCCESS)
	{
		Debug("BluetoothSendAuthenticationResponseEx ret: %d\n", dwRet);
	}

	return true;
}

HANDLE Wiimote::findDeviceHandle()
{
	// Construct the expected device name (the first part seems to be enough):
	// HID#<UUID_HID_SERVICE>_LOCALMFG&<QUALCOMM>#7&3a273ef&0&0000#<GUID_DEVINTERFACE_HID>
	RPC_WSTR wcharUuid;
	UuidToStringW(&HumanInterfaceDeviceServiceClass_UUID, &wcharUuid);
	std::wstring wstringStartDeviceName = L"HID#{";
	wstringStartDeviceName += std::wstring((WCHAR*)wcharUuid);
	wstringStartDeviceName += L"}";

	// List all devices
	WCHAR devicenames[100 * MAX_PATH] = L"";
	DWORD resultQDD = QueryDosDeviceW(NULL, devicenames, ARRAYSIZE(devicenames));
	if (resultQDD == ERROR_SUCCESS)
	{
		Debug("QueryDosDeviceW failed with error code %d\n", GetLastError());
	}

	// Find the device having the expected name and corresponding to a Nintendo device

	HANDLE hDevice = INVALID_HANDLE_VALUE;
	WCHAR * deviceName = devicenames;
	for (DWORD i = 0; i < resultQDD; i++) {
		if (devicenames[i] == '\0') {
			if (wcsncmp(deviceName, wstringStartDeviceName.c_str(), wstringStartDeviceName.length()) == 0)
			{
				Debug("HumanInterfaceDeviceServiceClass_UUID found:\n\t");
				OutputDebugString(deviceName);
				OutputDebugString(L"\n");

				std::wstring wstringDeviceName(deviceName);
				std::string stringDeviceName(wstringDeviceName.begin(), wstringDeviceName.end());
				std::string stringDeviceFileName = "\\\\.\\" + stringDeviceName;

				Debug("Open device file: ");	OutputDebugStringA(stringDeviceFileName.c_str()); Debug("\n");
				DWORD desired_access = GENERIC_READ | GENERIC_WRITE;
				DWORD share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
				hDevice = CreateFileA(stringDeviceFileName.c_str(),
					desired_access,
					share_mode,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
				if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the device file
				{
					Debug("Error CreateFileA: %d\n", GetLastError());
				}
				else
				{
					WCHAR ProductName[255];
					ZeroMemory(ProductName, sizeof(ProductName));

					if (HidD_GetProductString(hDevice, ProductName, 255))
					{
						Debug("HID Name: "); OutputDebugString(ProductName); Debug("\n");

						if (wcsncmp(ProductName, L"Nintendo", 8) == 0)
						{
							Debug("Wiimote found.\n");
							break;
						}
						else
						{
							Debug("Wiimote not detect. Please check that Wiimote is on. Press buttons 1+2.\n");
						}
					}
					else
					{
						Debug("Can not check the product name of the HID device.\n");
					}
				}
			}

			// next device name
			deviceName = devicenames + i + 1;
		}
	}

	if (hDevice != INVALID_HANDLE_VALUE)
	{


		PHIDP_PREPARSED_DATA PreparsedData = NULL;
		BOOL Result = HidD_GetPreparsedData(hDevice, &PreparsedData);
		if (!Result)
		{
			Debug("Error HidD_GetPreparsedData: %d\n", Result);
		}

		HIDP_CAPS Caps;
		NTSTATUS Status = HidP_GetCaps(PreparsedData, &Caps);
		if (Status < 0)
		{
			Debug("Error HidP_GetCaps: %d\n", Status);
			HidD_FreePreparsedData(PreparsedData);
		}

		Debug("Capabilities:\n");
		Debug("\tUsage (0x05 = Game Pad): 0x%X\n", Caps.Usage); // https://docs.microsoft.com/en-us/windows-hardware/drivers/hid/hid-usages#usage-id
		Debug("\tUsagePage (0x01 = Generic Desktop Controls): 0x%X\n", Caps.UsagePage); // https://docs.microsoft.com/en-us/windows-hardware/drivers/hid/hid-usages#usage-page
		Debug("\tInputReportByteLength: %d\n", Caps.InputReportByteLength);
		Debug("\tOutputReportByteLength: %d\n", Caps.OutputReportByteLength);
		Debug("\tFeatureReportByteLength: %d\n", Caps.FeatureReportByteLength);

		HidD_FreePreparsedData(PreparsedData);
	}
	
	return hDevice;
}

// https://wiibrew.org/wiki/Wiimote/Protocol#Player_LEDs
// report id=0x11, set LEDs
// LEDs 2&3 = 0x20+0x40=0x60
// LED 1 = 0x10
// LED 4 = 0x80
void Wiimote::setLEDs(HANDLE hDevice, UCHAR flag) {

	DataBuffer BufferSetLED({ 0x11, flag });

	writeData(hDevice, BufferSetLED);
}

void Wiimote::writeData(HANDLE hDevice, DataBuffer buffer) {
	DWORD BytesWritten;
	BOOL Result = WriteFile(hDevice, buffer.data(), (DWORD)buffer.size(), &BytesWritten, NULL);
	if (!Result)
	{
		DWORD Error = GetLastError();
		Debug("Error WriteFile (997 = ERROR_IO_PENDING): %d\n", Error);
	}
}

// MM = mode number, 0x30: Core Buttons
void Wiimote::setDataReportingMode(HANDLE hDevice, UCHAR modeNumber) {

	// report id=0x12, set Data Reporting mode
	// TT MM
	// TT = 0 no continuous reporting, report only when data has changed
	// MM = mode number, 0x30: Core Buttons
	DataBuffer BufferSetDataReportingMode({ 0x12, 0x00, 0x30 });

	writeData(hDevice, BufferSetDataReportingMode);
}
