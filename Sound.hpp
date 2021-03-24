#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>

enum SoundEffect
{
	Pause, SoftDrop, HardDrop, LinesCleared, Tetris
};

class Sound
{
private:
	std::unordered_map<SoundEffect, sf::SoundBuffer> effects;
	sf::Music backgroundMusic;

public:
	Sound() {}
	~Sound() {}

	void playSoundEffect(SoundEffect effect)
	{

	}

	void loadSoundEffects()
	{

	}

	void playBackgroundMusic()
	{

	}

	void pauseBackgroundMusic()
	{

	}

	void stopBackgroundMusic()
	{

	}

	void nextLevelReached()
	{

	}
};

