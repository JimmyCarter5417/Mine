#ifndef __BLOCK_AREA_H__
#define __BLOCK_AREA_H__

#include <vector>

using std::vector;
using namespace def;

class CBlockArea
{
public:
    static CBlockArea* GetInstance();

    bool Init(uint rows, uint cols, uint mines);
    bool LayMines(TPos pos);
    bool IsValidPos(TPos pos);
    bool IsMine(TPos pos);
    bool OpenAround(TPos pos);
    bool Open(TPos pos);
    bool IsVictory();
    bool DeadAt(TPos pos);
    bool HasCorrectFlags(TPos pos);
    bool ShowMines(bool show);

    bool LBtnDown(TPos pos);
    bool LBtnUp(TPos pos);
    bool LRBtnDown(TPos pos);
    bool LRBtnUp(TPos pos);
    
    BLOCK_STATE_E GetCurState(TPos pos);
    bool SetCurState(TPos pos, BLOCK_STATE_E state);
    
    BLOCK_STATE_E GetOldState(TPos pos);
    bool SetOldState(TPos pos, BLOCK_STATE_E state);

    BLOCK_ATTR_E GetAttr(TPos pos);

    int GetAroundMineNum(TPos pos);
    int GetAroundFlagNum(TPos pos);

private:
    CBlockArea();

    //删除拷贝与赋值
    CBlockArea(const CBlockArea&) = delete;
    CBlockArea& operator=(const CBlockArea&) = delete;

private:
    vector<vector<TBlock>> m_vBlockArea;
    uint                   m_uMineNum;
};

#endif // __BLOCK_AREA_H__