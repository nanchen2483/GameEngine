#pragma once

#include <assimp/scene.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>

#include "Engine/Util/AssimpUtil.h"

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
			timeStamp = positionKey.mTime;
		}
	};

	struct KeyRotation
	{
		glm::quat orientation;
		float timeStamp;

		KeyRotation(aiQuatKey rotationKey)
		{
			orientation = AssimpUtil::ToGlm(rotationKey.mValue);
			timeStamp = rotationKey.mTime;
		}
	};

	struct KeyScale
	{
		glm::vec3 scale;
		float timeStamp;

		KeyScale(aiVectorKey scaleKey)
		{
			scale = AssimpUtil::ToGlm(scaleKey.mValue);
			timeStamp = scaleKey.mTime;
		}
	};

	class AssimpBone
	{
	public:
		AssimpBone() = default;
		AssimpBone(uint32_t id, const aiNodeAnim* channel);
		AssimpBone(uint32_t id, glm::mat4 offset, const aiNodeAnim* channel);

		glm::mat4 GetLocalTransform(float animationTime);
		const uint32_t GetBoneId() const { return m_id; }
		glm::mat4 GetBoneOffset() const { return m_offset; }

		operator bool() const { return m_id != UNINITIALIZED_BONE_ID; }
	private:
		glm::mat4 InterpolatePosition(float animationTime);
		glm::mat4 InterpolateRotation(float animationTime);
		glm::mat4 InterpolateScaling(float animationTime);
		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
		uint32_t GetLastPositionIndex(float animationTime);
		uint32_t GetLastRotationIndex(float animationTime);
		uint32_t GetLastScaleIndex(float animationTime);

		uint32_t m_numPositions;
		std::vector<KeyPosition> m_positions;
		uint32_t m_numRotations;
		std::vector<KeyRotation> m_rotations;
		uint32_t m_numScalings;
		std::vector<KeyScale> m_scales;

		int m_id = UNINITIALIZED_BONE_ID;
		glm::mat4 m_offset = glm::mat4();
	};
}
