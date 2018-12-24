#ifndef __CFG_MGR_H__
#define __CFG_MGR_H__

using namespace def;

class CCfgMgr
{
public:
    static CCfgMgr* GetInstance();  

    void GetMineInfo(uint& row, uint& col, uint& mine, LEVEL_E& level);
    void SetMineInfo(uint row, uint col, uint mine, LEVEL_E level);

    bool GetSoundful();
    bool GetColorful();

    void SetSoundful(bool soundful);
    void SetColorful(bool colorful);

    uint GetPrimaryRecord();
    uint GetMediumRecord();
    uint GetAdvancedRecord();

    CString GetPrimaryHolder(); 
    CString GetMediumHolder();
    CString GetAdvancedHolder();

    void SetPrimaryRecord(uint record);
    void SetMediumRecord(uint record);
    void SetAdvancedRecord(uint record);

    void SetPrimaryHolder(const CString& holder);
    void SetMediumHolder(const CString& holder);
    void SetAdvancedHolder(const CString& holder);

private:
    explicit CCfgMgr(const CString& strCfgName);

private:
    CString m_strCfgPath;
};

#endif // __CFG_MGR_H__