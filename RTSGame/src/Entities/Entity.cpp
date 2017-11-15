#include "Entity.h"

Entity::Entity(std::string&& name, int ID)
	: m_name(std::move(name)),
	m_ID(ID),
	m_components()
{}