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
		AssimpBone(int id, std::string boneName, const aiNodeAnim* channel);

		glm::mat4 GetLocalTransform(float animationTime);
		int GetBoneId() { return m_id; }
		std::string GetBoneName() const { return m_name; }
		glm::mat4 GetBoneOffset() const { return m_offset; }
		void SetBoneOffset(glm::mat4 offset);
	
	private:
		glm::mat4 InterpolatePosition(float animationTime);
		glm::mat4 InterpolateRotation(float animationTime);
		glm::mat4 InterpolateScaling(float animationTime);
		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
		int GetLastPositionIndex(float animationTime);
		int GetLastRotationIndex(float animationTime);
		int GetLastScaleIndex(float animationTime);

		std::vector<KeyPosition> m_positions;
		int m_numPositions;
		std::vector<KeyRotation> m_rotations;
		int m_numRotations;
		std::vector<KeyScale> m_scales;
		int m_numScalings;

		const int m_id;
		const std::string m_name;
		glm::mat4 m_offset = glm::mat4();
	};

}
