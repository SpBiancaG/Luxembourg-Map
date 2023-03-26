#include "Arc.h"

Arc::Arc(Node* n1, Node* n2)
{
	this->m_node1 = n1;
	this->m_node2 = n2;
}

Arc::Arc(Node* n1, Node* n2, Type type) : m_node1(n1), m_node2(n2), m_type(type)
{
}

Arc::Arc(const Node& n1, const Node& n2)
{
	this->m_node1 = new Node(n1);
	this->m_node2 = new Node(n2);
}

Arc::~Arc()
{
	/*delete m_node1;
	delete m_node2;*/
}

Node* const Arc::getFirstNode() const
{
	return m_node1;
}

Node* const Arc::getSecondNode() const
{
	return m_node2;
}

Arc::Type Arc::getType() const
{
	return m_type;
}

void Arc::setFirstPoint(Node* node)
{
	this->m_node1 = node;

}

void Arc::setSecondPoint(Node* node)
{
	m_node2 = node;

}
