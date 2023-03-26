#include "Graf.h"
#include <QPointF>
#include <QTextStream>
#include<fstream>
#include <QtXml>
Graf::~Graf()
{
	for (Node* node : m_nodes)
		delete node;
	m_nodes.clear();

	for (Arc* arc : m_arcs)
		delete arc;
	m_arcs.clear();

}


std::vector<Node*>& Graf::GetVectorOfNodes()
{
	return m_nodes;
}

const std::vector<Arc*>& Graf::GetVectorOfArcs()
{
	return m_arcs;
}

const std::unordered_map<Node*, std::unordered_set<int>, Graf::hashNode>& Graf::getListaAdiacenta()
{
	return m_listaAdiacenta;
}

std::unordered_map<Node*, std::pair<int, Node*>, Graf::hashNode>& Graf::getPath()
{
	return m_path;
}

std::unordered_map<Node*, std::unordered_set<std::pair<Node*, int>, Graf::hashPair>, Graf::hashNode>& Graf::getNeighbours()
{
	return m_neighbours;
}

void Graf::pushNode(Node* node)
{
	m_nodes.push_back(node);
}

void Graf::pushArcs(Arc* arc)
{
	m_arcs.push_back(arc);

	m_listaAdiacenta[m_nodes[arc->getFirstNode()->getNumber()]].insert(arc->getSecondNode()->getNumber());

	for (auto x : m_listaAdiacenta)
	{
		std::cout << x.first->getNumber() << "->";
		for (auto y : x.second)
			std::cout << y << " ";
		std::cout << std::endl;
	}
	std::cout << "\n";
}

//void Graf::pushArcs(Node* n1, Node* n2)
//{
//	m_arcs.push_back(new Arc(n1, n2));
//}

void Graf::moveArcs(Node* node)
{
	for (Arc* arc : m_arcs)
	{
		if (arc->getFirstNode()->getNumber() == node->getNumber())
			arc->setFirstPoint(node);
		else
			if (arc->getSecondNode()->getNumber() == node->getNumber())
				arc->setSecondPoint(node);
	}
}

void Graf::DeleteAllArcs()
{
	for (Arc* arc : m_arcs)
		delete arc;
	m_arcs.clear();
	//m_listaAdiacenta.clear();
}

void Graf::DeleteAllNodes()
{
	for (Node* node : m_nodes)
		delete node;
	m_nodes.clear();
}

void Graf::Read()
{
	QDomDocument xmlBOM;
	longMin = INT_MAX;
	longMax = 0;
	latMin = INT_MAX;
	latMax = 0;

    QFile f("C:/Users/User/Documents/Qt Projects/Luxembourg/Luxembourg");
	if (!f.open(QIODevice::ReadOnly))
	{
		std::cout << "Error while loading file" << std::endl;
		return;
	}
	xmlBOM.setContent(&f);
	f.close();

	QDomElement root = xmlBOM.documentElement();
	QDomElement Component = root.firstChild().toElement();

	QDomNodeList nodes = root.elementsByTagName("node");

	while (!Component.isNull())
	{
		if (Component.tagName() == "nodes")
		{

			QDomElement Child = Component.firstChild().toElement();

			int id, x, y;

			while (!Child.isNull())
			{
				//std::cout << Child.tagName().toStdString()<<"\n";

				//std::cout << Child.attribute("id").toStdString() << "\n";
				id = Child.attribute("id").toInt();

				//std::cout << Child.attribute("latitude").toStdString() << "\n"; //x
				x = Child.attribute("latitude").toInt();

				//std::cout << Child.attribute("longitude").toStdString() << "\n"; //y
				y = Child.attribute("longitude").toInt();

				if (x > latMax)
					latMax = x;
				else
					if (x < latMin)
						latMin = x;

				if (y > longMax)
					longMax = y;
				else
					if (y < longMin)
						longMin = y;

				Node* node = new Node(QPointF(x, y), id);
				m_nodes.emplace_back(node);
				m_path[node].first = INT_MAX;
				m_path[node].second = nullptr;

				Child = Child.nextSibling().toElement();
			}

		}
		else
			if (Component.tagName() == "arcs")
			{
				QDomElement Child = Component.firstChild().toElement();

				int from, to, cost;

				while (!Child.isNull())
				{

					from = Child.attribute("from").toInt();

					to = Child.attribute("to").toInt();

					cost = Child.attribute("length").toInt();

					m_neighbours[m_nodes[from]].insert(std::make_pair(m_nodes[to], cost));

					m_arcs.emplace_back(new Arc(m_nodes[from], m_nodes[to]));

					Child = Child.nextSibling().toElement();
				}

			}
		Component = Component.nextSibling().toElement();
	}

}

void Graf::gasesteRadacina()
{
	std::vector<bool>visited(m_nodes.size(), false);

	int i = 0, v;

	for (auto& index : m_nodes) {
		if (!visited[index->getNumber()])
		{
			DFS(index, visited);
			v = i;
		}
		i++;
	}

	int root = v;

	for (int j = 0; j < m_nodes.size(); j++)
		visited[j] = false;

	DFS(m_nodes[v], visited);

	for (auto& index : m_nodes)
	{
		if (!visited[index->getNumber()])
		{
			root = -1;
			break;
		}
	}

	if (root == -1)
	{
		std::cout << "Graful nu are radacina!";
	}
	else
	{
		std::cout << "Radacina grafului este " << root << "\n";
	}
}

void Graf::DFS(Node* x, std::vector<bool>& visited)
{
	visited[x->getNumber()] = true;

	for (auto& index : m_listaAdiacenta[x])
	{
		if (!visited[index])
			DFS(m_nodes[index], visited);
	}
}

bool Graf::isTree()
{
	int n = m_nodes.size();

	bool* visited = new bool[n];
	bool* recStack = new bool[n];

	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
		recStack[i] = false;
	}

	for (int i = 0; i < n; i++)
		if (isCyclic(i, visited, recStack))
			return true;

	return false;
}

bool Graf::isCyclic(int v, bool visited[], bool recall[])
{
	if (visited[v] == false)
	{
		visited[v] = true;
		recall[v] = true;

		for (int node : m_listaAdiacenta[m_nodes[v]])
		{
			if (!visited[m_nodes[node]->getNumber()] && isCyclic(m_nodes[node]->getNumber(),
				visited, recall))
				return true;
			else if (recall[m_nodes[node]->getNumber()])
				return true;
		}

	}

	recall[v] = false;
	return false;
}

void Graf::DrawLineWithArrow(QPainter& painter, QPointF start, QPointF end)
{
	QLineF lineBetweenNode(start, end);
	painter.drawLine(start, end);

	QLineF line;

	//painter.setBrush(QBrush::set)
	line.setP1(end);
	line.setLength(20);

	line.setAngle(lineBetweenNode.angle() - 160);
	painter.drawLine(line);

	line.setAngle(lineBetweenNode.angle() + 160);
	painter.drawLine(line);
}

uint16_t Graf::getNumberOfArcs(const Node& n1, const Node& n2)
{
	uint16_t count = 0;
	for (Arc* arc : m_arcs)
	{
		if ((arc->getFirstNode()->getNumber() == n1.getNumber() && arc->getSecondNode()->getNumber() == n2.getNumber())
			|| (arc->getFirstNode()->getNumber() == n2.getNumber() && arc->getSecondNode()->getNumber() == n1.getNumber()))
			++count;

	}
	return count;
}
