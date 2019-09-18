#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	~Timer();

	void Start();
	void Stop();

	Uint32 Read() const;
	float ReadSec() const;
	void SubstractTimeFromStart(float sec);

	bool IsActive();

private:

	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
	bool	active = false;
};

#endif //__TIMER_H__