#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <fstream>

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

	void playSound()
	{
		std::ofstream out;

		out.open("output.txt", std::ios::app);

		//Load a sound buffer from a wav file
		sf::SoundBuffer buffer;
		if (!buffer.loadFromFile("sounds/pause.wav")) {
			return;
		}

		//Display sound information
		out << "orchestral.wav: " << std::endl;
		out << " " << buffer.getDuration().asSeconds() << "seconds" << std::endl;
		out << " " << buffer.getSampleRate() << "seconds / sec" << std::endl;
		out << " " << buffer.getChannelCount() << "channels" << std::endl;

		//Create a sound instance and play it
		sf::Sound sound;
		sound.setBuffer(buffer);
		sound.play();

		//Loop while the sound is playing
		while (sound.getStatus() == sf::Sound::Playing)
		{
			//Leave some cpu time for other process
			sf::sleep(sf::milliseconds(100));

			//Display the playing position
			out << "\rPlaying..." << sound.getPlayingOffset().asSeconds() << "sec  ";
		}
		out << std::endl << std::endl;

		out.close();
	}

	void playSoundEffect(SoundEffect effect)
	{
		if (!backgroundMusic.openFromFile("music.wav"))
			return;
		backgroundMusic.play();
	}

	void loadSoundEffects()
	{

	}

	void playBackgroundMusic()
	{
		backgroundMusic.play();
	}

	void pauseBackgroundMusic()
	{

		backgroundMusic.pause();
	}

	void stopBackgroundMusic()
	{

		backgroundMusic.pause();
	}

	void nextLevelReached()
	{

	}
};

