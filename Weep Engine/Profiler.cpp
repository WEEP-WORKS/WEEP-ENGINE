#include "Profiler.h"
#include "Globals.h"

Profiler::Profiler()
{
	create_time = SDL_GetTicks();
}

Profiler::~Profiler()
{
}

void Profiler::CleanUp()
{
	for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	{
		delete (*it);
	}

	profiles.clear();
}

void Profiler::AwakeTime()
{
	awake_Ttime = SDL_GetTicks() - create_time;
}

void Profiler::StartTime()
{
	start_Ttime = SDL_GetTicks() - awake_Ttime;

	update_start_time = SDL_GetTicks();
	update_time_from_create = 0.0f;
}

void Profiler::UpdateFinish()
{
	frames_since_create++;
	frame_ms = SDL_GetTicks() - (update_start_time + update_time_from_create);
	avg_fps = float(frames_since_create) / update_time_from_create;

	frame_counter++;
	frame_counter_ms += frame_ms;
	if (frame_counter_ms > 1000)
	{
		last_second_frames = frame_counter;
		frame_counter = 0;
		frame_counter_ms = frame_counter_ms - 1000;
		frames.push_back(last_second_frames);
		if (frames.size() > MAX_FRAMES_GRAPH) {
			frames.erase(frames.begin());
		}
		milliseconds.push_back(frame_counter_ms);
		if (milliseconds.size() > MAX_FRAMES_GRAPH) {
			milliseconds.erase(milliseconds.begin());
		}
	}

	update_time_from_create = SDL_GetTicks() - update_start_time;
}

float Profiler::GetAwakeTime()
{
	return awake_Ttime;
}

float Profiler::GetStartTime()
{
	return start_Ttime;
}


float Profiler::GetFrameTime()
{
	return frame_ms;
}

float Profiler::GetAvgFPS()
{
	return avg_fps;
}

int Profiler::GetFPS()
{
	return last_second_frames;
}

int Profiler::GetFramesSinceStartup()
{
	return frames_since_create;
}

int Profiler::GetTimeSinceStartup()
{
	return SDL_GetTicks();
}


void Profiler::StartProfile(const char * name)
{
	bool found = false;

	for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	{
		if ((*it)->name == name)
		{
			(*it)->frame_start = SDL_GetTicks();
			current_profile = (*it);

			found = true;
			break;
		}
	}

	if (!found)
	{
		Profile* prof = new Profile();
		prof->name = name;
		prof->frame_start = SDL_GetTicks();
		profiles.push_back(prof);

		current_profile = prof;
	}
}

void Profiler::FinishProfile()
{
	if (current_profile != nullptr)
	{
		current_profile->last_frame_ms = SDL_GetTicks() - current_profile->frame_start;
		current_profile->total_frames_ms += current_profile->last_frame_ms;
	}
}

Profile * Profiler::GetProfile(const char * name)
{
	Profile* ret = nullptr;

	for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	{
		if ((*it)->name == name)
		{
			ret = (*it);
			break;
		}
	}

	return ret;
}

std::vector<float> Profiler::GetFramesVector()
{
	return frames;
}

std::vector<float> Profiler::GetMillisecondsVector()
{
	return milliseconds;
}