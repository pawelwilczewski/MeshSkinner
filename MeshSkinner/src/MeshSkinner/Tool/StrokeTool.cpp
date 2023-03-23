#include "pch.h"
#include "StrokeTool.h"

static constexpr std::array<const char *, 3> strokeUpdateTypeNames = { "Pixel Distance", "World Distance", "Each Frame" };

StrokeQueryInfo::StrokeQueryInfo(bool hitTarget, const glm::vec3 &position, const glm::vec2 &viewportPosition)
	: hitTarget(hitTarget), position(position), viewportPosition(viewportPosition)
{
}

StrokeTool::StrokeTool(const std::string &toolWindowName, const std::function<void(StrokeQueryInfo &)> &queryFunction, Type type, float pixelDistance, float worldDistance, float fixedIntervalTime)
	: Tool(toolWindowName), queryFunction(queryFunction), type(type), pixelDistance(pixelDistance), worldDistance(worldDistance), fixedIntervalTime(fixedIntervalTime)
{
	onStrokeUpdateCallback = MakeCallbackNoArgRef([&]() { OnStrokeUpdate(); });
	onMouseButtonUpCallback = MakeCallbackRef<int>([&](int button) { OnMouseButtonUp(button); });
	onStrokeUpdateVec2CallbackWrapper = MakeCallbackRef<glm::vec2>([&](const glm::vec2 &) { OnStrokeUpdate(); });

	Input::OnMouseButtonReleasedSubscribe(onMouseButtonUpCallback);

	UpdateSubscribe();
}

StrokeTool::~StrokeTool()
{
	UpdateUnsubscribe();

	Input::OnMouseButtonReleasedUnsubscribe(onMouseButtonUpCallback);
}

void StrokeTool::OnStrokeUpdate()
{
	if (!Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || !UserInterface::GetClickedInViewport() || Input::IsKeyPressed(KEY_LEFT_CONTROL))
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
			shouldAttemptEmplace = glm::distance(lastWorldPosition, query.position) >= worldDistance;
			break;

		case Type::EachFrame:
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
		lastWorldPosition = query.position;
	}
}

void StrokeTool::OnMouseButtonUp(int button)
{
	initialContact = true;
}

void StrokeTool::UpdateSubscribe() const
{
	switch (type)
	{
	case Type::EachFrame:
		Application::OnUpdateSubscribe(onStrokeUpdateCallback);
		break;
	case Type::PixelDistance:
		Input::OnMouseMovedSubscribe(onStrokeUpdateVec2CallbackWrapper); // TODO: reconsider changing to Update cos lagging because single-threaded
		break;
	case Type::WorldDistance:
		Application::OnUpdateSubscribe(onStrokeUpdateCallback);
		break;
	default:
		assert(false);
	}
}

void StrokeTool::UpdateUnsubscribe() const
{
	switch (type)
	{
	case Type::EachFrame:
		Application::OnUpdateUnsubscribe(onStrokeUpdateCallback);
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

void StrokeTool::OnUpdateUI()
{
	ImGui::Begin(toolWindowName.c_str());

	Type tempType = type;
	if (InteractiveWidget(ImGui::ListBox("Stroke update type", (int *)(&tempType), strokeUpdateTypeNames.data(), strokeUpdateTypeNames.size())))
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
	default:
		assert(false);
	}

	ImGui::End();
}

void StrokeTool::OnStrokeEmplaceSubscribe(const CallbackRef<StrokeQueryInfo> &callback)
{
	onStrokeEmplace.Subscribe(callback);
}

void StrokeTool::OnStrokeEmplaceUnsubscribe(const CallbackRef<StrokeQueryInfo> &callback)
{
	onStrokeEmplace.Unsubscribe(callback);
}

void StrokeTool::SetType(Type newType)
{
	UpdateUnsubscribe();
	type = newType;
	UpdateSubscribe();
}
