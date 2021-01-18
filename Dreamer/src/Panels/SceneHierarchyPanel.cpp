//#include "dtpch.h"
#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"
#include "DreamTools/Scene/Components.h"

namespace DreamTools
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy:");

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}

		//Right Click PopUpMenu Start - Right click on a blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity("New Entity");
				
			}
			ImGui::EndPopup();
		}
		//Right Click PopUpMenu End

		ImGui::End();

		ImGui::Begin("Properties:");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
			if (ImGui::Button("AddComponent"))
			{
				ImGui::OpenPopup("AddComponent");
			}
			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		/*auto& tc = entity.GetComponent<TagComponent>();
		ImGui::Text("%s", tc.Tag.c_str());*/

		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		bool entityDeleted = false;
		//Right Click PopUpMenu Start - Right click on a blank space
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			/*ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)9812732, flags, tag.c_str());
			if (opened)
			{
				ImGui::TreePop();
			}*/
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
			{
				m_SelectionContext = {};
			}
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			static char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		
		if (entity.HasComponent<TransformComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");

			if (open)
			{
				auto& tc = entity.GetComponent<TransformComponent>();
			/*	ImGui::DragFloat3("Position", glm::value_ptr(tc.Translation), 0.5f);
				ImGui::DragFloat3("Rotation", glm::value_ptr(tc.Rotation), 0.5f);
				ImGui::DragFloat3("Scale", glm::value_ptr(tc.Scale), 0.5f);*/
				DrawVec3Control("Position", tc.Translation);
				
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);

				DrawVec3Control("Scale", tc.Scale, 1.0f);
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeString[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeString[i];
						if (ImGui::Selectable(projectionTypeString[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeString[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspCamera = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("FOV:", &perspCamera))
					{
						camera.SetPerspectiveVerticalFOV(glm::radians(perspCamera));
					}

					float perspNearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip:", &perspNearClip))
					{
						camera.SetPerspectiveNearClip(perspNearClip);
					}

					float perspFarClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip:", &perspFarClip))
					{
						camera.SetPerspectiveFarClip(perspFarClip);
					}
				}
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoCamera = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size:", &orthoCamera))
					{
						camera.SetOrthographicSize(orthoCamera);
					}

					float nearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip:", &nearClip))
					{
						camera.SetOrthographicNearClip(nearClip);
					}

					float farClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip:", &farClip))
					{
						camera.SetOrthographicFarClip(farClip);
					}

					ImGui::Checkbox("Fixed Aspect", &cameraComponent.FixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 30.0f);
			if (ImGui::Button("*", ImVec2{25, 15}))
			{
				ImGui::OpenPopup("ComponentSettings");

			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}
			auto& color = entity.GetComponent<SpriteRendererComponent>().Color;
			if (open)
			{
				ImGui::ColorEdit4("Tint", glm::value_ptr(color));
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemmoveComponent<SpriteRendererComponent>();
			}
		}
	}
}