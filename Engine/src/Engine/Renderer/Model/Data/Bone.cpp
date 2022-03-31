#include "enginepch.h"

#include "Bone.h"
#include "Engine\Util\AssimpUtil.h"

namespace Engine::ModelData
{
	Bone::Bone(int id, std::string boneName, const aiNodeAnim* channel)
		: m_id(id), m_name(boneName)
	{
		m_numPositions = channel->mNumPositionKeys;
		for (int positionIndex = 0; positionIndex < m_numPositions; ++positionIndex)
		{
			m_positions.push_back(KeyPosition(channel->mPositionKeys[positionIndex]));
		}

		m_numRotations = channel->mNumRotationKeys;
		for (int rotationIndex = 0; rotationIndex < m_numRotations; ++rotationIndex)
		{
			m_rotations.push_back(KeyRotation(channel->mRotationKeys[rotationIndex]));
		}

		m_numScalings = channel->mNumScalingKeys;
		for (int keyIndex = 0; keyIndex < m_numScalings; ++keyIndex)
		{
			m_scales.push_back(KeyScale(channel->mScalingKeys[keyIndex]));
		}
	}
	
	glm::mat4 Bone::GetLocalTransform(float animationTime)
	{
		glm::mat4 translation = InterpolatePosition(animationTime);
		glm::mat4 rotation = InterpolateRotation(animationTime);
		glm::mat4 scale = InterpolateScaling(animationTime);
		return translation * rotation * scale;
	}

	void Bone::SetBoneOffset(glm::mat4 offset)
	{
		m_offset = offset;
	}

	glm::mat4 Bone::InterpolatePosition(float animationTime)
	{
		if (m_numPositions == 1)
		{
			return glm::translate(glm::mat4(1.0f), m_positions[0].position);
		}

		int lastPositionIndex = GetLastPositionIndex(animationTime);
		KeyPosition lastPosition = m_positions[lastPositionIndex];
		KeyPosition nextPosition = m_positions[lastPositionIndex + 1];
		float scaleFactor = GetScaleFactor(lastPosition.timeStamp, nextPosition.timeStamp, animationTime);
		glm::vec3 finalPosition = glm::mix(lastPosition.position, nextPosition.position, scaleFactor);
		
		return glm::translate(glm::mat4(1.0f), finalPosition);
	}
	
	glm::mat4 Bone::InterpolateRotation(float animationTime)
	{
		if (m_numRotations == 1)
		{
			auto rotation = glm::normalize(m_rotations[0].orientation);
			return glm::toMat4(rotation);
		}

		int lastRotationIndex = GetLastRotationIndex(animationTime);
		KeyRotation lastRotation = m_rotations[lastRotationIndex];
		KeyRotation nextRotation = m_rotations[lastRotationIndex + 1];
		float scaleFactor = GetScaleFactor(lastRotation.timeStamp, nextRotation.timeStamp, animationTime);
		glm::quat finalRotation = glm::slerp(lastRotation.orientation, nextRotation.orientation, scaleFactor);
		finalRotation = glm::normalize(finalRotation);

		return glm::toMat4(finalRotation);
	}
	
	glm::mat4 Bone::InterpolateScaling(float animationTime)
	{
		if (m_numScalings == 1)
		{
			return glm::scale(glm::mat4(1.0f), m_scales[0].scale);
		}

		int lastScaleIndex = GetLastScaleIndex(animationTime);
		KeyScale lastScale = m_scales[lastScaleIndex];
		KeyScale nextScale = m_scales[lastScaleIndex + 1];
		float scaleFactor = GetScaleFactor(lastScale.timeStamp, nextScale.timeStamp, animationTime);
		glm::vec3 finalScale = glm::mix(lastScale.scale, nextScale.scale, scaleFactor);

		return glm::scale(glm::mat4(1.0f), finalScale);
	}
	
	float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;
		
		return midWayLength / framesDiff;
	}

	int Bone::GetLastPositionIndex(float animationTime)
	{
		for (int index = 1; index < m_numPositions; ++index)
		{
			if (animationTime < m_positions[index].timeStamp)
			{
				return index - 1;
			}
		}

		ENGINE_CORE_ASSERT(0, "Poisition index not found! AnimationTime {0}, Duration: {1}", animationTime);
	}
	
	int Bone::GetLastRotationIndex(float animationTime)
	{
		for (int index = 1; index < m_numRotations; ++index)
		{
			if (animationTime < m_rotations[index].timeStamp)
			{
				return index - 1;
			}
		}

		ENGINE_CORE_ASSERT(0, "Rotation index not found! AnimationTime {0}, Duration: {1}", animationTime);
	}
	
	int Bone::GetLastScaleIndex(float animationTime)
	{
		for (int index = 1; index < m_numScalings; ++index)
		{
			if (animationTime < m_scales[index].timeStamp)
			{
				return index - 1;
			}
		}

		ENGINE_CORE_ASSERT(0, "Scale index not found! AnimationTime {0}, Duration: {1}", animationTime);
	}
}
