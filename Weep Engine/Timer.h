#ifndef __j1TIMER_H__
#define __j1TIMER_H__

class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	int Read() const;
	float ReadSec() const;
	void SubstractTimeFromStart(float sec);
	void Stop();
	void PauseOn();
	void PauseOff();

	bool IsActive();


private:
	int	    started_at;
	bool	paused = false;
	int     paused_at = 0;
	bool	active = false;
};

#endif //__j1TIMER_H__