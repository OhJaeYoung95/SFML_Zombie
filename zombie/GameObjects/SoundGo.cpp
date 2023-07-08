#include "stdafx.h"
#include "SoundGo.h"

SoundGo::SoundGo(std::string soundbufferfile)
	: soundbufferfile(soundbufferfile)
{
}

SoundGo::~SoundGo()
{
}

void SoundGo::SoundInBuffer()
{
	soundbuffer.loadFromFile(soundbufferfile);
}

void SoundGo::SetSound()
{
	sound.setBuffer(soundbuffer);
}

void SoundGo::SoundPlayer()
{
	SoundInBuffer();
	SetSound();
	sound.play();
}

