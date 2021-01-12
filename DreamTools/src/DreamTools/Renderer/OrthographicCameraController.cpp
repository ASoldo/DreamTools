#include "dtpch.h"
#include "OrthographicCameraController.h"
#include "DreamTools/Core/Input.h"
#include "DreamTools//Core/KeyCodes.h"

namespace DreamTools
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio)
		,m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel })
		,m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top)
		,m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		DT_PROFILE_FUNCTION();

		//Camera Movement Start
		if (Input::IsKeyPressed(DreamTools::Key::A))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(DreamTools::Key::D))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(DreamTools::Key::W))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(DreamTools::Key::S))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}
		//Camera Movement End
		if (m_Rotation)
		{
			//Camera Rotate Start
			if (Input::IsKeyPressed(DreamTools::Key::Q))
			{
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}
			else if (Input::IsKeyPressed(DreamTools::Key::E))
			{
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			//Camera Rotate End

			m_Camera.SetRotation(m_CameraRotation);
		}
		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel * 0.2f;
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		DT_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(DT_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(DT_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		DT_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetOffsetY() * 0.1f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.05f);
		CalculateView();
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		DT_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		CalculateView();
		return false;
	}
}