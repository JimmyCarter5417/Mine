#include "StdAfx.h"
#include <time.h>
#include "Def.h"
#include "BlockArea.h"

CBlockArea::CBlockArea()
: m_uMineNum(0)
{

}

CBlockArea* CBlockArea::GetInstance()
{
    static CBlockArea obj;
    return &obj;
}

// 新建全空区域，设定雷数（未布雷）
bool CBlockArea::Init(uint rows, uint cols, uint mines)
{
    if (rows == 0 || cols == 0 || mines == 0)
    {
        return false;
    }

    m_uMineNum = mines;
    m_vBlockArea.assign(rows, vector<TBlock>(cols));

    for (uint i = 0; i < rows; ++i)
    {
        for (uint j = 0; j < cols; ++j)
        {
            m_vBlockArea[i][j].pos.x = i;
            m_vBlockArea[i][j].pos.y = j;
            m_vBlockArea[i][j].attr      = BLOCK_ATTR_Empty;
            m_vBlockArea[i][j].cur_state = BLOCK_STATE_Normal;
            m_vBlockArea[i][j].old_state = BLOCK_STATE_Normal;
        }
    }

    return true;
}

// 以x,y为中心布雷
bool CBlockArea::LayMines(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return false;
    }

    srand((uint)time(nullptr));

    uint left = m_uMineNum;
    uint rows = m_vBlockArea.size();
    uint cols = m_vBlockArea.front().size();

    // 布m_uMineNum个雷
    while (left > 0)
    {
        // 取随机数
        uint i = rand() % rows;
        uint j = rand() % cols;

        if (!(i == pos.x && j == pos.y) && 
            (m_vBlockArea[i][j].attr != BLOCK_ATTR_Mine))
        {
            m_vBlockArea[i][j].attr = BLOCK_ATTR_Mine; // 修改属性为雷
            left--; // 待布的雷减一     
        }
    }   

    return true;
}

bool CBlockArea::IsValidPos(TPos pos)
{
    return (pos.x >= 0) && 
           (pos.x < m_vBlockArea.size()) && 
           (pos.y >= 0) && 
           (pos.y < m_vBlockArea.front().size());
}

// 计算周围的雷数
int CBlockArea::GetAroundMineNum(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return -1;
    }

    int res = 0;

    for (uint i = pos.x - 1; i <= pos.x + 1; i++)
    {
        for (uint j = pos.y - 1; j <= pos.y + 1; j++)
        {
            if (!(i == pos.x && j == pos.y) && IsValidPos({ i, j }))
            {
                if (m_vBlockArea[i][j].attr == BLOCK_ATTR_Mine)
                {
                    res++;
                }
            }
        }
    }

    return res;
}

// 计算周围的flag数
int CBlockArea::GetAroundFlagNum(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return -1;
    }

    int res = 0;

    for (uint i = pos.x - 1; i <= pos.x + 1; i++)
    {
        for (uint j = pos.y - 1; j <= pos.y + 1; j++)
        {
            if (!(i == pos.x && j == pos.y) && IsValidPos({ i, j }))
            {
                if (m_vBlockArea[i][j].cur_state == BLOCK_STATE_Flag)
                {
                    res++;
                }
            }
        }
    }

    return res;
}

bool CBlockArea::IsMine(TPos pos)
{
    return m_vBlockArea[pos.x][pos.y].attr == BLOCK_ATTR_Mine;
}

// 打开周围区域
bool CBlockArea::OpenAround(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return false;
    }

    // 标记的雷数不等于周围的雷数，不打开
    if (GetAroundFlagNum(pos) != GetAroundMineNum(pos))
    {
        return true;
    }

    for (uint i = pos.x - 1; i <= pos.x + 1; i++)
    {
        for (uint j = pos.y - 1; j <= pos.y + 1; j++)
        {
            if (IsValidPos({ i, j }))
            {               
                if (m_vBlockArea[i][j].cur_state == BLOCK_STATE_Normal) // 正常区域
                {
                    Open({ i, j }); // 打开该方块并尝试扩展其周围区域
                }
            }
        }
    }

    return true;
}

// 打开该方块并尝试扩展其周围区域，dfs
bool CBlockArea::Open(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return false;
    }

    // 先打开该方块
    uint mines = GetAroundMineNum(pos);
    m_vBlockArea[pos.x][pos.y].cur_state = (BLOCK_STATE_E)(BLOCK_STATE_Empty - mines); // 设置该方块状态
    m_vBlockArea[pos.x][pos.y].old_state = m_vBlockArea[pos.x][pos.y].cur_state;

    //周围无雷即可扩展
    if (mines == 0)
    {
        for (uint i = pos.x - 1; i <= pos.x + 1; i++)
        {
            for (uint j = pos.y - 1; j <= pos.y + 1; j++)
            {
                if (IsValidPos({ i, j }))
                {
                    if (!(i == pos.x && j == pos.y) &&
                        (m_vBlockArea[i][j].attr != BLOCK_ATTR_Mine) &&
                        (m_vBlockArea[i][j].cur_state == BLOCK_STATE_Normal)) 
                    {
                        Open({ i, j }); // 递归
                    }
                }
            }
        }
    }

    return true;
}

// 点开所有非雷方块即胜利
bool CBlockArea::IsVictory()
{
    uint rows = m_vBlockArea.size();
    uint cols = m_vBlockArea.front().size();

    for (uint i = 0; i < rows; ++i)
    {
        for (uint j = 0; j < cols; ++j)
        {
            if (m_vBlockArea[i][j].attr == BLOCK_ATTR_Empty &&   // 空格
                m_vBlockArea[i][j].cur_state < BLOCK_STATE_Num8) // 未打开
                return false;
        }
    }

    return true;
}

// 在某位置失败
bool CBlockArea::DeadAt(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return false;
    }

    uint rows = m_vBlockArea.size();
    uint cols = m_vBlockArea.front().size();
    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j++)
        {
            if (m_vBlockArea[i][j].attr == BLOCK_ATTR_Mine) // 显示所有雷
            {
                m_vBlockArea[i][j].cur_state = BLOCK_STATE_Mine;
                m_vBlockArea[i][j].old_state = m_vBlockArea[i][j].cur_state;
            }
        }
    }

    if (m_vBlockArea[pos.x][pos.y].attr == BLOCK_ATTR_Mine) // 点中雷
    {
        m_vBlockArea[pos.x][pos.y].cur_state = BLOCK_STATE_Blast; // 爆炸图标
        m_vBlockArea[pos.x][pos.y].old_state = m_vBlockArea[pos.x][pos.y].cur_state;

    }
    else // 同时点击左右键展开导致失败
    {
        m_vBlockArea[pos.x][pos.y].cur_state = BLOCK_STATE_Error; // 错误图标
        m_vBlockArea[pos.x][pos.y].old_state = m_vBlockArea[pos.x][pos.y].cur_state;
    }

    return true;
}

BLOCK_STATE_E CBlockArea::GetCurState(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return BLOCK_STATE_End;
    }

    return m_vBlockArea[pos.x][pos.y].cur_state;
}

bool CBlockArea::SetCurState(TPos pos, BLOCK_STATE_E state)
{
    if (!IsValidPos(pos))
    {
        return false;
    }

    m_vBlockArea[pos.x][pos.y].cur_state = state;

    return true;
}

BLOCK_STATE_E CBlockArea::GetOldState(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return BLOCK_STATE_End;
    }

    return m_vBlockArea[pos.x][pos.y].old_state;
}

bool CBlockArea::SetOldState(TPos pos, BLOCK_STATE_E state)
{
    if (!IsValidPos(pos))
    {
        return false;
    }

    m_vBlockArea[pos.x][pos.y].old_state = state;

    return true;
}

BLOCK_ATTR_E CBlockArea::GetAttr(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return BLOCK_ATTR_End;
    }

    return m_vBlockArea[pos.x][pos.y].attr;
}

// 左右键同时按下
bool CBlockArea::LRBtnDown(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return false;
    }

    for (uint i = pos.x - 1; i <= pos.x + 1; i++)
    {
        for (uint j = pos.y - 1; j <= pos.y + 1; j++)
        {
            if (IsValidPos({ i, j }))
            {
                // 仅修改普通和未知状态
                if (m_vBlockArea[i][j].cur_state == BLOCK_STATE_Normal)
                {
                    m_vBlockArea[i][j].cur_state = BLOCK_STATE_Empty;
                }
                else if (m_vBlockArea[i][j].cur_state == BLOCK_STATE_Dicey)
                {
                    m_vBlockArea[i][j].cur_state = BLOCK_STATE_DiceyDown;
                }
            }
        }
    }

    return true;
}

// 左右键同时松开
bool CBlockArea::LRBtnUp(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return false;
    }

    for (uint i = pos.x - 1; i <= pos.x + 1; i++)
    {
        for (uint j = pos.y - 1; j <= pos.y + 1; j++)
        {
            if (IsValidPos({ i, j }))
            {
                // 仅恢复老的普通和未知状态
                if ((m_vBlockArea[i][j].old_state == BLOCK_STATE_Normal) ||
                    (m_vBlockArea[i][j].old_state == BLOCK_STATE_Dicey))
                {
                    m_vBlockArea[i][j].cur_state = m_vBlockArea[i][j].old_state;
                }               
            }
        }
    }

    return true;
}

bool CBlockArea::LBtnDown(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return false;
    }

    // 仅修改普通和未知状态
    if (m_vBlockArea[pos.x][pos.y].cur_state == BLOCK_STATE_Normal)
    {
        m_vBlockArea[pos.x][pos.y].cur_state = BLOCK_STATE_Empty;
    }
    else if (m_vBlockArea[pos.x][pos.y].cur_state == BLOCK_STATE_Dicey)
    {
        m_vBlockArea[pos.x][pos.y].cur_state = BLOCK_STATE_DiceyDown;
    }

    return true;
}

bool CBlockArea::LBtnUp(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return false;
    }

    // 仅恢复老的普通和未知状态
    if (m_vBlockArea[pos.x][pos.y].old_state == BLOCK_STATE_Normal ||
        m_vBlockArea[pos.x][pos.y].old_state == BLOCK_STATE_Dicey)
    {
        m_vBlockArea[pos.x][pos.y].cur_state = m_vBlockArea[pos.x][pos.y].old_state;
    }

    return true;
}

// 是否正确标记周围所有的雷
bool CBlockArea::HasCorrectFlags(TPos pos)
{
    if (!IsValidPos(pos))
    {
        return true;
    }

    for (uint i = pos.x - 1; i <= pos.x + 1; i++)
    {
        for (uint j = pos.y - 1; j <= pos.y + 1; j++)
        {
            if (IsValidPos({ i, j }))
            {
                // 非雷标记为雷
                if ((m_vBlockArea[i][j].cur_state == BLOCK_STATE_Flag) && 
                    (m_vBlockArea[i][j].attr      != BLOCK_ATTR_Mine))
                {
                    return false;
                }
                // 雷未标记出来
                if ((m_vBlockArea[i][j].cur_state != BLOCK_STATE_Flag) && 
                    (m_vBlockArea[i][j].attr      == BLOCK_ATTR_Mine))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool CBlockArea::ShowMines(bool show)
{
    uint rows = m_vBlockArea.size();
    uint cols = m_vBlockArea.front().size();
    
    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j++)
        {
            if (m_vBlockArea[i][j].attr == BLOCK_ATTR_Mine)
            {
                if (show) // 显示
                {
                    m_vBlockArea[i][j].cur_state = BLOCK_STATE_Mine;
                    // m_vBlockArea[i][j].old_state = m_vBlockArea[i][j].cur_state;
                }
                else // 还原
                {
                    m_vBlockArea[i][j].cur_state = m_vBlockArea[i][j].old_state;
                }
            }
        }
    }

    return true;
}