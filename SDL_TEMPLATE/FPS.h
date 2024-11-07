#pragma once

#include <SDL.h>

namespace FPS {
	void calculateAverageFPS(int& countFrame, Uint32& startTime);

    void capFPS(const Uint32& frameStart);

};

