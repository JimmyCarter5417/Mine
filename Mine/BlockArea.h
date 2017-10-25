#ifndef __BLOCK_AREA_H__
#define __BLOCK_AREA_H__

#include "Def.h"
#include <vector>

using def::uint;
using std::vector;

struct TPos
{
	uint x;//行
	uint y;//列
};

struct TBlock
{
	TPos pos;
	def::EBlockAttr  attr;//雷、空
	def::EBlockState cur_state;//当前状态
	def::EBlockState old_state;//用以恢复以前的状态
};

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
	
	def::EBlockState GetCurState(TPos pos);
	bool SetCurState(TPos pos, def::EBlockState state);
	
	def::EBlockState GetOldState(TPos pos);
	bool SetOldState(TPos pos, def::EBlockState state);

	def::EBlockAttr GetAttr(TPos pos);

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

#endif//__BLOCK_AREA_H__