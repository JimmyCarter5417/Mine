#include "StdAfx.h"
#include <string>
#include "Def.h"
#include "CfgMgr.h"

using namespace std;

CCfgMgr* CCfgMgr::GetInstance()
{
    static CCfgMgr mgr(g_strCfgName);
    return &mgr;
}

CCfgMgr::CCfgMgr(const CString& strCfgName)
{
    TCHAR szExePath[MAX_PATH];
    GetModuleFileName(nullptr, szExePath, MAX_PATH);

    m_strCfgPath = szExePath;
    m_strCfgPath = m_strCfgPath.Left(m_strCfgPath.ReverseFind('\\') + 1);
    m_strCfgPath += g_strCfgName;
}

void CCfgMgr::GetMineInfo(uint& row, uint& col, uint& mine, LEVEL_E& level)
{
    row   = GetPrivateProfileInt(g_strMine, g_strRowNum,  g_nDefPrimaryRowNum,  m_strCfgPath);
    col   = GetPrivateProfileInt(g_strMine, g_strColNum,  g_nDefPrimaryColNum,  m_strCfgPath);
    mine  = GetPrivateProfileInt(g_strMine, g_strMineNum, g_nDefPrimaryMineNum, m_strCfgPath);
    level = (LEVEL_E)GetPrivateProfileInt(g_strMine, g_strLevel, g_eDefLevel, m_strCfgPath);
}
void CCfgMgr::SetMineInfo(uint row, uint col, uint mine, LEVEL_E level)
{
    WritePrivateProfileString(g_strMine, g_strRowNum,  to_wstring(row).c_str(),  m_strCfgPath);
    WritePrivateProfileString(g_strMine, g_strColNum,  to_wstring(col).c_str(),  m_strCfgPath);
    WritePrivateProfileString(g_strMine, g_strMineNum, to_wstring(mine).c_str(), m_strCfgPath);
    WritePrivateProfileString(g_strMine, g_strLevel,   to_wstring(level).c_str(), m_strCfgPath);
}

bool CCfgMgr::GetSoundful()
{
    return GetPrivateProfileInt(g_strConfig, g_strSoundful, g_nDefSoundful, m_strCfgPath) != 0;
}

bool CCfgMgr::GetColorful()
{
    return GetPrivateProfileInt(g_strConfig, g_strColorful, g_nDefColorful, m_strCfgPath) != 0;
}

void CCfgMgr::SetSoundful(bool soundful)
{
    WritePrivateProfileString(g_strConfig, g_strSoundful, soundful ? TEXT("1") : TEXT("0") , m_strCfgPath);
}

void CCfgMgr::SetColorful(bool colorful)
{
    WritePrivateProfileString(g_strConfig, g_strColorful, colorful ? TEXT("1") : TEXT("0"), m_strCfgPath);
}

uint CCfgMgr::GetPrimaryRecord()
{
    return GetPrivateProfileInt(g_strRecord, g_strPrimary, g_nDefRecord, m_strCfgPath);
}

uint CCfgMgr::GetMediumRecord()
{
    return GetPrivateProfileInt(g_strRecord, g_strMedium, g_nDefRecord, m_strCfgPath);
}

uint CCfgMgr::GetAdvancedRecord()
{
    return GetPrivateProfileInt(g_strRecord, g_strAdvanced, g_nDefRecord, m_strCfgPath);
}

CString CCfgMgr::GetPrimaryHolder()
{
    TCHAR buf[128] = { 0 };
    GetPrivateProfileString(g_strHolder, g_strPrimary, g_strDefHolder, buf, 128, m_strCfgPath);
    return buf;
}

CString CCfgMgr::GetMediumHolder()
{
    TCHAR buf[128] = { 0 };
    GetPrivateProfileString(g_strHolder, g_strMedium, g_strDefHolder, buf, 128, m_strCfgPath);
    return buf;
}

CString CCfgMgr::GetAdvancedHolder()
{
    TCHAR buf[128] = { 0 };
    GetPrivateProfileString(g_strHolder, g_strAdvanced, g_strDefHolder, buf, 128, m_strCfgPath);
    return buf;
}

void CCfgMgr::SetPrimaryRecord(uint record)
{
    WritePrivateProfileString(g_strRecord, g_strPrimary, to_wstring(record).c_str(), m_strCfgPath);
}

void CCfgMgr::SetMediumRecord(uint record)
{
    WritePrivateProfileString(g_strRecord, g_strMedium, to_wstring(record).c_str(), m_strCfgPath);
}

void CCfgMgr::SetAdvancedRecord(uint record)
{
    WritePrivateProfileString(g_strRecord, g_strAdvanced, to_wstring(record).c_str(), m_strCfgPath);
}

void CCfgMgr::SetPrimaryHolder(const CString& holder)
{
    WritePrivateProfileString(g_strHolder, g_strPrimary, holder, m_strCfgPath);
}

void CCfgMgr::SetMediumHolder(const CString& holder)
{
    WritePrivateProfileString(g_strHolder, g_strMedium, holder, m_strCfgPath);
}

void CCfgMgr::SetAdvancedHolder(const CString& holder)
{
    WritePrivateProfileString(g_strHolder, g_strAdvanced, holder, m_strCfgPath);
}