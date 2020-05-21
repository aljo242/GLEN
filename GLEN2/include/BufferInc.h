#pragma once

#include <cstdint>

namespace GLEN
{
	namespace Shader
	{
		enum class DataType
		{
			None = 0,
			Float,
			Float2,
			Float3,
			Float4,
			Mat3,
			Mat4,
			Int,
			Int2,
			Int3,
			Int4,
			Bool
		};

		constexpr uint32_t DataTypeSize(Shader::DataType type)
		{
			switch (type)
			{
			case Shader::DataType::Float:		return 4;
			case Shader::DataType::Float2:		return 4 * 2;
			case Shader::DataType::Float3:		return 4 * 3;
			case Shader::DataType::Float4:		return 4 * 4;
			case Shader::DataType::Mat3:		return 4 * 3 * 3;
			case Shader::DataType::Mat4:		return 4 * 4 * 5;
			case Shader::DataType::Int:			return 4;
			case Shader::DataType::Int2:		return 4 * 2;
			case Shader::DataType::Int3:		return 4 * 3;
			case Shader::DataType::Int4:		return 4 * 4;
			case Shader::DataType::Bool:		return 1;
			}
		}
	}
}