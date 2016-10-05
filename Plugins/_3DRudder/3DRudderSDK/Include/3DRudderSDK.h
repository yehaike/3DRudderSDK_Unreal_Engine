#pragma once
#if defined(_WINDOWS) && ! defined ( _3DRUDDER_SDK_STATIC) 
	#ifdef _USRDLL
		#define _3DRUDDER_SDK_EXPORT __declspec(dllexport)
	#else
		#define _3DRUDDER_SDK_EXPORT __declspec(dllimport)
	#endif
#else
	#define _3DRUDDER_SDK_EXPORT
#endif

#define  _3DRUDDER_SDK_MAX_DEVICE 4
#define  _3DRUDDER_SDK_VERSION 0x0040

namespace ns3DRudder
{
	enum ErrorCode
	{
		Success = 0, NotConnected, Fail, IncorrectCommand, Timeout, WrongSignature,NotReady, Other = 0xFF
	};

	class State
	{
	public:
		enum Status
		{
			NoStatus=0,NoFootStayStill = 1, Initialisation, PutYourFeet, PutSecondFoot, StayStill, InUse, ExtendedMode, End = 0xffff
		};

		State()
		{
			aX = aY = aZ = rZ = 0;
			status = NoStatus;
			s0=s1=s2=s3=s4=s5 = 0;
		}

		int aX;
		int aY;
		int aZ;
		int rZ;
		Status status;
		unsigned short s0,s1,s2,s3,s4,s5;

		int GetXAxis();
		int GetYAxis();
		int GetZAxis();
		int GetZRotation();
		Status GetStatus();
		unsigned short GetSensor(int nIndex);

	};

	class CSdk
	{
	public:
		CSdk();

		virtual unsigned short GetSDKVersion();
		virtual int GetNumberOfConnectedDevice();
		virtual bool IsDeviceConnected(int nPortNumber);
		virtual unsigned short GetFirmwareVersion(int nPortNumber);
		virtual ErrorCode PlaySnd(int nPortNumber, short int nFrequency, short int nDuration);
		virtual ErrorCode Get3DRudderState(int nPortNumber, State *pState);

	};

	_3DRUDDER_SDK_EXPORT CSdk* GetSDK();
}
