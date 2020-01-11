#pragma once

#include <TheForge/ECS/BaseComponent.h>

class PositionComponent : public BaseComponent {

	FORGE_DECLARE_COMPONENT(PositionComponent)

public:
	PositionComponent();

	float x, y;
};
