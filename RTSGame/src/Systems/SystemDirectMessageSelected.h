#pragma once

#include <Systems\SystemMessage.h>
#include <vector>
#include <utility>

class SystemDirectMessageSelected : public SystemMessage
{
public:
	SystemDirectMessageSelected(std::vector<int>&& entitiesID, )

private:
	const std::vector<int> m_entitiesID;
};