#pragma once
#include "Platform/Util/AssimpUtil.h"

#include <assimp/scene.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>

#define UNINITIALIZED_BONE_ID -1

namespace Engine
{
	struct KeyPosition
	{
		glm::vec3 position;
		float timeStamp;

		KeyPosition(aiVectorKey positionKey)
		{
			position = AssimpUtil::ToGlm(positionKey.mValue);
			timeStamp = (float)positionKey.mTime;
		}
	};

	struct KeyRotation
	{
		glm::quat orientation;
		float timeStamp;

		KeyRotation(aiQuatKey rotationKey)
		{
			orientation = AssimpUtil::ToGlm(rotationKey.mValue);
			timeStamp = (float)rotationKey.mTime;
		}
	};

	struct KeyScale
	{
		glm::vec3 scale;
		float timeStamp;

		KeyScale(aiVectorKey scaleKey)
		{
			scale = AssimpUtil::ToGlm(scaleKey.mValue);
			timeStamp = (float)scaleKey.mTime;
		}
	};

	class AssimpBone
	{
	public:
		AssimpBone() = default;
		AssimpBone(uint32_t id, glm::mat4 offset, const aiNodeAnim* channel);

		const glm::mat4 GetLocalTransform(float animationTime) const;
		const uint32_t GetId() const { return m_id; }
		const glm::mat4 GetOffset() const { return m_offset; }

		operator bool() const { return m_id != UNINITIALIZED_BONE_ID; }
	private:
		const glm::mat4 InterpolatePosition(const float animationTime) const;
		const glm::mat4 InterpolateRotation(const float animationTime) const;
		const glm::mat4 InterpolateScaling(const float animationTime) const;
		const float GetScaleFactor(const float lastTimeStamp, const float nextTimeStamp, const float animationTime) const;
		const uint32_t GetLastPositionIndex(const float animationTime) const;
		const uint32_t GetLastRotationIndex(const float animationTime) const;
		const uint32_t GetLastScaleIndex(const float animationTime) const;

		uint32_t m_numPositions = 0;
		std::vector<KeyPosition> m_positions;
		uint32_t m_numRotations = 0;
		std::vector<KeyRotation> m_rotations;
		uint32_t m_numScalings = 0;
		std::vector<KeyScale> m_scales;

		int m_id = UNINITIALIZED_BONE_ID;
		glm::mat4 m_offset = glm::mat4();
	};
}
