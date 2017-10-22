#ifndef __DEF_H__
#define __DEF_H__

#include <windef.h>

namespace def
{
	typedef unsigned int uint;

	//��Ϸ�ȼ�
	enum ELevel
	{
		ELevel_Primary,
		ELevel_Medium,
		ELevel_Advanced,
		ELevel_Custom
	};

	//��Ϸ״̬
	enum EGameState
	{
		EGS_Wait,
		EGS_Run,
		EGS_Dead,
		EGS_Victory
	};

	//��������
	enum EBlockAttr
	{
		EBA_Empty,
		EBA_Mine
	};

	//����״̬
	enum EBlockState
	{
		EBS_Normal,//����
		EBS_Flag,//��־Ϊ��
		EBS_Dicey,//δ֪״̬0
		EBS_Blast,//��ը״̬
		EBS_Error,//����״̬
		EBS_Mine,//��״̬
		EBS_DiceyDown,//δ֪״̬1
		EBS_Num8,
		EBS_Num7,
		EBS_Num6,
		EBS_Num5,
		EBS_Num4,
		EBS_Num3,
		EBS_Num2,
		EBS_Num1,		
		EBS_Empty,//����

		EBS_End//16
	};

	//��ť״̬
	enum EButtonState
	{
		EBtnS_Down,
		EBtnS_Victory,
		EBtnS_Dead,
		EBtnS_Click,
		EBtnS_Normal,
		
		EBtnS_End//5
	};

	//����
	enum ENum
	{
		ENum_Null,
		ENum_Empty,
		ENum_9,
		ENum_8,
		ENum_7,
		ENum_6,
		ENum_5,
		ENum_4,
		ENum_3,
		ENum_2,
		ENum_1,
		ENum_0,
		
		ENum_End//12
	};

	//���飬16 * 16
	//λͼ��16 * (16 * 16)
	static uint g_nBlockWidth = 16;
	static uint g_nBlockHeight = 16;
	//��ť��24 * 24
	//λͼ��24 * (24 * 5)
	static uint g_nBtnWidth = 24;
	static uint g_nBtnHeight = 24;
	//���֣�13 * 23
	//λͼ��13 * (23 * 12)
	static uint g_nNumWidth = 13;
	static uint g_nNumHeight = 23;

	//ƫ��������ͼƬ
	static uint g_nXOffset = 5;
	static uint g_nYOffset = 5;
	static uint g_nInnerXOffset = 5;
	static uint g_nInnerYOffset = 8;
	static uint g_nGap = 5;

	//���߶�
	static uint g_nPanelHeight = g_nInnerYOffset * 2 + g_nNumHeight;

	//�����ļ�Ĭ������
	static ELevel  g_DefLevel     = ELevel_Primary;
	static CString g_strDefHolder = TEXT("����");
	static uint    g_nDefRecord   = 999;
	static uint    g_nDefColorful = 1;
	static uint    g_nDefSoundful = 0;

	//���ȼ�Ĭ�ϲ���
	static uint g_nDefPrimaryXNum = 10;
	static uint g_nDefPrimaryYNum = 10;
	static uint g_nDefPrimaryMineNum = 10;

	static uint g_nDefMediumXNum = 16;
	static uint g_nDefMediumYNum = 16;
	static uint g_nDefMediumMineNum = 40;

	static uint g_nDefAdvancedXNum = 30;
	static uint g_nDefAdvancedYNum = 30;
	static uint g_nDefAdvancedMineNum = 99;

	//��ɫ����
	static COLORREF g_clrBlack = RGB(0, 0, 0);
	static COLORREF g_clrWhite = RGB(255, 255, 255);
	static COLORREF g_clrGray = RGB(192, 192, 192);
	static COLORREF g_clrDarkGray = RGB(128, 128, 128);

	//�����ļ�����
	static CString g_strCfgName = TEXT("Config.ini");
	//�����ļ�section��key
	static CString g_strMine = TEXT("MINE");
	static CString g_strXNum = TEXT("XNum");
	static CString g_strYNum = TEXT("YNum");
	static CString g_strMineNum = TEXT("MineNum");
	static CString g_strLevel = TEXT("Level");
	
	static CString g_strRecord = TEXT("Record");
	static CString g_strPrimary = TEXT("Primary");
	static CString g_strMedium = TEXT("Medium");
	static CString g_strAdvanced = TEXT("Advanced");

	static CString g_strHolder = TEXT("Holder");
	/*static CString g_strPrimary = TEXT("Primary");
	static CString g_strMedium = TEXT("Medium");
	static CString g_strAdvanced = TEXT("Advanced");*/

	static CString g_strConfig = TEXT("Config");
	static CString g_strColorful = TEXT("Colorful");
	static CString g_strSoundful = TEXT("Soundful");
}

#endif//__DEF_H__