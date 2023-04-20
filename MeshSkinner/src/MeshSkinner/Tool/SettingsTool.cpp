#include "pch.h"
#include "SettingsTool.h"

#include "Core/Camera/CameraControllerComponent.h"
#include "MeshSkinner/Context.h"

static const char *colorSchemes[] = { "Viridis", "Magma", "Rainbow" };

SettingsTool::SettingsTool(const std::string &toolWindowName, CameraControllerComponent *controller) : Tool(toolWindowName), cameraController(controller)
{
	camera = dynamic_cast<Camera *>(cameraController->GetEntity());

	onDrawAdditionalViewportWidgetsCallback = MakeCallbackNoArgRef([&]() {

		// draw color legend in the top right corner of the viewport

		// setup
		static const std::string legendText = "Weight Color Legend";
		static const auto textSize = ImGui::CalcTextSize(legendText.c_str());
		auto marginSize = ImVec2(30.f, 50.f);
		auto barSize = ImVec2(150.f, textSize.y);
		auto textBarGapSize = 5.f;

		auto windowPos = ImGui::GetWindowPos();
		auto windowSize = ImGui::GetWindowSize();

		// calculate positions
		auto barPos = ImVec2(windowPos.x + windowSize.x - marginSize.x - barSize.x, windowPos.y + marginSize.y);
		auto textPos = ImVec2(barPos.x - textBarGapSize - textSize.x, barPos.y);

		ImU32 colors[5] = {
			MathUtils::ColorToUInt32(Renderer::color000),
			MathUtils::ColorToUInt32(Renderer::color025),
			MathUtils::ColorToUInt32(Renderer::color050),
			MathUtils::ColorToUInt32(Renderer::color075),
			MathUtils::ColorToUInt32(Renderer::color100)
		};

		auto drawList = ImGui::GetWindowDrawList();

		// render the text
		drawList->AddText(textPos, 0xffffffff, legendText.c_str());
		drawList->AddText(ImVec2(barPos.x, barPos.y + textSize.y), 0xffffffff, "0%");
		static const auto size100 = ImGui::CalcTextSize("100%");
		drawList->AddText(ImVec2(barPos.x + barSize.x - size100.x, barPos.y + textSize.y), 0xffffffff, "100%");

		// render the bar
		for (int i = 0; i < 4; ++i)
			drawList->AddRectFilledMultiColor(
				ImVec2(barPos.x + i * (barSize.x / 4), barPos.y),
				ImVec2(barPos.x + (i + 1) * (barSize.x / 4), barPos.y + barSize.y),
				colors[i], colors[i + 1], colors[i + 1], colors[i]
			);
		});
	UserInterface::OnDrawAdditionalViewportWidgetsSubscribe(onDrawAdditionalViewportWidgetsCallback);
}

SettingsTool::~SettingsTool()
{
	UserInterface::OnDrawAdditionalViewportWidgetsUnsubscribe(onDrawAdditionalViewportWidgetsCallback);
}

void SettingsTool::OnUpdateUI()
{
	auto selectedMesh = Context::Get().GetSelectedSkeletalMesh();

	// settings
	ImGui::Begin("Settings");

	InteractiveWidget(ImGui::DragFloat("Mouse sensitivity", &cameraController->mouseSensitivity, 0.0001f, 0.0f, 10.f, "%.3f", ImGuiSliderFlags_ClampOnInput));

	if (InteractiveWidget(ImGui::DragFloat("Bone radius", &Context::Get().boneRadius, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic)) && selectedMesh)
		Renderer::UpdateBoneRadius(selectedMesh, Context::Get().boneRadius);

	if (InteractiveWidget(ImGui::DragFloat("Tip bone length", &Context::Get().tipBoneLength, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic)) && selectedMesh)
	{
		auto &bones = selectedMesh->skeleton->GetBones();
		for (auto &bone : bones)
		{
			if (bone->GetChildren().size() > 0) continue;

			auto &boneMeshes = bone->GetComponents<StaticMeshComponent>();
			if (boneMeshes.size() == 0) continue;

			auto &boneMesh = *boneMeshes.begin();
			if (!boneMesh) continue;

			for (auto &vertex : boneMesh->vertices)
				if (vertex.position.y > glm::epsilon<float>())
					vertex.position.y = Context::Get().tipBoneLength;

			Renderer::UpdateMeshVertices(boneMesh.get());
		}
	}

	InteractiveWidget(ImGui::SliderFloat("Bone alpha", &Renderer::finalAlpha, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput));

	ImGui::Separator();
	ImGui::BeginGroup();
	{
		if (InteractiveWidget(ImGui::ListBox("Color scheme", &colorSchemeIndex, colorSchemes, 3)))
		{
			switch (colorSchemeIndex)
			{
			case 0:
				// viridis
				Renderer::color000 = glm::vec3(0.33f, 0.07f, 0.32f);
				Renderer::color025 = glm::vec3(0.23f, 0.37f, 0.55f);
				Renderer::color050 = glm::vec3(0.18f, 0.60f, 0.63f);
				Renderer::color075 = glm::vec3(0.06f, 0.65f, 0.24f);
				Renderer::color100 = glm::vec3(1.00f, 0.81f, 0.13f);
				break;
			case 1:
				// magma
				Renderer::color000 = glm::vec3(0.00f, 0.00f, 0.01f);
				Renderer::color025 = glm::vec3(0.25f, 0.07f, 0.45f);
				Renderer::color050 = glm::vec3(0.67f, 0.20f, 0.49f);
				Renderer::color075 = glm::vec3(0.98f, 0.51f, 0.37f);
				Renderer::color100 = glm::vec3(0.99f, 0.99f, 0.75f);
				break;
			case 2:
				// rainbow
				Renderer::color000 = glm::vec3(0.f, 0.f, 1.f);
				Renderer::color025 = glm::vec3(0.f, 1.f, 1.f);
				Renderer::color050 = glm::vec3(0.f, 1.f, 0.f);
				Renderer::color075 = glm::vec3(1.f, 1.f, 0.f);
				Renderer::color100 = glm::vec3(1.f, 0.f, 0.f);
				break;

			default:
				assert(false);
			}
		}

		InteractiveWidget(ImGui::ColorEdit3("Colour   0%", glm::value_ptr(Renderer::color000)));
		InteractiveWidget(ImGui::ColorEdit3("Colour  25%", glm::value_ptr(Renderer::color025)));
		InteractiveWidget(ImGui::ColorEdit3("Colour  50%", glm::value_ptr(Renderer::color050)));
		InteractiveWidget(ImGui::ColorEdit3("Colour  75%", glm::value_ptr(Renderer::color075)));
		InteractiveWidget(ImGui::ColorEdit3("Colour 100%", glm::value_ptr(Renderer::color100)));
	}
	ImGui::EndGroup();

    ImGui::End();
}
