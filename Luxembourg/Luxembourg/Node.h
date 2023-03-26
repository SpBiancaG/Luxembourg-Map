#pragma once
#include <QPoint>

class Node
{
public:
	enum class Type : uint16_t
	{
		EWalked,
		EStart,
		EFinish,
		EDefault,
		None
	};

	Node();
	Node(const int& nr);
	Node(const QPointF& point, const int& nr);
	Node(const Node& other);
public:

	QPointF getPoint()const;
	void SetPoint(const QPointF& point);
	void SetNumber(const int& newNumber);
	int getNumber()const;

	void SetType(Type type);
	Type GetType()const;

private:

	QPointF m_point;
	int m_number;
	Type m_type;
};

