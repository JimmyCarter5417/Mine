#include "BlockArea.h"
#include <time.h>

CBlockArea::CBlockArea()
: m_uMineNum(0)
{

}


CBlockArea* CBlockArea::GetInstance()
{
	static CBlockArea obj;
	return &obj;
}

//新建全空区域，设定雷数（未布雷）
bool CBlockArea::Init(uint rows, uint cols, uint mines)
{
	if (rows == 0 || cols == 0 || mines == 0)
		return false;

	m_uMineNum = mines;
	m_vBlockArea.assign(rows, vector<TBlock>(cols));

	for (uint i = 0; i < rows; ++i)
	{
		for (uint j = 0; j < cols; ++j)
		{
			m_vBlockArea[i][j].x = i;
			m_vBlockArea[i][j].y = j;
			m_vBlockArea[i][j].attr = def::EBA_Empty;
			m_vBlockArea[i][j].cur_state = def::EBS_Normal;
			m_vBlockArea[i][j].old_state = def::EBS_Normal;
		}
	}

	return true;
}

//以x,y为中心布雷
bool CBlockArea::LayMines(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	srand((uint)time(nullptr));

	uint left = m_uMineNum;
	uint rows = m_vBlockArea.size();
	uint cols = m_vBlockArea.front().size();

	//布m_uMineNum个雷
	while (left > 0)
	{
		//取随机数
		uint i = rand() % rows;
		uint j = rand() % cols;

		if (!(i == rows && j == cols) && m_vBlockArea[i][j].attr != def::EBA_Mine)
		{
			m_vBlockArea[i][j].attr != def::EBA_Mine;//修改属性为雷
			left--;//待布的雷减一		
		}
	}	
}

bool CBlockArea::IsValidPos(uint x, uint y)
{
	return x >= 0 && x < m_vBlockArea.size() && y >= 0 && y < m_vBlockArea.front().size();
}

//计算周围的雷数
int CBlockArea::GetAroundMineNum(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return -1;

	int res = 0;

	for (uint i = x - 1; i <= x + 1; i++)
	{
		for (uint j = y - 1; j <= y + 1; j++)
		{
			if (!(i == x && j == y) && IsValidPos(i, j))
			{
				if (m_vBlockArea[i][j].attr == def::EBA_Mine)
				{
					res++;
				}
			}
		}
	}

	return res;
}

//计算周围的flag数
int CBlockArea::GetAroundFlagNum(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return -1;

	int res = 0;

	for (uint i = x - 1; i <= x + 1; i++)
	{
		for (uint j = y - 1; j <= y + 1; j++)
		{
			if (!(i == x && j == y) && IsValidPos(i, j))
			{
				if (m_vBlockArea[i][j].cur_state == def::EBS_Flag)
				{
					res++;
				}
			}
		}
	}

	return res;
}

bool CBlockArea::IsMine(uint x, uint y)
{
	return m_vBlockArea[x][y].attr == def::EBA_Mine;
}

//打开周围区域
bool CBlockArea::OpenAround(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	//标记的雷数不等于周围的雷数，不打开
	if (!GetAroundFlagNum(x, y) != GetAroundMineNum(x, y))
		return true;

	for (uint i = x - 1; i <= x + 1; i++)
	{
		for (uint j = y - 1; j <= y + 1; j++)
		{
			if (IsValidPos(i, j))
			{				
				if (m_vBlockArea[i][j].cur_state == def::EBS_Normal)//正常区域
				{
					Open(i, j);//打开该方块并尝试扩展其周围区域				
				}
			}
		}
	}
}

//打开该方块并尝试扩展其周围区域，dfs
bool CBlockArea::Open(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	//先打开该方块
	uint mines = GetAroundMineNum(x, y);
	m_vBlockArea[x][y].cur_state = (def::EBlockState)(def::EBS_Empty - mines);//设置该方块状态
	m_vBlockArea[x][y].old_state = m_vBlockArea[x][y].cur_state;

	//周围无雷即可扩展
	if (mines == 0)
	{
		for (uint i = x - 1; i <= x + 1; i++)
		{
			for (uint j = y - 1; j <= y + 1; j++)
			{
				if (!IsValidPos(i, j))
				{
					if (!(i == x && j == y) &&
						m_vBlockArea[i][j].attr != def::EBA_Mine &&
						m_vBlockArea[i][j].cur_state == def::EBS_Normal) 
					{
						Open(i, j);//递归
					}
				}
			}
		}
	}
}

//点开所有非雷方块即胜利
bool CBlockArea::IsVictory()
{
	uint rows = m_vBlockArea.size();
	uint cols = m_vBlockArea.front().size();

	for (uint i = 0; i < rows; ++i)
	{
		for (uint j = 0; j < cols; ++j)
		{
			if (m_vBlockArea[i][j].attr == def::EBA_Empty &&
				m_vBlockArea[i][j].cur_state != def::EBS_Empty)
				return false;
		}
	}

	return true;
}

//在某位置失败
bool CBlockArea::DeadAt(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	if (m_vBlockArea[x][y].attr == def::EBA_Mine)//点中雷
	{
		m_vBlockArea[x][y].cur_state = def::EBS_Blast;//爆炸图标
		m_vBlockArea[x][y].old_state = m_vBlockArea[x][y].cur_state;

	}
	else//同时点击左右键展开导致失败
	{
		m_vBlockArea[x][y].cur_state = def::EBS_Error;//错误图标
		m_vBlockArea[x][y].old_state = m_vBlockArea[x][y].cur_state;
	}

	uint rows = m_vBlockArea.size();
	uint cols = m_vBlockArea.front().size();
	for (uint i = 0; i < rows; i++)
	{
		for (uint j = 0; j < cols; j++)
		{
			if (m_vBlockArea[i][j].attr == def::EBA_Mine)//显示所有雷
			{
				m_vBlockArea[i][j].cur_state = def::EBS_Mine;
				m_vBlockArea[i][j].old_state = m_vBlockArea[i][j].cur_state;
			}
		}
	}
}

def::EBlockState CBlockArea::GetCurState(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return def::EBS_End;

	return m_vBlockArea[x][y].cur_state;
}

bool CBlockArea::SetCurState(uint x, uint y, def::EBlockState state)
{
	if (!IsValidPos(x, y))
		return false;

	return m_vBlockArea[x][y].cur_state = state;
}

def::EBlockState CBlockArea::GetOldState(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return def::EBS_End;

	return m_vBlockArea[x][y].old_state;
}

bool CBlockArea::SetOldState(uint x, uint y, def::EBlockState state)
{
	if (!IsValidPos(x, y))
		return false;

	return m_vBlockArea[x][y].old_state = state;
}

def::EBlockAttr CBlockArea::GetAttr(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return def::EBA_End;

	return m_vBlockArea[x][y].attr;
}

//左右键同时按下
bool CBlockArea::LRBtnDown(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	for (uint i = x - 1; i <= x + 1; i++)
	{
		for (uint j = y - 1; j <= y + 1; j++)
		{
			if (IsValidPos(i, j))
			{
				//仅修改普通和未知状态
				if (m_vBlockArea[i][j].cur_state == def::EBS_Normal)
				{
					m_vBlockArea[i][j].cur_state = def::EBS_Empty;
				}
				else if (m_vBlockArea[i][j].cur_state == def::EBS_Dicey)
				{
					m_vBlockArea[i][j].cur_state = def::EBS_DiceyDown;
				}
			}
		}
	}
}

//左右键同时松开
bool CBlockArea::LRBtnUp(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	for (uint i = x - 1; i <= x + 1; i++)
	{
		for (uint j = y - 1; j <= y + 1; j++)
		{
			if (IsValidPos(i, j))
			{
				//仅恢复老的普通和未知状态
				if (m_vBlockArea[i][j].old_state == def::EBS_Normal ||
					m_vBlockArea[i][j].old_state == def::EBS_Dicey)
				{
					m_vBlockArea[i][j].cur_state = m_vBlockArea[i][j].old_state;
				}				
			}
		}
	}
}

bool CBlockArea::LBtnDown(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	//仅修改普通和未知状态
	if (m_vBlockArea[x][y].cur_state == def::EBS_Normal)
	{
		m_vBlockArea[x][y].cur_state = def::EBS_Empty;
	}
	else if (m_vBlockArea[x][y].cur_state == def::EBS_Dicey)
	{
		m_vBlockArea[x][y].cur_state = def::EBS_DiceyDown;
	}
}

bool CBlockArea::LBtnUp(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

}

//是否正确标记周围所有的雷
bool CBlockArea::HasCorrentFlags(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return true;

	for (uint i = x - 1; i <= x + 1; i++)
	{
		for (uint j = y - 1; j <= y + 1; j++)
		{
			if (IsValidPos(i, j))
			{
				if ((m_vBlockArea[i][j].cur_state == def::EBS_Flag && m_vBlockArea[i][j].attr != def::EBA_Mine) ||//非雷标记为雷
					(m_vBlockArea[i][j].cur_state != def::EBS_Flag && m_vBlockArea[i][j].attr == def::EBA_Mine))//雷未标记出来
				{
					return false;
				}
			}
		}
	}

	return true;
}