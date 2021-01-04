#pragma once

namespace DreamTools
{
	class Timestep
	{
	public:
		Timestep(float time) : m_Time(time)
		{

		}

		//Cast operator (Float)
		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMillieconds() const { return m_Time * 1000.0f; }
		float GetTimestep(float& time) { return m_Time = time; }
	private:
		float m_Time = 0.0f;
	};
}
