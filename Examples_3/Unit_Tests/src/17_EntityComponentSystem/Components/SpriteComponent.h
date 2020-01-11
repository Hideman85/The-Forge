#pragma once

#include <TheForge/ECS/BaseComponent.h>

class SpriteComponent : public BaseComponent {

	FORGE_DECLARE_COMPONENT(SpriteComponent)

public:
	SpriteComponent();

	float colorR, colorG, colorB;
	int   spriteIndex;
	float scale;
};
