#pragma once
#include "../../maths/Vector3.h"
#include "../../maths/Matrix.h"
#include "../../ecs/Component.h"
namespace DXH {

	struct Camera : public Component
	{
		Camera() {}

		Matrix GetViewMatrix() const;
		Matrix GetViewProjectionMatrix() const;

		float FieldOfView = 65.f;
		float NearPlan = 0.001f;
		float FarPlan = 1000.f;
		bool IsPrimary = false;

		Matrix View = Matrix::Identity;
		Matrix Proj = Matrix::Identity;
		Vector3 Target;
	};
}