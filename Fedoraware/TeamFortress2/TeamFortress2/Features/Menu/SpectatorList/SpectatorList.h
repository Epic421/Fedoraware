#pragma once
#include "../../../SDK/SDK.h"

class CSpectatorList
{
private:
	struct Spectator_t
	{
		std::wstring Name;
		std::wstring Mode;
		bool IsFriend;
		int Team;
		int Index;
	};

	std::vector<Spectator_t> Spectators;
	int SpecListW = 200, SpecListTitleBarH = 15;

public:
	int SpecListX = 3, SpecListY = 450;
	bool GetSpectators(CBaseEntity* pLocal);
	bool ShouldRun();
	void Run();
	void DragSpecList(int& x, int& y, int w, int h, int offsety);
	void DrawDefault();
	void DrawClassic();
};

ADD_FEATURE(CSpectatorList, SpectatorList)