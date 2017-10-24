#ifndef __BLOCK_AREA_H__
#define __BLOCK_AREA_H__

#include "Def.h"
#include <vector>

using def::uint;
using std::vector;

struct TBlock
{
	uint x;//行
	uint y;//列
	def::EBlockAttr  attr;//雷、空
	def::EBlockState cur_state;//当前状态
	def::EBlockState old_state;//用以恢复以前的状态
};

class CBlockArea
{
public:
	static CBlockArea* GetInstance();

	bool Init(uint rows, uint cols, uint mines);
	bool LayMines(uint x, uint y);
	bool IsValidPos(uint x, uint y);
	bool IsMine(uint x, uint y);
	bool OpenAround(uint x, uint y);
	bool Open(uint x, uint y);
	bool IsVictory();
	bool DeadAt(uint x, uint y);
	bool HasCorrentFlags(uint x, uint y);

	bool LBtnDown(uint x, uint y);
	bool LBtnUp(uint x, uint y);
	bool LRBtnDown(uint x, uint y);
	bool LRBtnUp(uint x, uint y);
	
	def::EBlockState GetCurState(uint x, uint y);
	bool SetCurState(uint x, uint y, def::EBlockState state);
	
	def::EBlockState GetOldState(uint x, uint y);
	bool SetOldState(uint x, uint y, def::EBlockState state);

	def::EBlockAttr GetAttr(uint x, uint y);

	int GetAroundMineNum(uint x, uint y);
	int GetAroundFlagNum(uint x, uint y);

private:
	CBlockArea();

	//删除拷贝与赋值
	CBlockArea(const CBlockArea&) = delete;
	CBlockArea& operator=(const CBlockArea&) = delete;

private:
	vector<vector<TBlock>> m_vBlockArea;
	uint                   m_uMineNum;
};

#endif//__BLOCK_AREA_H__