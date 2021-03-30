#pragma once

#include <SFML/Audio.hpp>
#include <array>
#include <fstream>

enum SoundEffect 
{
	Pause, SoftDrop, HardDrop, LinesCleared, Tetris
};

class Sound
{
private:
	static const int numberOfSoundEffects{ 5 };
	std::array<sf::SoundBuffer, Sound::numberOfSoundEffects> effects;
	sf::Music menuMusic;
	sf::Music backgroundMusic;
	sf::Sound sound;

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
		sound.setBuffer(effects[effect]);
		sound.play();
	}

	void loadSoundEffects()
	{
		// I changed effects to an array which might be easier to work with.
		// You can fill out the rest of the method if you like.

		// Pause
		effects[SoundEffect::Pause].loadFromFile("Sounds/pause.wav");

		// SoftDrop
		// HardDrop
		// LinesCleared
		// Tetris
	}

	void playMenuMusic()
	{
		if (!menuMusic.openFromFile("Sounds/menu.wav"))
			return;
		menuMusic.play();
	}

	void stopMenuMusic()
	{
		menuMusic.stop();
	}

	void playBackgroundMusic()
	{
		if (!backgroundMusic.openFromFile("music.wav"))
			return;
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

