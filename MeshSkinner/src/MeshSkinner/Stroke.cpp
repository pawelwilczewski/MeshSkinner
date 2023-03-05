#include "pch.h"
#include "Stroke.h"

StrokeQueryInfo::StrokeQueryInfo(bool hitTarget, const glm::vec3 &worldPosition, const glm::vec2 &viewportPosition)
	: hitTarget(hitTarget), worldPosition(worldPosition), viewportPosition(viewportPosition)
{
}

Stroke::Stroke(const std::string &toolWindowName, const std::function<void(StrokeQueryInfo &)> &queryFunction, Type type, float pixelDistance, float worldDistance, float fixedIntervalTime)
	: Tool(toolWindowName), queryFunction(queryFunction), type(type), pixelDistance(pixelDistance), worldDistance(worldDistance), fixedIntervalTime(fixedIntervalTime)
{
	onStrokeUpdateCallback = MakeCallbackNoArgRef([&]() { OnStrokeUpdate(); });
	onMouseButtonUpCallback = MakeCallbackRef<int>([&](int button) { OnMouseButtonUp(button); });
	onStrokeUpdateVec2CallbackWrapper = MakeCallbackRef<glm::vec2>([&](const glm::vec2 &) { OnStrokeUpdate(); });

	Input::OnMouseButtonReleasedSubscribe(onMouseButtonUpCallback);

	UpdateSubscribe();
}

Stroke::~Stroke()
{
	UpdateUnsubscribe();

	Input::OnMouseButtonReleasedUnsubscribe(onMouseButtonUpCallback);
}

void Stroke::OnStrokeUpdate()
{
	if (!Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || !UserInterface::GetClickedInViewport())
		return;

	auto query = StrokeQueryInfo();
	query.viewportPosition = Input::GetMouseViewportPosition();

	bool shouldAttemptEmplace;
	if (initialContact)
	{
		queryFunction(query);
		shouldAttemptEmplace = true;
		initialContact = false;
	}
	else
	{
		switch (type)
		{
		case Type::PixelDistance:
			if (glm::distance(lastViewportPosition, query.viewportPosition) >= pixelDistance)
			{
				queryFunction(query);
				shouldAttemptEmplace = true;
			}
			else
				shouldAttemptEmplace = false;
			break;

		case Type::WorldDistance:
			queryFunction(query);
			shouldAttemptEmplace = glm::distance(lastWorldPosition, query.worldPosition) >= worldDistance;
			break;

		case Type::EachFrame:
			queryFunction(query);
			shouldAttemptEmplace = true;
			break;

		case Type::FixedDeltaTime:
			queryFunction(query);
			shouldAttemptEmplace = true;
			break;

		default:
			assert(false);
		}
	}

	if (shouldAttemptEmplace && query.hitTarget)
	{
		onStrokeEmplace.Invoke(query);
		lastViewportPosition = query.viewportPosition;
		lastWorldPosition = query.worldPosition;
	}
}

void Stroke::OnMouseButtonUp(int button)
{
	initialContact = true;
}

void Stroke::UpdateSubscribe() const
{
	switch (type)
	{
	case Type::EachFrame:
		Application::OnUpdateSubscribe(onStrokeUpdateCallback);
		break;
	case Type::FixedDeltaTime:
		assert(false); // TODO: implement
		break;
	case Type::PixelDistance:
		Input::OnMouseMovedSubscribe(onStrokeUpdateVec2CallbackWrapper); // TODO: reconsider cos lagging because single-threaded
		break;
	case Type::WorldDistance:
		Application::OnUpdateSubscribe(onStrokeUpdateCallback);
		break;
	default:
		assert(false);
	}
}

void Stroke::UpdateUnsubscribe() const
{
	switch (type)
	{
	case Type::EachFrame:
		Application::OnUpdateUnsubscribe(onStrokeUpdateCallback);
		break;
	case Type::FixedDeltaTime:
		assert(false); // TODO: implement
		break;
	case Type::PixelDistance:
		Input::OnMouseMovedUnsubscribe(onStrokeUpdateVec2CallbackWrapper);
		break;
	case Type::WorldDistance:
		Application::OnUpdateUnsubscribe(onStrokeUpdateCallback);
		break;
	default:
		assert(false);
	}
}

void Stroke::OnUpdateUI()
{
	ImGui::Begin(toolWindowName.c_str());

	const char *items[] = { "Pixel Distance", "World Distance", "Each Frame", "Fixed Delta Time" }; // TODO: obviously do not have it fixed here like that
	Type tempType = type;
	if (InteractiveWidget(ImGui::ListBox("Stroke update type", (int *)(&tempType), items, 4)))
		SetType(tempType);

	switch (type)
	{
	case Type::PixelDistance:
		InteractiveWidget(ImGui::DragFloat("Pixel distance", &pixelDistance, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic));
		break;
	case Type::WorldDistance:
		InteractiveWidget(ImGui::DragFloat("World distance", &worldDistance, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic));
		break;
	case Type::EachFrame:
		break;
	case Type::FixedDeltaTime:
		InteractiveWidget(ImGui::DragFloat("Fixed delta time", &pixelDistance, 0.001f, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic));
		break;
	default:
		assert(false);
	}

	ImGui::End();
}

void Stroke::OnStrokeEmplaceSubscribe(const CallbackRef<StrokeQueryInfo> &callback)
{
	onStrokeEmplace.Subscribe(callback);
}

void Stroke::OnStrokeEmplaceUnsubscribe(const CallbackRef<StrokeQueryInfo> &callback)
{
	onStrokeEmplace.Unsubscribe(callback);
}

void Stroke::SetType(Type newType)
{
	UpdateUnsubscribe();
	type = newType;
	UpdateSubscribe();
}
