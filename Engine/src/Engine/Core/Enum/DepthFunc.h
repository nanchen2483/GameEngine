#pragma once


namespace Engine
{
	enum class DepthFunc
	{
		UNKNOWN = 0,
		NEVER, // Never passes.
		LESS, // Passes if the incoming z value is less than the stored z value. This is the default value.
		LEQUAL, // Passes if the incoming z value is less than or equal to the stored z value.
		EQUAL, // Passes if the incoming z value is equal to the stored z value.
		GREATER, // Passes if the incoming z value is greater than the stored z value.
		NOTEQUAL, // Passes if the incoming z value is not equal to the stored z value.
		GEQUAL, // Passes if the incoming z value is greater than or equal to the stored z value.
		ALWAYS, // Always passes.
	};
}
