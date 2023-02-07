#pragma once

#include "Application/Core.h"

class Transform
{
public:
	Transform(const glm::vec3 &position = glm::vec3(0.f), const glm::vec3 &rotation = glm::vec3(0.f), const glm::vec3 &scale = glm::vec3(1.f));
	virtual ~Transform() = default;

public:
	const glm::vec3 &GetPosition() const;
	const glm::vec3 &GetRotation() const;
	const glm::vec3 &GetScale() const;

	void SetPosition(const glm::vec3 &position);
	void SetRotation(const glm::vec3 &rotation);
	void SetScale(const glm::vec3 &scale);

	glm::mat4 GetMatrix();

protected:
	virtual void RecalculateMatrix();

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	bool isMatrixValid = false;
	glm::mat4 matrix;
};
