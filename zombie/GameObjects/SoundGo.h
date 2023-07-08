#pragma once
class SoundGo
{
protected:
public:
	sf::String soundbufferfile;
	sf::SoundBuffer soundbuffer;
	sf::Sound sound;
	SoundGo(std::string soundbufferfile);
	~SoundGo();

	void SetSound();
	void SoundInBuffer();
	void SoundPlayer();
};

