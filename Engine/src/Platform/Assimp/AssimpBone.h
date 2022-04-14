#pragma once

#include <assimp/scene.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>

#include "Engine/Util/AssimpUtil.h"

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
		AssimpBone(int id, const aiNodeAnim* channel);
		AssimpBone(int id, glm::mat4 offset, const aiNodeAnim* channel);

		glm::mat4 GetLocalTransform(float animationTime);
		uint32_t GetBoneId() { return m_id; }
		glm::mat4 GetBoneOffset() const { return m_offset; }
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

		const uint32_t m_id;
		glm::mat4 m_offset = glm::mat4();
	};
}
