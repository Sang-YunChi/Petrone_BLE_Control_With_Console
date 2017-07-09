#pragma once
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <bthdef.h>
#include <Bluetoothleapis.h>
#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothApis.lib")
#define PETRONE_CUSTOM_SERVICE_UUID "{C320DF00-7891-11E5-8BCF-FEFF819CDC9F}" //we use UUID for an Petrone device 
//#define PETRONE_CUSTOM_SERVICE_UUID "{C320DF01-7891-11E5-8BCF-FEFF819CDC9F}" //we use UUID for an Petrone device 
//#define PETRONE_CUSTOM_SERVICE_UUID "{C320DF02-7891-11E5-8BCF-FEFF819CDC9F}" //we use UUID for an Petrone device 

using namespace std;

#pragma region Variables Region
PBTH_LE_GATT_SERVICE pServiceBuffer, currGattServ;
HANDLE hLEDevice;
HRESULT hr;
PBTH_LE_GATT_CHARACTERISTIC pCharBuffer, currGattChar, pWritableChar;
PBTH_LE_GATT_DESCRIPTOR pDescriptorBuffer, currGattDescriptor;
PBTH_LE_GATT_DESCRIPTOR_VALUE pDescValueBuffer;
#pragma endregion

#pragma region Petrone Enum Class Data Implementation
enum class DataType
{
	None = 0,
	// �ý��� ����
	Ping, // ��� Ȯ��(reserve)
	Ack, // ������ ���ſ� ���� ����
	Error, // ����(reserve, ��Ʈ �÷��״� ���Ŀ� ����)
	Request, // ������ Ÿ���� ������ ��û
	Passcode, // ���ο� �� ��� ��ȣ(�� ��� ��ȣ ���� �� ���)
			  // ����, ���
			  Control = 0x10, // ����
			  Command, // ���
			  Command2, // ���� ���(2���� ������ ���ÿ� ����)
			  Command3, // ���� ���(3���� ������ ���ÿ� ����)
						// LED
						LedMode = 0x20, // LED ��� ����
						LedMode2, // LED ��� 2�� ����
						LedModeCommand, // LED ���, Ŀ�ǵ�
						LedModeCommandIr, // LED ���, Ŀ�ǵ�, IR ������ �۽�
						LedModeColor, // LED ��� 3�� ���� ����
						LedModeColor2, // LED ��� 3�� ���� ���� 2��
						LedEvent, // LED �̺�Ʈ
						LedEvent2, // LED �̺�Ʈ 2��,
						LedEventCommand, // LED �̺�Ʈ, Ŀ�ǵ�
						LedEventCommandIr, // LED �̺�Ʈ, Ŀ�ǵ�, IR ������ �۽�
						LedEventColor, // LED �̺�Ʈ 3�� ���� ����
						LedEventColor2, // LED �̺�Ʈ 3�� ���� ���� 2��
						LedModeDefaultColor, // LED �ʱ� ��� 3�� ���� ����
						LedModeDefaultColor2, // LED �ʱ� ��� 3�� ���� ���� 2��
											  // ����
											  Address = 0x30, // IEEE address
											  State, // ����� ����(���� ���, ��������, ���͸���)
											  Attitude, // ����� �ڼ�
											  GyroBias, // ���̷� ���̾ ��
											  TrimAll, // ��ü Ʈ��
											  TrimFlight, // ���� Ʈ��
											  TrimDrive, // ���� Ʈ��
														 // ������ �ۼ���
														 IrMessage = 0x40, // IR ������ �ۼ���
														 EndOfType
};
enum class PetroneCommandType
{
	None = 0, // �̺�Ʈ ����
			  // ����
			  ModePetrone = 0x10, // ��Ʈ�� ���� ��� ��ȯ
								  // ����
								  Coordinate = 0x20, // ���� ���� ����
								  Trim, // Ʈ�� ����
								  FlightEvent, // ���� �̺�Ʈ ����
								  DriveEvent, // ���� �̺�Ʈ ����
								  Stop, // ����
								  ResetHeading = 0x50, // ������ ����(�ۼַ�Ʈ ��� �� �� ���� heading�� 0���� ����)
								  ClearGyroBiasAndTrim, // ���̷� ���̾�� Ʈ�� ���� �ʱ�ȭ
														//���
														PairingActivate = 0x80, // �� Ȱ��ȭ
														PairingDeactivate, // �� ��Ȱ��ȭ
														TerminateConnection, // ���� ����
																			 // ��û
																			 Request = 0x90, // ������ Ÿ���� ������ ��û
																			 EndOfType
};
enum class ModePetrone
{
	None = 0,
	Flight = 0x10, // ���� ���(���� ����)
	FlightNoGuard, // ���� ���(���� ����)
	FlightFPV, // ���� ���(FPV)
	Drive = 0x20, // ���� ���
	DriveFPV, // ���� ���(FPV)
	Test = 0x30, // �׽�Ʈ ���
	EndOfType
};
enum class Coordinate
{
	None = 0,
	Absolute, // ���� ��ǥ��
	Relative, // ��� ��ǥ��
	EndOfType
};
enum class Trim
{
	None = 0,
	RollIncrease, // Roll ����
	RollDecrease, // Roll ����
	PitchIncrease, // Pitch ����
	PitchDecrease, // Pitch ����
	YawIncrease, // Yaw ����
	YawDecrease, // Yaw ����
	ThrottleIncrease, // Throttle ����
	ThrottleDecrease, // Throttle ����
	EndOfType
};
enum class FlightEvent
{
	None = 0,
	TakeOff, // �̷�
	FlipFront, // ȸ��
	FlipRear, // ȸ��
	FlipLeft, // ȸ��
	FlipRight, // ȸ��
	Stop, // ����
	Landing, // ����
	Reverse, // ������
	Shot, // �̻����� �� �� ������
	UnderAttack, // �̻����� ���� �� ������
	Square, // ������ ����
	CircleLeft, // �������� ȸ��
	CircleRight, // ���������� ȸ��
	Rotate180, // 180�� ȸ��
	EndOfType
};
enum class Request
{
	// ����
	Address = 0x30, // IEEE address
	State, // ����� ����(���� ���, ��������, ���͸���)
	Attitude, // ����� �ڼ�(Vector)
	GyroBias, // ���̷� ���̾ ��(Vector)
	TrimAll, // ��ü Ʈ��
	TrimFlight, // ���� Ʈ��
	TrimDrive, // ���� Ʈ��
};
enum class LEDMode
{
	None = 0,
	EyeNone = 0x10,
	EyeHold, // ������ ������ ��� ��
	EyeMix, // ���������� LED �� ����
	EyeFlicker, // ������
	EyeFlickerDouble, // ������(�� �� �����̰� ������ �ð���ŭ ����)
	EyeDimming, // ��� �����Ͽ� õõ�� ������
	ArmNone = 0x40,
	ArmHold, // ������ ������ ��� ��
	ArmMix, // ���������� LED �� ����
	ArmFlicker, // ������
	ArmFlickerDouble, // ������(�� �� �����̰� ������ �ð���ŭ ����)
	ArmDimming, // ��� �����Ͽ� õõ�� ������
	ArmFlow, // �տ��� �ڷ� �帧
	ArmFlowReverse, // �ڿ��� ������ �帧
	EndOfType
};
enum class Colors
{
	AliceBlue, AntiqueWhite, Aqua,
	Aquamarine, Azure, Beige,
	Bisque, Black, BlanchedAlmond,
	Blue, BlueViolet, Brown,
	BurlyWood, CadetBlue, Chartreuse,
	Chocolate, Coral, CornflowerBlue,
	Cornsilk, Crimson, Cyan,
	DarkBlue, DarkCyan, DarkGoldenRod,
	DarkGray, DarkGreen, DarkKhaki,
	DarkMagenta, DarkOliveGreen, DarkOrange,
	DarkOrchid, DarkRed, DarkSalmon,
	DarkSeaGreen, DarkSlateBlue, DarkSlateGray,
	DarkTurquoise, DarkViolet, DeepPink,
	DeepSkyBlue, DimGray, DodgerBlue,
	FireBrick, FloralWhite, ForestGreen,
	Fuchsia, Gainsboro, GhostWhite,
	Gold, GoldenRod, Gray,
	Green, GreenYellow, HoneyDew,
	HotPink, IndianRed, Indigo,
	Ivory, Khaki, Lavender,
	LavenderBlush, LawnGreen, LemonChiffon,
	LightBlue, LightCoral, LightCyan,
	LightGoldenRodYellow, LightGray, LightGreen,
	LightPink, LightSalmon, LightSeaGreen,
	LightSkyBlue, LightSlateGray, LightSteelBlue,
	LightYellow, Lime, LimeGreen,
	Linen, Magenta, Maroon,
	MediumAquaMarine, MediumBlue, MediumOrchid,
	MediumPurple, MediumSeaGreen, MediumSlateBlue,
	MediumSpringGreen, MediumTurquoise, MediumVioletRed,
	MidnightBlue, MintCream, MistyRose,
	Moccasin, NavajoWhite, Navy,
	OldLace, Olive, OliveDrab,
	Orange, OrangeRed, Orchid,
	PaleGoldenRod, PaleGreen, PaleTurquoise,
	PaleVioletRed, PapayaWhip, PeachPuff,
	Peru, Pink, Plum,
	PowderBlue, Purple, RebeccaPurple,
	Red, RosyBrown, RoyalBlue,
	SaddleBrown, Salmon, SandyBrown,
	SeaGreen, SeaShell, Sienna,
	Silver, SkyBlue, SlateBlue,
	SlateGray, Snow, SpringGreen,
	SteelBlue, Tan, Teal,
	Thistle, Tomato, Turquoise,
	Violet, Wheat, White,
	WhiteSmoke, Yellow, YellowGreen,
	EndOfType
};
enum class ModeFlight
{
	None = 0,
	Ready, // ���� �غ�
	TakeOff, // �̷� (Flight�� �ڵ���ȯ)
	Flight, // ����
	Flip, // ȸ��
	Stop, // ���� ����
	Landing, // ����
	Reverse, // ������
	Accident, // ��� (Ready�� �ڵ���ȯ)
	Error, // ����
	EndOfType
};
enum class ModeDrive
{
	None = 0,
	Ready, // �غ�
	Start, // ���
	Drive, // ����Stop, // ���� ����
	Accident, // ��� (Ready�� �ڵ���ȯ)
	Error, // ����
	EndOfType
};
enum class SensorOrientation
{
	None = 0,
	Normal, // ����
	ReverseStart, // �������� ����
	Reverse, // ������
	EndOfType
};
#pragma endregion

#pragma region Command Sample
UCHAR *ArmCyan = new UCHAR[4]{ (UCHAR)DataType::LedMode, (UCHAR)LEDMode::ArmDimming, (UCHAR)Colors::Cyan, (UCHAR)0x05 };
UCHAR *EyeCyan = new UCHAR[4]{ (UCHAR)DataType::LedMode, (UCHAR)LEDMode::EyeDimming, (UCHAR)Colors::Cyan, (UCHAR)0x05 };
UCHAR *ArmYellow = new UCHAR[4]{ (UCHAR)DataType::LedMode, (UCHAR)LEDMode::ArmDimming, (UCHAR)Colors::Yellow, (UCHAR)0x05 };
UCHAR *EyeYellow = new UCHAR[4]{ (UCHAR)DataType::LedMode, (UCHAR)LEDMode::EyeDimming, (UCHAR)Colors::Yellow, (UCHAR)0x05 };
UCHAR *ArmRed = new UCHAR[4]{ (UCHAR)DataType::LedMode, (UCHAR)LEDMode::ArmDimming, (UCHAR)Colors::Red, (UCHAR)0x05 };
UCHAR *EyeRed = new UCHAR[4]{ (UCHAR)DataType::LedMode, (UCHAR)LEDMode::EyeDimming, (UCHAR)Colors::Red, (UCHAR)0x05 };
UCHAR *TakeOff = new UCHAR[3]{ (UCHAR)DataType::Command, (UCHAR)PetroneCommandType::FlightEvent, (UCHAR)FlightEvent::TakeOff };
UCHAR *Landing = new UCHAR[3]{ (UCHAR)DataType::Command, (UCHAR)PetroneCommandType::FlightEvent, (UCHAR)FlightEvent::Landing };
UCHAR *EmergencyLanding = new UCHAR[3]{ (UCHAR)DataType::Command, (UCHAR)PetroneCommandType::FlightEvent, (UCHAR)FlightEvent::Stop };
UCHAR *Altitude = new UCHAR[3]{ (UCHAR)DataType::Command, (UCHAR)PetroneCommandType::Request, (UCHAR)0x57 };
UCHAR *Battery = new UCHAR[3]{ (UCHAR)DataType::Command, (UCHAR)PetroneCommandType::Request, (UCHAR)0x31 };
UCHAR *GyroBias = new UCHAR[3]{ (UCHAR)DataType::Command, (UCHAR)PetroneCommandType::Request, (UCHAR)0x33 };
UCHAR *Gyro = new UCHAR[3]{ (UCHAR)DataType::Command, (UCHAR)PetroneCommandType::Request, (UCHAR)0x50 };
UCHAR *Pressure = new UCHAR[3]{ (UCHAR)DataType::Command, (UCHAR)PetroneCommandType::Request, (UCHAR)0x51 };
UCHAR *OpticalFlow = new UCHAR[3]{ (UCHAR)DataType::Command, (UCHAR)PetroneCommandType::Request, (UCHAR)0x52 };
UCHAR *Battery2 = new UCHAR[3]{ (UCHAR)DataType::Command, (UCHAR)PetroneCommandType::Request, (UCHAR)0x54 };
#pragma endregion

void delay(clock_t n);
int8_t S8BitConvert(UCHAR * src, int index);	// Converts bit for Signed 8bit data
int16_t S16BitConvert(UCHAR * src, int index);	// Converts bit for Signed 16bit data
int32_t S32BitConvert(UCHAR * src, int index);	// Converts bit for Signed 32bit data
uint8_t U8BitConvert(UCHAR * src, int index);	// Converts bit for Unsigned 8bit data
uint16_t U16BitConvert(UCHAR * src, int index);	// Converts bit for Unsigned 16bit data
uint32_t U32BitConvert(UCHAR * src, int index);	// Converts bit for Unsigned 32bit data
void Event_Handler(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context);
HANDLE GetBLEHandle(__in GUID AGuid);
void ScanPetrone();
void Send_Command(UCHAR *cmd);
int main();
