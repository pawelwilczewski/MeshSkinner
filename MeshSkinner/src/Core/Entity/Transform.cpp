#include "pch.h"
#include "Transform.h"

const glm::vec3 Transform::vectorForward = { 1.f, 0.f, 0.f };
const glm::vec3 Transform::vectorRight = { 0.f, 0.f, 1.f };
const glm::vec3 Transform::vectorUp = { 0.f, 1.f, 0.f };

Transform::Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) : position(position), rotation(rotation), scale(scale), isMatrixUpdated(false)
{
	RecalculateMatrix();
}

const glm::vec3 &Transform::GetPosition() const	{ return position; }
const glm::vec3 &Transform::GetRotation() const	{ return rotation; }
const glm::vec3 &Transform::GetScale() const	{ return scale; }

void Transform::SetPosition(const glm::vec3 &position)
{
	this->position = position;
	isMatrixUpdated = false;
}

void Transform::SetRotation(const glm::vec3 &rotation)
{
	this->rotation = rotation;
	isMatrixUpdated = false;
}

void Transform::SetScale(const glm::vec3 &scale)
{
	this->scale = scale;
	isMatrixUpdated = false;
}

void Transform::Translate(const glm::vec3 &translation)	{ SetPosition(GetPosition() + translation); }
void Transform::Rotate(const glm::vec3 &rotation)		{ SetRotation(GetRotation() + rotation); }
void Transform::Scale(const glm::vec3 &scaleMultiplier)	{ SetScale(GetScale() * scaleMultiplier); }

const glm::vec3 Transform::GetForwardVector() const	{ return glm::quat(glm::radians(rotation)) * vectorForward; }
const glm::vec3 Transform::GetRightVector() const	{ return glm::quat(glm::radians(rotation)) * vectorRight; }
const glm::vec3 Transform::GetUpVector() const		{ return glm::quat(glm::radians(rotation)) * vectorUp; }

bool Transform::IsMatrixUpdated() const
{
	return isMatrixUpdated;
}

const glm::mat4 &Transform::GetMatrix()
{
	if (!isMatrixUpdated)
		RecalculateMatrix();

	return matrix;
}

void Transform::RecalculateMatrix()
{
	matrix = glm::translate(glm::mat4(1.f), position);
	matrix *= glm::toMat4(glm::quat(glm::radians(rotation)));
	matrix = glm::scale(matrix, scale);

	isMatrixUpdated = true;
}
