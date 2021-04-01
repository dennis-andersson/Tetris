#pragma once

#include <SFML/Audio.hpp>
#include <array>
#include <fstream>

enum SoundEffect 
{
	Pause, SoftDrop, HardDrop, Rotate, LinesCleared, Tetris
};

class Sound
{
protected:
	Sound() {}
	~Sound() {}

	static Sound* soundInstance;
	
private:
	static const int numberOfSoundEffects{ 6 };
	std::array<sf::SoundBuffer, Sound::numberOfSoundEffects> effects;
	sf::Music menuMusic;
	sf::Music backgroundMusic;
	sf::Sound sound;

public:
	Sound(Sound& other) = delete;
	
	void operator=(const Sound&) = delete;

	static Sound& GetInstance()
	{
		if (soundInstance == nullptr)
			soundInstance = new Sound;
		return *(soundInstance);
	}


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
		effects[SoundEffect::SoftDrop].loadFromFile("Sounds/line-drop.wav");

		// HardDrop
		effects[SoundEffect::HardDrop].loadFromFile("Sounds/force-hit.wav");

		// Rotate
		effects[SoundEffect::Rotate].loadFromFile("Sounds/block-rotate.wav");

		// LinesCleared
		effects[SoundEffect::LinesCleared].loadFromFile("Sounds/line-remove.wav");

		// Tetris
		effects[SoundEffect::Tetris].loadFromFile("Sounds/line-remove.wav");
	}

	void playMenuMusic()
	{
		if (menuMusic.getStatus() != sf::SoundSource::Status::Playing) {
			if (!menuMusic.openFromFile("Sounds/menu.ogg"))
				return;
			menuMusic.setLoop(true);
			menuMusic.play();
		}
	}

	void stopMenuMusic()
	{
		menuMusic.stop();
	}

	void playBackgroundMusic()
	{
		if (backgroundMusic.getStatus() != sf::SoundSource::Status::Playing) {
			if (!backgroundMusic.openFromFile("Sounds/music.ogg"))
				return;
			backgroundMusic.setLoop(true);
			backgroundMusic.play();
		}
	}

	void pauseBackgroundMusic()
	{
		backgroundMusic.pause();
	}

	void stopBackgroundMusic()
	{
		backgroundMusic.stop();
	}

	/*void nextLevelReached()
	{

	}*/
};

Sound* Sound::soundInstance = nullptr;
