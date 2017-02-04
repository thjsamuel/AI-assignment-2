#ifndef BASE_GAME_ENTITY_H
#define BASE_GAME_ENTITY_H

#include "Vector3.h"
#include "Messaging\Telegram.h"
#include <map>
#include "EntityNames.h"
using std::map;

class CBaseGameEntity
{
public:
	CBaseGameEntity(int ID);
	virtual ~CBaseGameEntity();

	virtual void Update(double dt) = 0;
	virtual bool HandleMessage(const Telegram& msg) = 0;

	int GetID();
	std::string GetIDInText();

	virtual std::string GetStateInText() = 0;

	void SetPosition(float x, float y, float z = 0);
	Vector3 GetPosition();

	void SetSpriteInvertStatus(bool bSpriteInvert);
	bool GetSpriteInvertStatus();

	void SetIdleState(bool bIdle);
	bool GetIdleState();

	// For chef use only
	void SetCookingState(bool bCooking);
	bool GetCookingState();

	// For customers use only, since cannot use dynamic_cast
	void SetSeatPosition(Vector3 seatPos);
	Vector3 GetSeatPosition();

	void SetSeatedStatus(bool bSeated);
	bool GetSeatedStatus();

	void SetExitStatus(const bool bExited);
	bool GetExitStatus() const;

	// SetLeader();
	// GetLeaderStatus();

	void SetDone(const bool bDone); // finished eating
	bool GetDoneStatus() const;

    map<const int, Vector3> waypoints;
//protected:
	Vector3 position;

    void SetID(int value);

private:
	int m_iID;
	static int m_iNextValidID;

	bool bIdle;

	// For chef use only
	bool bCooking;

	// For customers use only, since cannot use dynamic_cast
	Vector3 seatPos;
	bool bSeated;
	bool bExited;
	bool bDone;
	bool bSpriteInvert;
};

#endif // BASE_GAME_ENTITY_H