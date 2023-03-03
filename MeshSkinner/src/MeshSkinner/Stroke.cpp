#include "pch.h"
#include "Stroke.h"

Stroke::Stroke(Type type, float pixelDistance, float worldDistance, float fixedIntervalTime)
	: type(type), pixelDistance(pixelDistance), worldDistance(worldDistance), fixedIntervalTime(fixedIntervalTime)
{
}
