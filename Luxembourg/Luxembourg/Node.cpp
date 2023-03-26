#include "Node.h"

Node::Node(const int& nr) : m_number(nr)
{
}

Node::Node() :
	m_number(-1),
	m_type(Type::EDefault)
{

}
Node::Node(const QPointF& point, const int& nr) :m_point(point), m_number(nr), 
m_type(Type::EDefault)

{
}


Node::Node(const Node& other)
{
	m_number = other.m_number;
	m_point = other.m_point;
}

QPointF Node::getPoint() const
{
	return m_point;
}

void Node::SetPoint(const QPointF& point)
{
	m_point = point;
}

void Node::SetNumber(const int& newNumber)
{
	m_number = newNumber;
}

int Node::getNumber() const
{
	return m_number;
}

void Node::SetType(Type type)
{
	m_type = type;
}

Node::Type Node::GetType() const
{
	return m_type;
}
