#pragma once
#include "../../SDK/SDK.h"

#pragma warning ( disable : 4091 )

class CIncomingSequence
{
public:
	int InReliableState;
	int SequenceNr;
	float CurTime;

	CIncomingSequence(int inState, int seqNr, float time)
	{
		InReliableState = inState;
		SequenceNr = seqNr;
		CurTime = time;
	}
};

using BoneMatrixes = struct
{
	float BoneMatrix[128][3][4];
};

struct TickRecord
{
	float SimulationTime = -1;
	int TickCount = -1;
	Vec3 HeadPosition = { };
	Vec3 AbsOrigin = { };
	BoneMatrixes BoneMatrix{ };
	model_t* Model = nullptr;
	studiohdr_t* HDR = nullptr;
	int  HitboxSet = 0;
	Vec3 Mins = Vec3();
	Vec3 Maxs = Vec3();
	Vec3 WorldSpaceCenter = { };
	Vec3 EyeAngles = { };
};

struct TickRecordNew{
	float flSimTime = 0.f;
	float flCreateTime = 0.f;
	int iTickCount = 0;
	bool bOnShot = false;
	BoneMatrixes BoneMatrix{};
	Vec3 vOrigin = {};
	Vec3 vAngles = {};
};

enum class BacktrackMode
{
	ALL,		//	iterates through every tick (slow probably)
	FIRST,		//	first
	LAST,		//	last
	ADAPTIVE,	//	prefers on shot records, last
	ONSHOT,		//	only returns on shot records
};

class CBacktrackNew
{
private:
	//	logic
	bool IsTracked(TickRecordNew Record);
	bool IsSimulationReliable(CBaseEntity* pEntity);
	//bool IsBackLagComped(CBaseEntity* pEntity);

	//	utils
	void CleanRecords();
	void MakeRecords();
	std::optional<TickRecordNew> GetHitRecord(CUserCmd* pCmd, CBaseEntity* pEntity, const Vec3 vAngles, const Vec3 vPos);
	//	utils - fake latency
	void UpdateDatagram();
	float GetLatency();

	//	data
	std::unordered_map<CBaseEntity*, std::deque<TickRecordNew>> mRecords;
	std::unordered_map<int, bool> mDidShoot;
	int iLastCreationTick = 0;

	//	data - fake latency
	std::deque<CIncomingSequence> dSequences;
	float flLatencyRampup = 0.f;
	int iLastInSequence = 0;
public:
	bool WithinRewind(TickRecordNew Record);
	void PlayerHurt(CGameEvent* pEvent);	//	called on player_hurt event
	void Restart();	//	called whenever lol
	void FrameStageNotify();	//	called in FrameStageNotify
	void ReportShot(int iIndex);
	std::deque<TickRecordNew>* GetRecords(CBaseEntity* pEntity);
	std::optional<TickRecordNew> Aimbot(CBaseEntity* pEntity, BacktrackMode iMode, int nHitbox);
	std::optional<TickRecordNew> GetLastRecord(CBaseEntity* pEntity);
	std::optional<TickRecordNew> GetFirstRecord(CBaseEntity* pEntity);
	std::optional<TickRecordNew> Run(CUserCmd* pCmd);	//	returns a valid record
	void AdjustPing(INetChannel* netChannel);	//	blurgh
	bool bFakeLatency = false;
};

class CBacktrack
{
	//std::optional<TickRecord> GetLastRecord(int entIdx);
	//std::optional<TickRecord> GetFirstRecord(int entIdx);

	void UpdateDatagram();
	float GetLatency();

	float LatencyRampup = 0.f;
	int LastInSequence = 0;
	

public:
	//void Run();
	void AdjustPing(INetChannel* netChannel);
	void ResetLatency();
	//bool IsGoodTick(float simTime);

	std::deque<TickRecord>* GetPlayerRecords(int entIdx);
	//std::optional<TickRecord> GetRecord(int entIdx, BacktrackMode mode);

	bool AllowLatency = false;
	std::array<std::deque<TickRecord>, 64> Records;
};

ADD_FEATURE(CBacktrack, Backtrack)
ADD_FEATURE(CBacktrackNew, BacktrackNew)