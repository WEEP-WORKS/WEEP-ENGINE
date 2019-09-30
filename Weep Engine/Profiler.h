#ifndef __PROFILER_H__
#define __PROFILER_H__

#include "SDL\include\SDL.h"
#include <vector>
#include <iostream>

#define MAX_FRAMES_GRAPH 25
#define MAX_MEMORY_GRAPH 25

struct Profile
{
	const char* name;
	float frame_start = 0.0f;
	float last_frame_ms = 0.0f;
	float total_frames_ms = 0.0f;
};

class Profiler
{
public:
	Profiler();
	~Profiler();
	void CleanUp();

	void AwakeTime();
	void StartTime();
	void UpdateFinish();

	float GetAwakeTime();
	float GetStartTime();
	float GetFrameTime();
	int GetFPS();
	float GetAvgFPS();
	int GetFramesSinceStartup();
	int GetTimeSinceStartup();

	void StartProfile(const char* name);
	void FinishProfile();
	Profile* GetProfile(const char* name);

	std::vector<float> GetFramesVector();
	std::vector<float> GetMillisecondsVector();
	std::vector<float> GetMemoryVector();

	//void SetMaxFps(int fps);
	//int GetMaxFps();

public:
	int	  capped_ms = -1;

	int	  max_fps = 0;

private:	
	float create_time = 0.0f;

	float awake_Ttime = 0.0f;
	float start_Ttime = 0.0f;
	float update_start_time = 0.0f;
	float update_time_from_create = 0.0f;

	int   frames_since_create = 0;
	float frame_ms = 0.0f;
	float avg_fps = 0.0f;
	std::vector<float> frames;
	std::vector<float> milliseconds;
	std::vector<float> memory;

	int frame_counter = 0;
	float frame_counter_ms = 0.0f;
	int last_second_frames = 0;

	Profile* current_profile = nullptr;
	std::vector<Profile*> profiles;


};

#endif //__PROFILER_H__
