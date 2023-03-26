
#include "Node.h"

class Arc
{
public:
	enum class Type : uint16_t
	{
		EWalked,
		EDefault,
		None
	};

	Arc() = default;
	Arc(Node* n1, Node* n2);
	Arc(Node* n1, Node* n2, Type type);
	Arc(const Node& n1, const Node& n2);
	~Arc();

	Node* const getFirstNode()const;
	Node* const getSecondNode()const;
	Type getType()const;

	void setFirstPoint(Node* node);
	void setSecondPoint(Node* node);
private:
	Type m_type;
	Node* m_node1, * m_node2;
};

