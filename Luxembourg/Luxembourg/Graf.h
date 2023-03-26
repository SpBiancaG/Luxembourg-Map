#pragma once
#include "Node.h"
#include "Arc.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <QPainter>

#include <unordered_map>
#include <unordered_set>
#include <QtXml>
#include <QFile>
#include <QtCore>

class Graf
{

public:
	Graf() = default;
	~Graf();

	const std::vector<Node*>& GetVectorOfNodes()const;
	std::vector<Node*>& GetVectorOfNodes();
	const std::vector<Arc*>& GetVectorOfArcs();
	struct hashNode
	{
		int operator()(Node* node)const
		{
			return (node->getNumber());
		}
	};

	struct hashPair
	{
		int operator()(const std::pair<Node*, int>& n)const
		{
			return (n.first->getNumber());
		}
	};


	const std::unordered_map<Node*, std::unordered_set<int>, hashNode>& getListaAdiacenta();
	std::unordered_map<Node*, std::pair<int, Node*>, hashNode>& getPath();
	std::unordered_map < Node*, std::unordered_set<std::pair<Node*, int>, hashPair >, hashNode>& getNeighbours();;

	void pushNode(Node* node);
	void pushArcs(Arc* arc);
	//void pushArcs(Node* n1, Node* n2);
	void moveArcs(Node* node);
	void DeleteAllArcs();
	void DeleteAllNodes();
	void Read();
	void gasesteRadacina();
	void DFS(Node* x, std::vector<bool>& visited);
	void DrawLineWithArrow(QPainter& painter, QPointF start, QPointF end);

	bool isTree();

	uint16_t Previous(uint16_t node);
	uint16_t getNumberOfArcs(const Node& n1, const Node& n2);

	std::unordered_map < Node*, std::unordered_set<std::pair<Node*, int>, hashPair >, hashNode> m_neighbours;

	int longMax, longMin, latMin, latMax;
private:
	std::unordered_map<Node*, std::pair<int, Node*>, hashNode> m_path;

	std::vector<Node*> m_nodes;
	std::vector<Arc*> m_arcs;

	std::unordered_map<Node*, std::unordered_set<int>, hashNode> m_listaAdiacenta;

private:
	bool isCyclic(int v, bool visited[], bool recall[]);

};

