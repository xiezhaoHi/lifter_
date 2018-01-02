
#ifndef CONTROLCAN
#define CONTROLCAN
//�ӿڿ����Ͷ���
#define VCI_PCI5121		1
#define VCI_PCI9810		2
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_PCI9820		5
#define VCI_CAN232		6
#define VCI_PCI5110		7
#define VCI_CANLITE		8
#define VCI_ISA9620		9
#define VCI_ISA5420		10
#define VCI_PC104CAN	11
#define VCI_CANETUDP	12
#define VCI_CANETTCP	17

//CAN������
#define	ERR_CAN_OVERFLOW			0x0001	//CAN�������ڲ�FIFO���
#define	ERR_CAN_ERRALARM			0x0002	//CAN���������󱨾�
#define	ERR_CAN_PASSIVE				0x0004	//CAN��������������
#define	ERR_CAN_LOSE				0x0008	//CAN�������ٲö�ʧ
#define	ERR_CAN_BUSERR				0x0010	//CAN���������ߴ���

//ͨ�ô�����
#define	ERR_DEVICEOPENED			0x0100	//�豸�Ѿ���
#define	ERR_DEVICEOPEN				0x0200	//���豸����
#define	ERR_DEVICENOTOPEN			0x0400	//�豸û�д�
#define	ERR_BUFFEROVERFLOW			0x0800	//���������
#define	ERR_DEVICENOTEXIST			0x1000	//���豸������
#define	ERR_LOADKERNELDLL			0x2000	//װ�ض�̬��ʧ��
#define ERR_CMDFAILED				0x4000	//ִ������ʧ�ܴ�����
#define	ERR_BUFFERCREATE			0x8000	//�ڴ治��
//��¼�������Ӧ�Ĵ���

 

//�������÷���״ֵ̬
#define	STATUS_OK					1
#define STATUS_ERR					0
	
#define CMD_DESIP				0
#define CMD_DESPORT				1
#define CMD_CHGDESIPANDPORT		2

typedef unsigned short USHORT;
typedef unsigned char       BYTE;
typedef char CHAR;
typedef unsigned int        UINT;
typedef unsigned char UCHAR;
typedef unsigned long       DWORD;
typedef unsigned long ULONG;
typedef int                 INT;
typedef void *PVOID;
//1.ZLGCANϵ�нӿڿ���Ϣ���������͡�
typedef  struct  _VCI_BOARD_INFO{
		USHORT	hw_Version;
		USHORT	fw_Version;
		USHORT	dr_Version;
		USHORT	in_Version;
		USHORT	irq_Num;
		BYTE	can_Num;
		CHAR	str_Serial_Num[20];
		CHAR	str_hw_Type[40];
		USHORT	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; 

//2.����CAN��Ϣ֡���������͡�
typedef  struct  _VCI_CAN_OBJ{
	UINT	ID;
	UINT	TimeStamp;
	BYTE	TimeFlag;
	BYTE	SendType;
	BYTE	RemoteFlag;//�Ƿ���Զ��֡
	BYTE	ExternFlag;//�Ƿ�����չ֡
	BYTE	DataLen;
	BYTE	Data[8];
	BYTE	Reserved[3];
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;
typedef enum SET_CMD
{
    SET_CH_EN = 0xf0,
    SET_ADJUST,
    SET_K,
    SET_RATE,
    SET_GAIN,
    SET_IMAG,
    SET_BRAGE,
    SET_TIME,
    SET_ID,
    SET_CLEAR
}SET_CMD_T;
//3.����CAN������״̬���������͡�
typedef struct _VCI_CAN_STATUS{
	UCHAR	ErrInterrupt;
	UCHAR	regMode;
	UCHAR	regStatus;
	UCHAR	regALCapture;
	UCHAR	regECCapture; 
	UCHAR	regEWLimit;
	UCHAR	regRECounter; 
	UCHAR	regTECounter;
	DWORD	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.���������Ϣ���������͡�
typedef struct _ERR_INFO{
		UINT	ErrCode;
		BYTE	Passive_ErrData[3];
		BYTE	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.�����ʼ��CAN����������
typedef struct _INIT_CONFIG{
	DWORD	AccCode;
	DWORD	AccMask;
	DWORD	Reserved;
	UCHAR	Filter;
	UCHAR	Timing0;	
	UCHAR	Timing1;	
	UCHAR	Mode;
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

typedef struct _tagChgDesIPAndPort
{
	char szpwd[10];
	char szdesip[20];
	int desport;
	BYTE blistenonly;
}CHGDESIPANDPORT;

//__stdcall
extern "C"
{
	typedef DWORD(__stdcall *VCI_OpenDevice)(DWORD DeviceType, DWORD DeviceInd, DWORD Reserved);
	typedef DWORD(__stdcall  *VCI_CloseDevice)(DWORD DeviceType, DWORD DeviceInd);
	typedef DWORD(__stdcall  *VCI_InitCAN)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_INIT_CONFIG pInitConfig);

	typedef DWORD(__stdcall *VCI_ReadBoardInfo)(DWORD DeviceType, DWORD DeviceInd, PVCI_BOARD_INFO pInfo);
	typedef DWORD(__stdcall  *VCI_ReadErrInfo)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_ERR_INFO pErrInfo);
	typedef DWORD(__stdcall  *VCI_ReadCANStatus)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_CAN_STATUS pCANStatus);

	typedef DWORD(__stdcall  *VCI_GetReference)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, DWORD RefType, PVOID pData);
	typedef DWORD(__stdcall *VCI_SetReference)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, DWORD RefType, PVOID pData);

	typedef ULONG(__stdcall  *VCI_GetReceiveNum)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);
	typedef DWORD(__stdcall  *VCI_ClearBuffer)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);

	typedef DWORD(__stdcall  *VCI_StartCAN)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);
	typedef DWORD(__stdcall  *VCI_ResetCAN)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);

	typedef ULONG(__stdcall  *VCI_Transmit)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_CAN_OBJ pSend, ULONG Len);
	//=-1
	typedef ULONG(__stdcall  *VCI_Receive)(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_CAN_OBJ pReceive, ULONG Len, INT WaitTime); 
}
#endif
