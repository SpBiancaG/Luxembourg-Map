#include "Luxembourg.h"
#include<QPaintEvent>
#include<QPushButton>
#include <QMouseEvent>

Luxembourg::Luxembourg(QWidget* parent)
	: QMainWindow(parent),
	m_drawByHand(false),
	luxembourg(false),
	IsTree(true)
{
    ui.setupUi(this);
	connect(ui.draw, &QPushButton::clicked, this, &Luxembourg::on_draw_clicked);
	connect(ui.luxembourg, &QPushButton::clicked, this, &Luxembourg::on_luxembourg_clicked);
	connect(ui.root, &QPushButton::clicked, this, &Luxembourg::on_root_clicked);
	connect(ui.Dijkstra, &QPushButton::clicked, this, &Luxembourg::on_Dijkstra_clicked);
}

void Luxembourg::paintEvent(QPaintEvent* event)
{
	QPainter p(this);

	if (m_drawByHand)
	{
		DrawByHand(p);
		if (!IsTree)
		{
			QPointF point = ui.root->pos();
			point.setY(point.y() + 70);
			p.drawText(point, QString("Graf incompatibil"));
		}
	}
	else
		if (luxembourg)
			drawLuxembourg(p);
}

void Luxembourg::mouseReleaseEvent(QMouseEvent* e)
{
	if (m_drawByHand)
	{
		drawNode = false;
		drawArc = false;
		if (e->button() == Qt::RightButton)
		{

			if (isValid(point))
			{
				Node* n = new Node(e->localPos(), graf.GetVectorOfNodes().size());
				graf.pushNode(n);
				drawNode = true;
				firstNode = Node();
				secondNode = Node();

				update();
			}
		}
		else
			if (e->button() == Qt::LeftButton)
			{
				if (pressedNode)
				{
					if (!isValid(pressedNode->getPoint()))
					{
						pressedNode->SetPoint(this->point);
						graf.moveArcs(pressedNode);
						firstNode = Node();
						secondNode = Node();
						update();
					}
				}


				QPointF p = e->localPos();

				const std::vector<Node*>& noduri = graf.GetVectorOfNodes();

				Node foundNode;
				foundNode.SetNumber(-1);

				for (Node* n : noduri)
				{
					if (fabs(n->getPoint().x() - p.x()) < 20 && fabs(n->getPoint().y() - p.y()) < 20)
					{
						foundNode = *n;
						break;
					}
				}

				if (foundNode.getNumber() == -1)
				{
					firstNode = Node();
					secondNode = Node();
					update();
					return;
				}

				if (firstNode.getNumber() == -1)
				{
					firstNode = foundNode;
					return;
				}
				else if (firstNode.getNumber() != foundNode.getNumber())
				{
					uint16_t nrOfArcsBetweenTheNodes = graf.getNumberOfArcs(firstNode, foundNode);

					if (nrOfArcsBetweenTheNodes < 2)
					{
						secondNode = foundNode;
						graf.pushArcs(new Arc(firstNode, secondNode));
						firstNode = Node();
						secondNode = Node();
						drawArc = true;
						update();

					}
					else
					{
						firstNode = Node();
						secondNode = Node();
						update();
					}
				}
			}
	}
	else
		if (luxembourg)
		{
			QPointF p = e->localPos();

			const std::vector<Node*>& noduri = graf.GetVectorOfNodes();

			Node foundNode;
			foundNode.SetNumber(-1);
			if (firstNode.getNumber() == -1 || secondNode.getNumber() == -1)
			{

				for (Node* n : noduri)
				{
					float x = width() / ((graf.longMax - graf.longMin) / divider) * ((n->getPoint().y() - graf.longMin) / divider);
					x += xSpace;
					float y = height() / ((graf.latMax - graf.latMin) / divider) * ((graf.latMax - n->getPoint().x()) / divider);
					y += ySpace;

					if (fabs(x - p.x()) <= 5 && fabs(y - p.y()) < 5)
					{
						foundNode = *n;
						break;
					}
				}

				if (foundNode.getNumber() == -1)
				{
					firstNode = Node();
					secondNode = Node();
					update();
					return;
				}

				if (firstNode.getNumber() == -1)
				{
					firstNode = foundNode;
					graf.GetVectorOfNodes().at(firstNode.getNumber())->SetType(Node::Type::EStart);
					std::cout << "Nod start ales: " << firstNode.getNumber() << "\n";
					update();
					return;
				}
				else if (firstNode.getNumber() != foundNode.getNumber())
				{
					secondNode = foundNode;
					graf.GetVectorOfNodes().at(secondNode.getNumber())->SetType(Node::Type::EStart);
					ui.Dijkstra->setEnabled(true);
					std::cout << "Nod finish ales: " << secondNode.getNumber() << "\n";
					update();
				}
			}
		}
}

void Luxembourg::mousePressEvent(QMouseEvent* e)
{
	if (m_drawByHand)
	{
		if (e->button() == Qt::LeftButton)
		{
			for (Node* node : graf.GetVectorOfNodes())
			{
				if (fabs(e->x() - node->getPoint().x()) < 30 && fabs(e->y() -
					node->getPoint().y()) < 30) {
					pressedNode = node;
					this->point = node->getPoint();
					return;
				}
			}
		}
		pressedNode = nullptr;
	}
}

void Luxembourg::mouseMoveEvent(QMouseEvent* e)
{
	if (m_drawByHand)
	{
		if (!(e->buttons() & Qt::LeftButton))
			return;
		if (pressedNode)
		{
			pressedNode->SetPoint(e->pos());
			graf.moveArcs(pressedNode);
			update();
		}
	}
}

void Luxembourg::Dijkstra()
{
	std::chrono::time_point<std::chrono::system_clock> m_StartTime;
	std::chrono::time_point<std::chrono::system_clock> m_EndTime;

	auto compare = [&](std::pair < Node*, int>& n1, std::pair<Node*, int>& n2) {return (n1).second >
		n2.second; };

	std::priority_queue<std::pair<Node*, int>, std::vector<std::pair<Node*, int>>, decltype(compare)> q(compare);

	std::unordered_map<Node*, std::pair<int, Node*>, Graf::hashNode>& path = graf.getPath();
	std::vector<Node*>& nodes = graf.GetVectorOfNodes();

	Node* first = nodes[firstNode.getNumber()];
	Node* second = nodes[secondNode.getNumber()];

	std::vector<bool> seen(nodes.size(), false);
	path[first].first = 0;
	q.push({ first, 0 });

	m_StartTime = std::chrono::system_clock::now();

	while (!q.empty())
	{
		Node* start = q.top().first;
		q.pop();

		if (seen[start->getNumber()])
			continue;

		seen[start->getNumber()] = true;

		if (start == second)
			break;

		for (auto& end : graf.m_neighbours[start])
		{
			Node* endNode = end.first;
			int weight = end.second;

			if (!seen[endNode->getNumber()] && path[start].first + weight < path[endNode].first)
			{
				path[endNode].first = path[start].first + weight;
				path[endNode].second = start;
				q.push({ endNode, path[start].first + weight });
			}
		}
	}


	Node* aux = second;
	while (path[aux].second != nullptr)
	{
		aux->SetType(Node::Type::EWalked);
		aux = path[aux].second;
	}

	m_EndTime = std::chrono::system_clock::now();
	std::cout << "Am ajuns la ultimul nod in: " << std::chrono::duration_cast<std::chrono::milliseconds>(m_EndTime - m_StartTime).count() / 60.0;

	update();
}

bool Luxembourg::isValid(const QPointF& point)
{
	if (graf.GetVectorOfNodes().size() > 0)
	{
		for (const auto& n : graf.GetVectorOfNodes())
		{
			if (n->getPoint() == point)
				continue;
			if (fabs(n->getPoint().x() - point.x()) < 28 && fabs(n->getPoint().y() - point.y()) < 28)
				return false;
		}
	}
	return true;
}

void Luxembourg::DrawByHand(QPainter& p)
{
	const float ellipseStroke = 2.9;
	const float rectangleSize = 35;


	if (graf.GetVectorOfNodes().size())
	{
		const std::vector<Node*>& noduri = graf.GetVectorOfNodes();
		QPointF sourcePoint, destPoint;
		p.setRenderHint(QPainter::Antialiasing);

		for (Node* nod : noduri)
		{
			QPen pen;
			pen.setWidth(ellipseStroke);
			p.setPen(pen);
			p.setBrush(QBrush(Qt::green));
			QRectF r(nod->getPoint().x() - 17.5, nod->getPoint().y() - 17.5, rectangleSize, rectangleSize);
			p.drawEllipse(r);

			QPointF newPoint = r.center();
			newPoint.setX(newPoint.x() - 2);
			newPoint.setY(newPoint.y() + 2);

			p.drawText(newPoint, QString::number(nod->getNumber()));
		}

		const std::vector<Arc*>& arce = graf.GetVectorOfArcs();
		for (Arc* arc : arce)
		{
			QPen pen;
			pen.setWidth(2);
			pen.setColor(Qt::blue);
			p.setPen(pen);

			QLineF line(arc->getFirstNode()->getPoint(), arc->getSecondNode()->getPoint());
			qreal length = line.length();


			if (length > qreal(10.)) {
				QPointF edgeOffset(((line.dx() * qreal(17.5)) / length),
					((line.dy() * qreal(17.5)) / length));

				sourcePoint = line.p1() + edgeOffset;
				destPoint = line.p2() - edgeOffset;
			}
			else {
				sourcePoint = destPoint = line.p1();
			}

			//p.drawLine(sourcePoint, destPoint);
			graf.DrawLineWithArrow(p, sourcePoint, destPoint);
		}

		if (drawNode)
		{
			Node* n = noduri.at(noduri.size() - 1);
			QPen pen;
			pen.setWidth(ellipseStroke);
			p.setPen(pen);
			p.setBrush(QBrush(Qt::red));
			QRectF r(n->getPoint().x() - 17.5, n->getPoint().y() - 17.5, rectangleSize, rectangleSize);
			p.drawEllipse(r);
			QPointF newPoint = r.center();

			newPoint.setX(newPoint.x() - 2);
			newPoint.setY(newPoint.y() + 2);
			//p.drawText(newPoint, QString::number(n->getNumber() + 1));
			p.drawText(newPoint, QString::number(n->getNumber()));
		}
		else if (drawArc)
		{
			drawArc = false;
			QPen pen;
			pen.setWidth(2);
			p.setBrush(QBrush(Qt::red));
			p.setPen(pen);

			QLineF line(arce.at(arce.size() - 1)->getFirstNode()->getPoint(),
				arce.at(arce.size() - 1)->getSecondNode()->getPoint());
			qreal length = line.length();


			if (length > qreal(10.)) {
				QPointF edgeOffset(((line.dx() * qreal(17.5)) / length),
					((line.dy() * qreal(17.5)) / length));

				sourcePoint = line.p1() + edgeOffset;
				destPoint = line.p2() - edgeOffset;
			}
			else {
				sourcePoint = destPoint = line.p1();
			}

			//p.drawLine(sourcePoint, destPoint);
			graf.DrawLineWithArrow(p, sourcePoint,
				destPoint);

			firstNode = Node();
			secondNode = Node();

		}
	}
}

void Luxembourg::drawLuxembourg(QPainter& p)
{
	float nodeSize = 4;
	p.setRenderHint(QPainter::Antialiasing);

	for (Arc* arc : graf.GetVectorOfArcs())
	{
		float x = width() / ((graf.longMax - graf.longMin) / divider) * ((arc->getFirstNode()->getPoint().y() - graf.longMin) / divider);
		x += xSpace;
		float y = height() / ((graf.latMax - graf.latMin) / divider) * ((graf.latMax - arc->getFirstNode()->getPoint().x()) / divider);
		y += ySpace;

		float x2 = width() / ((graf.longMax - graf.longMin) / divider) * ((arc->getSecondNode()->getPoint().y() - graf.longMin) / divider);
		x2 += xSpace;
		float y2 = height() / ((graf.latMax - graf.latMin) / divider) * ((graf.latMax - arc->getSecondNode()->getPoint().x()) / divider);
		y2 += ySpace;

		if (arc->getFirstNode()->GetType() == Node::Type::EWalked || arc->getSecondNode()->GetType() == Node::Type::EWalked)
		{
			QPen pen(Qt::red, 1);
			p.setPen(pen);
		}
		else
			if (arc->getFirstNode()->GetType() == Node::Type::EDefault || arc->getSecondNode()->GetType() == Node::Type::EDefault)
			{
				QPen pen(Qt::blue, 0.5);
				p.setPen(pen);
			}
		p.drawLine(x, y, x2, y2);
	}
}

void Luxembourg::on_draw_clicked()
{
	graf.DeleteAllArcs();
	graf.DeleteAllNodes();
	graf.getNeighbours().clear();
	m_drawByHand = true;
	luxembourg = false;
	ui.root->setEnabled(true);
	update();
}

void Luxembourg::on_luxembourg_clicked()
{
	graf.DeleteAllArcs();
	graf.DeleteAllNodes();
	graf.Read();
	m_drawByHand = false;
	luxembourg = true;
	ui.root->setEnabled(false);
	update();
}

void Luxembourg::on_root_clicked()
{
	if (!graf.isTree())
		graf.gasesteRadacina();
	else
	{
		IsTree = false;
		update();
	}
}

void Luxembourg::on_Dijkstra_clicked()
{
	Dijkstra();

	/*for (Node* n: graf.GetVectorOfNodes())
	{
		for (auto& smth : graf.m_neighbours[n])
		{
			std::cout << smth.first->getNumber() << " " << smth.second<<"\n";
		}
		std::cout << "\n";
	}*/
}
