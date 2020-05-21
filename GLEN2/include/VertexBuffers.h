#pragma once
#include <vector>

namespace GLEN
{
	class VertexBuffers
	{
	public:
		VertexBuffers(const size_t size);

		void SetVBO(const int location);
		void PushBack();
		void EnableBuffers(const int location) const;
		void EnableBuffers(const std::vector<int>& locations) const;



		unsigned int GetLabel(const int location) const {return m_VBOs[location];}
		

	private:
		std::vector<unsigned int> m_VBOs;

	};
}