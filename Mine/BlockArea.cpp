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

//�½�ȫ�������趨������δ���ף�
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

//��x,yΪ���Ĳ���
bool CBlockArea::LayMines(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	srand((uint)time(nullptr));

	uint left = m_uMineNum;
	uint rows = m_vBlockArea.size();
	uint cols = m_vBlockArea.front().size();

	//��m_uMineNum����
	while (left > 0)
	{
		//ȡ�����
		uint i = rand() % rows;
		uint j = rand() % cols;

		if (!(i == rows && j == cols) && m_vBlockArea[i][j].attr != def::EBA_Mine)
		{
			m_vBlockArea[i][j].attr != def::EBA_Mine;//�޸�����Ϊ��
			left--;//�������׼�һ		
		}
	}	
}

bool CBlockArea::IsValidPos(uint x, uint y)
{
	return x >= 0 && x < m_vBlockArea.size() && y >= 0 && y < m_vBlockArea.front().size();
}

//������Χ������
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

//������Χ��flag��
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

//����Χ����
bool CBlockArea::OpenAround(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	//��ǵ�������������Χ������������
	if (!GetAroundFlagNum(x, y) != GetAroundMineNum(x, y))
		return true;

	for (uint i = x - 1; i <= x + 1; i++)
	{
		for (uint j = y - 1; j <= y + 1; j++)
		{
			if (IsValidPos(i, j))
			{				
				if (m_vBlockArea[i][j].cur_state == def::EBS_Normal)//��������
				{
					Open(i, j);//�򿪸÷��鲢������չ����Χ����				
				}
			}
		}
	}
}

//�򿪸÷��鲢������չ����Χ����dfs
bool CBlockArea::Open(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	//�ȴ򿪸÷���
	uint mines = GetAroundMineNum(x, y);
	m_vBlockArea[x][y].cur_state = (def::EBlockState)(def::EBS_Empty - mines);//���ø÷���״̬
	m_vBlockArea[x][y].old_state = m_vBlockArea[x][y].cur_state;

	//��Χ���׼�����չ
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
						Open(i, j);//�ݹ�
					}
				}
			}
		}
	}
}

//�㿪���з��׷��鼴ʤ��
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

//��ĳλ��ʧ��
bool CBlockArea::DeadAt(uint x, uint y)
{
	if (!IsValidPos(x, y))
		return false;

	if (m_vBlockArea[x][y].attr == def::EBA_Mine)//������
	{
		m_vBlockArea[x][y].cur_state = def::EBS_Blast;//��ըͼ��
		m_vBlockArea[x][y].old_state = m_vBlockArea[x][y].cur_state;

	}
	else//ͬʱ������Ҽ�չ������ʧ��
	{
		m_vBlockArea[x][y].cur_state = def::EBS_Error;//����ͼ��
		m_vBlockArea[x][y].old_state = m_vBlockArea[x][y].cur_state;
	}

	uint rows = m_vBlockArea.size();
	uint cols = m_vBlockArea.front().size();
	for (uint i = 0; i < rows; i++)
	{
		for (uint j = 0; j < cols; j++)
		{
			if (m_vBlockArea[i][j].attr == def::EBA_Mine)//��ʾ������
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

//���Ҽ�ͬʱ����
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
				//���޸���ͨ��δ֪״̬
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

//���Ҽ�ͬʱ�ɿ�
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
				//���ָ��ϵ���ͨ��δ֪״̬
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

	//���޸���ͨ��δ֪״̬
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

//�Ƿ���ȷ�����Χ���е���
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
				if ((m_vBlockArea[i][j].cur_state == def::EBS_Flag && m_vBlockArea[i][j].attr != def::EBA_Mine) ||//���ױ��Ϊ��
					(m_vBlockArea[i][j].cur_state != def::EBS_Flag && m_vBlockArea[i][j].attr == def::EBA_Mine))//��δ��ǳ���
				{
					return false;
				}
			}
		}
	}

	return true;
}