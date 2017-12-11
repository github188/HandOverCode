#ifndef STRUCT_H_H_Z
#define STRUCT_H_H_Z

#define WM_ICONMESSAGE WM_USER + 250
#define ID_MENU_EXIT   WM_USER + 251
#define ID_MENU_SHOW   WM_USER + 252
#define UM_HKLASTERROR WM_USER + 253
#define UM_SOCKUDPZ	   WM_USER + 254
#define UM_JGPTDATA	   WM_USER + 256
#define UM_SOCKETZ	   WM_USER + 257

typedef struct _terminalpz
{ 
	char TermainlBH[10];	//���
	char TerminalIP[16];	//�豸IP
	char RasUser[16];		//�û���
	char RasPassword[16];	//����
	WORD TerminalPort;		//�˿ں�
	WORD dwChannel;		//ͨ����
	char mediaIP[16];//��ý��IP
	int  imllx;//�������� 0 ������ 1 ������
	_terminalpz()
	{
		TermainlBH[0] ='\0';
		TerminalIP[0] ='\0';
		RasUser[0]	  ='\0';
		RasPassword[0]='\0';
		TerminalPort  =8000;
		dwChannel     =-1;
		mediaIP[0]='\0';
		imllx=1;
	}
}STerminalPZ;

typedef struct _errordata
{
	char errorlx[31];
	int ikcfs;
}ERRORDATA;

typedef enum PACKTYPE{ 
	SOCKZREEOR,
	GNSSDATA,
	JGPTDATA,
	JMQStart,
	QueryStart,
	AllCarNum,
	M17C51,
	M17C52,
	M17C53,
	M17C54,
	M17C55,
	M17C56
};

// typedef struct _liststru
// {
// 	int ikch;//������
// 	int itype;//״̬
// 	int irunnum;
// 	int ics;//����
// 	CString zkzmbh;
// 	CString msg;
// }JGPTLISTSTRU;

#endif //STRUCT_H_H_Z 