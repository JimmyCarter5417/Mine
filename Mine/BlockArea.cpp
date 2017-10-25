#include "StdAfx.h"
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

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			m_vBlockArea[i][j].pos.x = i;
			m_vBlockArea[i][j].pos.y = j;
			m_vBlockArea[i][j].attr = def::EBA_Empty;
			m_vBlockArea[i][j].cur_state = def::EBS_Normal;
			m_vBlockArea[i][j].old_state = def::EBS_Normal;
		}
	}

	return true;
}

//��x,yΪ���Ĳ���
bool CBlockArea::LayMines(TPos pos)
{
	if (!IsValidPos(pos))
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

		if (!(i == pos.x && j == pos.y) && m_vBlockArea[i][j].attr != def::EBA_Mine)
		{
			m_vBlockArea[i][j].attr = def::EBA_Mine;//�޸�����Ϊ��
			left--;//�������׼�һ		
		}
	}	

	return true;
}

bool CBlockArea::IsValidPos(TPos pos)
{
	return pos.x >= 0 && pos.x < m_vBlockArea.size() && pos.y >= 0 && pos.y < m_vBlockArea.front().size();
}

//������Χ������
int CBlockArea::GetAroundMineNum(TPos pos)
{
	if (!IsValidPos(pos))
		return -1;

	int res = 0;

	for (int i = pos.x - 1; i <= (int)pos.x + 1; i++)
	{
		for (int j = pos.y - 1; j <= (int)pos.y + 1; j++)
		{
			if (!(i == pos.x && j == pos.y) && IsValidPos({ i, j }))
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
int CBlockArea::GetAroundFlagNum(TPos pos)
{
	if (!IsValidPos(pos))
		return -1;

	int res = 0;

	for (int i = pos.x - 1; i <= (int)pos.x + 1; i++)
	{
		for (int j = pos.y - 1; j <= (int)pos.y + 1; j++)
		{
			if (!(i == pos.x && j == pos.y) && IsValidPos({ i, j }))
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

bool CBlockArea::IsMine(TPos pos)
{
	return m_vBlockArea[pos.x][pos.y].attr == def::EBA_Mine;
}

//����Χ����
bool CBlockArea::OpenAround(TPos pos)
{
	if (!IsValidPos(pos))
		return false;

	//��ǵ�������������Χ������������
	if (GetAroundFlagNum(pos) != GetAroundMineNum(pos))
		return true;

	for (int i = pos.x - 1; i <= (int)pos.x + 1; i++)
	{
		for (int j = pos.y - 1; j <= (int)pos.y + 1; j++)
		{
			if (IsValidPos({ i, j }))
			{				
				if (m_vBlockArea[i][j].cur_state == def::EBS_Normal)//��������
				{
					Open({ i, j });//�򿪸÷��鲢������չ����Χ����				
				}
			}
		}
	}

	return true;
}

//�򿪸÷��鲢������չ����Χ����dfs
bool CBlockArea::Open(TPos pos)
{
	if (!IsValidPos(pos))
		return false;

	//�ȴ򿪸÷���
	uint mines = GetAroundMineNum(pos);
	m_vBlockArea[pos.x][pos.y].cur_state = (def::EBlockState)(def::EBS_Empty - mines);//���ø÷���״̬
	m_vBlockArea[pos.x][pos.y].old_state = m_vBlockArea[pos.x][pos.y].cur_state;

	//��Χ���׼�����չ
	if (mines == 0)
	{
		for (int i = pos.x - 1; i <= (int)pos.x + 1; i++)
		{
			for (int j = pos.y - 1; j <= (int)pos.y + 1; j++)
			{
				if (IsValidPos({ i, j }))
				{
					if (!(i == pos.x && j == pos.y) &&
						m_vBlockArea[i][j].attr != def::EBA_Mine &&
						m_vBlockArea[i][j].cur_state == def::EBS_Normal) 
					{
						Open({ i, j });//�ݹ�
					}
				}
			}
		}
	}

	return true;
}

//�㿪���з��׷��鼴ʤ��
bool CBlockArea::IsVictory()
{
	uint rows = m_vBlockArea.size();
	uint cols = m_vBlockArea.front().size();

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (m_vBlockArea[i][j].attr == def::EBA_Empty &&//�ո�
				m_vBlockArea[i][j].cur_state < def::EBS_Num8)//δ��
				return false;
		}
	}

	return true;
}

//��ĳλ��ʧ��
bool CBlockArea::DeadAt(TPos pos)
{
	if (!IsValidPos(pos))
		return false;

	uint rows = m_vBlockArea.size();
	uint cols = m_vBlockArea.front().size();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (m_vBlockArea[i][j].attr == def::EBA_Mine)//��ʾ������
			{
				m_vBlockArea[i][j].cur_state = def::EBS_Mine;
				m_vBlockArea[i][j].old_state = m_vBlockArea[i][j].cur_state;
			}
		}
	}

	if (m_vBlockArea[pos.x][pos.y].attr == def::EBA_Mine)//������
	{
		m_vBlockArea[pos.x][pos.y].cur_state = def::EBS_Blast;//��ըͼ��
		m_vBlockArea[pos.x][pos.y].old_state = m_vBlockArea[pos.x][pos.y].cur_state;

	}
	else//ͬʱ������Ҽ�չ������ʧ��
	{
		m_vBlockArea[pos.x][pos.y].cur_state = def::EBS_Error;//����ͼ��
		m_vBlockArea[pos.x][pos.y].old_state = m_vBlockArea[pos.x][pos.y].cur_state;
	}

	return true;
}

def::EBlockState CBlockArea::GetCurState(TPos pos)
{
	if (!IsValidPos(pos))
		return def::EBS_End;

	return m_vBlockArea[pos.x][pos.y].cur_state;
}

bool CBlockArea::SetCurState(TPos pos, def::EBlockState state)
{
	if (!IsValidPos(pos))
		return false;

	return m_vBlockArea[pos.x][pos.y].cur_state = state;
}

def::EBlockState CBlockArea::GetOldState(TPos pos)
{
	if (!IsValidPos(pos))
		return def::EBS_End;

	return m_vBlockArea[pos.x][pos.y].old_state;
}

bool CBlockArea::SetOldState(TPos pos, def::EBlockState state)
{
	if (!IsValidPos(pos))
		return false;

	return m_vBlockArea[pos.x][pos.y].old_state = state;
}

def::EBlockAttr CBlockArea::GetAttr(TPos pos)
{
	if (!IsValidPos(pos))
		return def::EBA_End;

	return m_vBlockArea[pos.x][pos.y].attr;
}

//���Ҽ�ͬʱ����
bool CBlockArea::LRBtnDown(TPos pos)
{
	if (!IsValidPos(pos))
		return false;

	for (int i = pos.x - 1; i <= (int)pos.x + 1; i++)
	{
		for (int j = pos.y - 1; j <= (int)pos.y + 1; j++)
		{
			if (IsValidPos({ i, j }))
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

	return true;
}

//���Ҽ�ͬʱ�ɿ�
bool CBlockArea::LRBtnUp(TPos pos)
{
	if (!IsValidPos(pos))
		return false;

	for (int i = pos.x - 1; i <= (int)pos.x + 1; i++)
	{
		for (int j = pos.y - 1; j <= (int)pos.y + 1; j++)
		{
			if (IsValidPos({ i, j }))
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

	return true;
}

bool CBlockArea::LBtnDown(TPos pos)
{
	if (!IsValidPos(pos))
		return false;

	//���޸���ͨ��δ֪״̬
	if (m_vBlockArea[pos.x][pos.y].cur_state == def::EBS_Normal)
	{
		m_vBlockArea[pos.x][pos.y].cur_state = def::EBS_Empty;
	}
	else if (m_vBlockArea[pos.x][pos.y].cur_state == def::EBS_Dicey)
	{
		m_vBlockArea[pos.x][pos.y].cur_state = def::EBS_DiceyDown;
	}

	return true;
}

bool CBlockArea::LBtnUp(TPos pos)
{
	if (!IsValidPos(pos))
		return false;

	//���ָ��ϵ���ͨ��δ֪״̬
	if (m_vBlockArea[pos.x][pos.y].old_state == def::EBS_Normal ||
		m_vBlockArea[pos.x][pos.y].old_state == def::EBS_Dicey)
	{
		m_vBlockArea[pos.x][pos.y].cur_state = m_vBlockArea[pos.x][pos.y].old_state;
	}

	return true;
}

//�Ƿ���ȷ�����Χ���е���
bool CBlockArea::HasCorrectFlags(TPos pos)
{
	if (!IsValidPos(pos))
		return true;

	for (int i = pos.x - 1; i <= (int)pos.x + 1; i++)
	{
		for (int j = pos.y - 1; j <= (int)pos.y + 1; j++)
		{
			if (IsValidPos({ i, j }))
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

bool CBlockArea::ShowMines(bool show)
{
	uint rows = m_vBlockArea.size();
	uint cols = m_vBlockArea.front().size();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (m_vBlockArea[i][j].attr == def::EBA_Mine)
			{
				if (show)//��ʾ
				{
					m_vBlockArea[i][j].cur_state = def::EBS_Mine;
					//m_vBlockArea[i][j].old_state = m_vBlockArea[i][j].cur_state;
				}
				else//��ԭ
				{
					m_vBlockArea[i][j].cur_state = m_vBlockArea[i][j].old_state;
				}
			}
		}
	}

	return true;
}