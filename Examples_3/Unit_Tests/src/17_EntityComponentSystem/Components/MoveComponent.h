#pragma once

#include <TheForge/ECS/BaseComponent.h>

class MoveComponent : public BaseComponent {

	FORGE_DECLARE_COMPONENT(MoveComponent)

public:
	MoveComponent();

	void Initialize(float minSpeed, float maxSpeed);

	float velx, vely;
};
