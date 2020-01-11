#pragma once

#include <TheForge/ECS/BaseComponent.h>

class WorldBoundsComponent : public BaseComponent {

	FORGE_DECLARE_COMPONENT(WorldBoundsComponent)

public:
	WorldBoundsComponent();

	float xMin, xMax, yMin, yMax;
};
