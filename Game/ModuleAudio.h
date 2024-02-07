#pragma once
#include "BaseModule.h"
#include <vector>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:
	Audio(bool startEnabled);

	virtual ~Audio();

	//called before render is available
	bool Awake(pugi::xml_node&);

	//called before quitting
	bool CleanUp();

	//play a music file
	bool PlayMusic(std::string path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	//load a WAV in memory
	unsigned int LoadFx(std::string path);

	//play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

private:

	_Mix_Music* music;
	std::vector<Mix_Chunk*> fx;
};