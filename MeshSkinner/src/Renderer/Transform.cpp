#include "pch.h"
#include "Transform.h"

Transform::Transform() :
	position(glm::vec3(0.f)),
	rotation(glm::vec3(0.f)),
	scale(glm::vec3(1.f))
{
	RecalculateMatrix();
}

const glm::vec3 &Transform::GetPosition() const { return position; }
const glm::vec3 &Transform::GetRotation() const { return rotation; }
const glm::vec3 &Transform::GetScale() const { return scale; }

void Transform::SetPosition(const glm::vec3 &position)
{
	this->position = position;
	isMatrixValid = false;
}

void Transform::SetRotation(const glm::vec3 &rotation)
{
	this->rotation = rotation;
	isMatrixValid = false;
}

void Transform::SetScale(const glm::vec3 &scale)
{
	this->scale = scale;
	isMatrixValid = false;
}

glm::mat4 Transform::GetMatrix()
{
	if (!isMatrixValid)
		RecalculateMatrix();

	return matrix;
}

void Transform::RecalculateMatrix()
{
	matrix = glm::translate(glm::mat4(1.f), position);
	matrix *= glm::toMat4(glm::quat(glm::radians(rotation)));
	matrix = glm::scale(matrix, scale);

	isMatrixValid = true;
}
