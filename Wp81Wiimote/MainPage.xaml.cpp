//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include <ppltasks.h>
#include <strsafe.h>
#include "MainPage.xaml.h"
#include "Wiimote.h"

using namespace Wp81Wiimote;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;
using namespace std;
using namespace Windows::UI::Core;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter

	// TODO: Prepare page for display here.

	// TODO: If your application contains multiple pages, ensure that you are
	// handling the hardware Back button by registering for the
	// Windows::Phone::UI::Input::HardwareButtons.BackPressed event.
	// If you are using the NavigationHelper provided by some templates,
	// this event is handled for you.

	Refresh();
}

void MainPage::AppBarButton_Click(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e)
{
	Button^ b = (Button^)sender;
	if (b->Tag->ToString() == "Refresh")
	{
		// Refresh
		Refresh();
	}
}

void MainPage::Refresh()
{
	TextBoxWiimoteFound->Text = L"Looking for Wiimote...\n";

	create_task([this]()
	{
		// Allocate memory with the new keyword
		// don't use the stack because we want to use the BLUETOOTH_DEVICE_INFO outside lambdas
		BLUETOOTH_DEVICE_INFO* deviceInfo = new BLUETOOTH_DEVICE_INFO;
		ZeroMemory(deviceInfo, sizeof(BLUETOOTH_DEVICE_INFO));
		deviceInfo->dwSize = sizeof(BLUETOOTH_DEVICE_INFO);

		try
		{
			// Throw exception when Wiimote is not detected
			Wiimote::Detect(deviceInfo);

			// Allocate memory in the stack, will be free at the end of this lambda
			char address[18]; // length 18 = xx:xx:xx:xx:xx:xx[0] 
			Wiimote::BtAddressString(deviceInfo->Address, address, sizeof(address));
			char16 address16[18];
			// copy an array of char into an array of char16
			transform(address, address + 18, address16, [](char c) { return (char16)c; });

			UIConsoleAddText(L"\n"+ ref new Platform::String(deviceInfo->szName) + L" (" + ref new Platform::String(address16) + L")\n");

			if (deviceInfo->fAuthenticated != FALSE)
			{
				// Wiimote already paired
				UIConsoleAddText(L"Wiimote is paired.\n");

				if (deviceInfo->fConnected != FALSE)
				{

					UIConsoleAddText(L"Wiimote is connected.\n");
					
					HANDLE hDevice = INVALID_HANDLE_VALUE;
					hDevice = Wiimote::findDeviceHandle();

					// LED 1 = 0x10
					Wiimote::setLEDs(hDevice, 0x10);
					// Core Buttons = 0x30
					Wiimote::setDataReportingMode(hDevice, 0x30);

					// Read data
					UCHAR Buffer[255];
					DWORD BytesRead;
					while (true) {
						BOOL Result = ReadFile(hDevice, &Buffer, sizeof(Buffer), &BytesRead, NULL);
						if (!Result)
						{
							WCHAR buffer1[100];
							StringCbPrintfW(buffer1, sizeof(buffer1), L"Error ReadFile: %d\n", GetLastError());
							UIConsoleAddText(ref new Platform::String(buffer1));
						}

						WCHAR buffer3[100];
						StringCbPrintfW(buffer3, sizeof(buffer3), L"%04X ", (Buffer[1] << 8) + Buffer[2]);
						UIConsoleAddText(ref new Platform::String(buffer3));
					}
				}
				else
				{
					UIConsoleAddText(
						L"Wiimote is not connected.\n" + 
						L"Press buttons 1+2 of the Wiimote.\n" + 
						L"If Wiimote refuses to connect: Delete the Wiimote in the Bluetooth settings and restart the procedure.\n"
					);
				}
			}
			else
			{
				// Wiimote not paired
				UIConsoleAddText(L"Wiimote is not paired.\n");

				Wiimote::RegisterPairingHandle(deviceInfo);

				UIConsoleAddText(L"Pairing handle registred.\n"+
					L"Please go to the Bluetooth settings to pair the Wiimote:\n"+
					L"1a-If the Wiimote line is not visible: Press buttons 1+2 of the Wiimote.\n"+
					L"1b-If the status is 'pairing'; Tap Wiimote line to switch from 'pairing' to 'tap to pair'.\n"+
					L"2-Tap Wiimote line to open pairing window.\n"+
					L"3-Tap 'cancel' to close the pairing window.\n"
				);
			}
		}
		catch (char* reason)
		{
			char16 reason16[100];
			ZeroMemory(reason16, sizeof(reason16));
			transform(reason, reason + strlen(reason), reason16, [](char c) { return (char16)c; });

			UIConsoleAddText(ref new Platform::String(reason16) + L"\n");
		}
	});
}

void MainPage::UIConsoleAddText(Platform::String ^ text ) {
	Dispatcher->RunAsync(
		CoreDispatcherPriority::Normal,
		ref new DispatchedHandler([this, text]()
	{
		TextBoxWiimoteFound->Text += text;
	}));
}

