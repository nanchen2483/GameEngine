#pragma once

namespace Engine
{
	enum class DepthFunc
	{
		Never = 1, // Never passes.
		Less, // Passes if the incoming z value is less than the stored z value. This is the default value.
		Lequal, // Passes if the incoming z value is less than or equal to the stored z value.
		Equal, // Passes if the incoming z value is equal to the stored z value.
		Greater, // Passes if the incoming z value is greater than the stored z value.
		NotEqual, // Passes if the incoming z value is not equal to the stored z value.
		Gequal, // Passes if the incoming z value is greater than or equal to the stored z value.
		Always, // Always passes.
	};
}
