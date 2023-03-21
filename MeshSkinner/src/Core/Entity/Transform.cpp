#include "pch.h"
#include "Transform.h"

const glm::vec3 Transform::vectorForward = { 1.f, 0.f, 0.f };
const glm::vec3 Transform::vectorRight = { 0.f, 0.f, 1.f };
const glm::vec3 Transform::vectorUp = { 0.f, 1.f, 0.f };

Transform::Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) : position(position), rotation(rotation), scale(scale), isMatrixUpdated(false)
{
	RecalculateMatrix();
}

Transform::Transform(const glm::mat4 &mat)
{
	assert(false);

	// this is not fully correct so needs to be reimplemented
	SetPosition(glm::vec3(mat[3][0], mat[3][1], mat[3][2]));
	SetScale(glm::vec3(glm::length(glm::vec3(mat[0])) * glm::sign(mat[0][0]), glm::length(glm::vec3(mat[1])) * glm::sign(mat[1][1]), glm::length(glm::vec3(mat[2])) * glm::sign(mat[2][2])));
	SetRotation(glm::degrees(glm::eulerAngles(glm::quat(glm::mat3(mat[0] / scale.x, mat[1] / scale.y, mat[2]/scale.z)))));
}

const glm::vec3 &Transform::GetPosition() const	{ return position; }
const glm::vec3 &Transform::GetRotation() const	{ return rotation; }
const glm::vec3 &Transform::GetScale() const	{ return scale; }

void Transform::SetPosition(const glm::vec3 &position)
{
	if (position == this->position)
		return;

	this->position = position;

	if (isMatrixUpdated)
	{
		isMatrixUpdated = false;
		onMatrixDirty.Invoke();
	}
}

void Transform::SetRotation(const glm::vec3 &rotation)
{
	if (rotation == this->rotation)
		return;

	this->rotation = rotation;

	if (isMatrixUpdated)
	{
		isMatrixUpdated = false;
		onMatrixDirty.Invoke();
	}
}

void Transform::SetScale(const glm::vec3 &scale)
{
	if (scale == this->scale)
		return;

	this->scale = scale;

	if (isMatrixUpdated)
	{
		isMatrixUpdated = false;
		onMatrixDirty.Invoke();
	}
}

void Transform::Translate(const glm::vec3 &translation)	{ SetPosition(GetPosition() + translation); }
void Transform::Rotate(const glm::vec3 &rotation)		{ SetRotation(GetRotation() + rotation); }
void Transform::Scale(const glm::vec3 &scaleMultiplier)	{ SetScale(GetScale() * scaleMultiplier); }

const glm::vec3 Transform::GetForwardVector() const	{ return glm::quat(glm::radians(rotation)) * vectorForward; }
const glm::vec3 Transform::GetRightVector() const	{ return glm::quat(glm::radians(rotation)) * vectorRight; }
const glm::vec3 Transform::GetUpVector() const		{ return glm::quat(glm::radians(rotation)) * vectorUp; }

Transform Transform::operator*(Transform &other)
{
	// the scale may be off but it's kinda complicated to  get this right
	auto pos = glm::vec3(this->GetMatrix() * glm::vec4(other.GetPosition(), 1.f));
	auto rotQ = glm::quat(glm::radians(this->GetRotation())) * glm::quat(glm::radians(other.GetRotation()));
	auto rot = glm::degrees(glm::eulerAngles(rotQ));
	auto sca = this->GetScale() * other.GetScale();

	// the below doesn't give correct results
	//auto rotScale = glm::mat3(this->GetMatrix()) * glm::mat3(other.GetMatrix());
	//auto justRot = glm::toMat3(rotQ);
	//auto scaleMat = rotScale * glm::inverse(justRot);
	//sca = glm::vec3(glm::length(scaleMat[0]), glm::length(scaleMat[1]), glm::length(scaleMat[2]));
	//Log::Info("the scale would be {}", sca);

	return Transform(pos, rot, sca);
}

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

void Transform::DisplayUI()
{
	glm::vec3 positionCopy = GetPosition();
	glm::vec3 rotationCopy = GetRotation();
	glm::vec3 scaleCopy = GetScale();
	ImGui::Text("Transform");
	ImGui::DragFloat3("Position", glm::value_ptr(positionCopy), 1.f, -1000000000.f, 1000000000.f);
	ImGui::DragFloat3("Rotation", glm::value_ptr(rotationCopy), 1.f, -1000000000.f, 1000000000.f);
	ImGui::DragFloat3("Scale", glm::value_ptr(scaleCopy), 0.05f, -1000000000.f, 1000000000.f);
	SetPosition(positionCopy);
	SetRotation(rotationCopy);
	SetScale(scaleCopy);
}

void Transform::OnMatrixDirtySubscribe(const CallbackNoArgRef &callback)
{
	onMatrixDirty.Subscribe(callback);
}

void Transform::OnMatrixDirtyUnsubscribe(const CallbackNoArgRef &callback)
{
	onMatrixDirty.Unsubscribe(callback);
}
