#include "enginepch.h"
#include "AssimpBone.h"

namespace Engine
{
	AssimpBone::AssimpBone(uint32_t id, glm::mat4 offset, const aiNodeAnim* channel)
		: m_id(id), m_offset(offset)
	{
		m_numPositions = channel->mNumPositionKeys;
		m_positions.reserve(channel->mNumPositionKeys);
		for (uint32_t positionIndex = 0; positionIndex < m_numPositions; positionIndex++)
		{
			m_positions.push_back(KeyPosition(channel->mPositionKeys[positionIndex]));
		}

		m_numRotations = channel->mNumRotationKeys;
		m_rotations.reserve(channel->mNumRotationKeys);
		for (uint32_t rotationIndex = 0; rotationIndex < m_numRotations; rotationIndex++)
		{
			m_rotations.push_back(KeyRotation(channel->mRotationKeys[rotationIndex]));
		}

		m_numScalings = channel->mNumScalingKeys;
		m_scales.reserve(channel->mNumScalingKeys);
		for (uint32_t keyIndex = 0; keyIndex < m_numScalings; keyIndex++)
		{
			m_scales.push_back(KeyScale(channel->mScalingKeys[keyIndex]));
		}
	}
	
	const glm::mat4 AssimpBone::GetLocalTransform(const float animationTime) const
	{
		const glm::mat4 translation = InterpolatePosition(animationTime);
		const glm::mat4 rotation = InterpolateRotation(animationTime);
		const glm::mat4 scale = InterpolateScaling(animationTime);
		
		return translation * rotation * scale;
	}

	const glm::mat4 AssimpBone::InterpolatePosition(const float animationTime) const
	{
		if (m_numPositions == 1)
		{
			return glm::translate(glm::mat4(1.0f), m_positions[0].position);
		}

		uint32_t lastPositionIndex = GetLastPositionIndex(animationTime);
		const KeyPosition& lastPosition = m_positions[lastPositionIndex];
		const KeyPosition& nextPosition = m_positions[lastPositionIndex + 1];
		const float scaleFactor = GetScaleFactor(lastPosition.timeStamp, nextPosition.timeStamp, animationTime);
		const glm::vec3& finalPosition = glm::mix(lastPosition.position, nextPosition.position, scaleFactor);
		
		return glm::translate(glm::mat4(1.0f), finalPosition);
	}
	
	const glm::mat4 AssimpBone::InterpolateRotation(const float animationTime) const
	{
		if (m_numRotations == 1)
		{
			glm::quat rotation = glm::normalize(m_rotations[0].orientation);
			return glm::toMat4(rotation);
		}

		uint32_t lastRotationIndex = GetLastRotationIndex(animationTime);
		const KeyRotation& lastRotation = m_rotations[lastRotationIndex];
		const KeyRotation& nextRotation = m_rotations[lastRotationIndex + 1];
		const float scaleFactor = GetScaleFactor(lastRotation.timeStamp, nextRotation.timeStamp, animationTime);
		glm::quat finalRotation = glm::slerp(lastRotation.orientation, nextRotation.orientation, scaleFactor);
		finalRotation = glm::normalize(finalRotation);

		return glm::toMat4(finalRotation);
	}
	
	const glm::mat4 AssimpBone::InterpolateScaling(const float animationTime) const
	{
		if (m_numScalings == 1)
		{
			return glm::scale(glm::mat4(1.0f), m_scales[0].scale);
		}

		uint32_t lastScaleIndex = GetLastScaleIndex(animationTime);
		const KeyScale& lastScale = m_scales[lastScaleIndex];
		const KeyScale& nextScale = m_scales[lastScaleIndex + 1];
		const float scaleFactor = GetScaleFactor(lastScale.timeStamp, nextScale.timeStamp, animationTime);
		const glm::vec3& finalScale = glm::mix(lastScale.scale, nextScale.scale, scaleFactor);

		return glm::scale(glm::mat4(1.0f), finalScale);
	}
	
	const float AssimpBone::GetScaleFactor(const float lastTimeStamp, const float nextTimeStamp, const float animationTime) const
	{
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;
		
		return midWayLength / framesDiff;
	}

	const uint32_t AssimpBone::GetLastPositionIndex(const float animationTime) const
	{
		for (uint32_t index = 1; index < m_numPositions; ++index)
		{
			if (animationTime < m_positions[index].timeStamp)
			{
				return index - 1;
			}
		}

		ENGINE_CORE_ASSERT(false, "Poisition index not found! AnimationTime {0}, Duration: {1}", animationTime);
	}
	
	const uint32_t AssimpBone::GetLastRotationIndex(const float animationTime) const
	{
		for (uint32_t index = 1; index < m_numRotations; ++index)
		{
			if (animationTime < m_rotations[index].timeStamp)
			{
				return index - 1;
			}
		}

		ENGINE_CORE_ASSERT(false, "Rotation index not found! AnimationTime {0}, Duration: {1}", animationTime);
	}
	
	const uint32_t AssimpBone::GetLastScaleIndex(const float animationTime) const
	{
		for (uint32_t index = 1; index < m_numScalings; ++index)
		{
			if (animationTime < m_scales[index].timeStamp)
			{
				return index - 1;
			}
		}

		ENGINE_CORE_ASSERT(0, "Scale index not found! AnimationTime {0}, Duration: {1}", animationTime);
	}
}
