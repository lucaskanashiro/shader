#ifndef MESH_H
#define MESH_H

#include <sstream>
#include <fstream>
#include <cstdio>
#include <cctype>
#include "point3d.h"
#include "vector3d.h"

#include <iostream>

using namespace std;

class MeshOff : public Drawable {
public:
	const static int TRIANGLES = 0;
	const static int LINES = 1;

	MeshOff();							//mesh *New_Mesh()
	virtual ~MeshOff();				//int Del_Mesh(mesh *pmesh)

	void draw();					//int Render_Mesh(mesh *pmesh);
	void resize(float scalar);		//int Resize_Mesh(mesh *pmesh);
	void loadFile(string fileName);	//int Load_Mesh(mesh *pmesh, char *File_Name);

	void setPosition(float x = 0, float y = 0, float z = 0);
	void incPosition(float x = 0, float y = 0, float z = 0);

	void clear();

	void setType(int drawType);
private:
	vector<Point3D *> vectorPoint;
	vector< vector<int> > vectorIndexFaces;

	bool haveColor;

	void (MeshOff::*drawType)() = NULL;

	void drawLines();
	void drawTriangles();
	void resizePoints(Point3D *head, Point3D *tail, float scalar);

	void loadFileToStringStream(string fileName, stringstream &stringCin);
	void loadPoints(stringstream &stringCin, unsigned int numPoints);
	void loadFaces(stringstream &stringCin, unsigned int numFaces);
};

MeshOff::MeshOff()
{
	this->haveColor = false;
	this->drawType = &MeshOff::drawTriangles;
}

MeshOff::~MeshOff()
{
	
}
void
MeshOff::draw()
{
	(this->*drawType)();
}

void
MeshOff::resize(float scalar)
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	float midX, midY, midZ;
	Point3D midPoint;

	for(unsigned int i = 0; i < vectorPoint.size(); i++)
	{
		if(i == 0)
		{
			minX = vectorPoint[i]->x;
			minY = vectorPoint[i]->y;
			minZ = vectorPoint[i]->z;

			maxX = vectorPoint[i]->x;
			maxY = vectorPoint[i]->y;
			maxZ = vectorPoint[i]->z;
		}

		if(vectorPoint[i]->x < minX)
			minX = vectorPoint[i]->x;

		if(vectorPoint[i]->x > maxX)
			maxX = vectorPoint[i]->x;

		if(vectorPoint[i]->y < minY)
			minY = vectorPoint[i]->y;

		if(vectorPoint[i]->y > maxY)
			maxY = vectorPoint[i]->y;

		if(vectorPoint[i]->z < minZ)
			minZ = vectorPoint[i]->z;

		if(vectorPoint[i]->z > maxZ)
			maxZ = vectorPoint[i]->z;
	}

	midX = (maxX + minX) / 2;
	midY = (maxY + minY) / 2;
	midZ = (maxZ + minZ) / 2;

	midPoint = Point3D(midX, midY, midZ);

	for(unsigned int i = 0; i < vectorPoint.size(); i++)
	{
		resizePoints(&midPoint, vectorPoint[i], scalar);
	}
}

void
MeshOff::loadFile(string fileName)
{
	unsigned int numPoints, numFaces;
	ifstream file(fileName.c_str());
	stringstream stringCin;
	string lineOne;

	loadFileToStringStream(fileName, stringCin);
	stringCin >> lineOne;

	if(toupper(lineOne[0]) == 'C')
		this->haveColor = true;
	else
		this->haveColor = false;
	
	stringCin >> numPoints >> numFaces >> lineOne;
	
	loadPoints(stringCin, numPoints);
	loadFaces(stringCin, numFaces);	
}

void
MeshOff::setPosition(float x, float y, float z)
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	float midX, midY, midZ;
	Point3D midPoint, newPoint;
	Vector3D vector3d;

	for(unsigned int i = 0; i < vectorPoint.size(); i++)
	{
		if(i == 0)
		{
			minX = vectorPoint[i]->x;
			minY = vectorPoint[i]->y;
			minZ = vectorPoint[i]->z;

			maxX = vectorPoint[i]->x;
			maxY = vectorPoint[i]->y;
			maxZ = vectorPoint[i]->z;
		}

		if(vectorPoint[i]->x < minX)
			minX = vectorPoint[i]->x;

		if(vectorPoint[i]->y < minY)
			minY = vectorPoint[i]->y;

		if(vectorPoint[i]->z < minZ)
			minZ = vectorPoint[i]->z;

		if(vectorPoint[i]->x > maxX)
			maxX = vectorPoint[i]->x;
		if(vectorPoint[i]->y > maxY)
			maxY = vectorPoint[i]->y;
		if(vectorPoint[i]->z > maxZ)
			maxZ = vectorPoint[i]->z;
	}

	midX = (maxX + minX) / 2;
	midY = (maxY + minY) / 2;
	midZ = (maxZ + minZ) / 2;

	newPoint = Point3D(x, y, z);
	midPoint = Point3D(midX, midY, midZ);
	vector3d = Vector3D(midPoint, newPoint);

	for(unsigned int i = 0; i < vectorPoint.size(); i++)
	{
		*(vectorPoint[i]) = vector3d + *(vectorPoint[i]);
	}
}

void
MeshOff::incPosition(float x, float y, float z)
{
	float minX, minY, minZ;
	Point3D minPoint, newPoint;
	Vector3D vector3d;

	for(unsigned int i = 0; i < vectorPoint.size(); i++)
	{
		if(i == 0)
		{
			minX = vectorPoint[i]->x;
			minY = vectorPoint[i]->y;
			minZ = vectorPoint[i]->z;
		}

		if(vectorPoint[i]->x < minX)
			minX = vectorPoint[i]->x;

		if(vectorPoint[i]->y < minY)
			minY = vectorPoint[i]->y;

		if(vectorPoint[i]->z < minZ)
			minZ = vectorPoint[i]->z;
	}

	newPoint = Point3D(x, y, z);
	minPoint = Point3D(minX, minY, minZ);
	newPoint = newPoint + minPoint;
	vector3d = Vector3D(minPoint, newPoint);

	for(unsigned int i = 0; i < vectorPoint.size(); i++)
	{
		*(vectorPoint[i]) = vector3d + *(vectorPoint[i]);
	}
}

void
MeshOff::clear()
{
	for(unsigned int i = 0; i < this->vectorPoint.size(); i++)
	{
		delete this->vectorPoint[i];
	}
	this->vectorPoint.clear();
	this->vectorIndexFaces.clear();
}

void
MeshOff::setType(int drawType)
{
	switch(drawType)
	{
		case TRIANGLES:
			this->drawType = &MeshOff::drawTriangles;
			break;
		case LINES:
			this->drawType = &MeshOff::drawLines;
			break;
	}
}

void
MeshOff::drawLines()
{
	for(unsigned int i = 0; i < vectorIndexFaces.size(); i++)
	{
		glBegin(GL_LINES);
			this->vectorPoint[vectorIndexFaces[i][0]]->drawGl();
			this->vectorPoint[vectorIndexFaces[i][1]]->drawGl();
		glEnd();

		glBegin(GL_LINES);
			this->vectorPoint[vectorIndexFaces[i][0]]->drawGl();
			this->vectorPoint[vectorIndexFaces[i][2]]->drawGl();
		glEnd();

		glBegin(GL_LINES);
			this->vectorPoint[vectorIndexFaces[i][1]]->drawGl();
			this->vectorPoint[vectorIndexFaces[i][2]]->drawGl();
		glEnd();
	}
}

void
MeshOff::drawTriangles()
{
	for(unsigned int i = 0; i < vectorIndexFaces.size(); i++)
	{
		glBegin(GL_TRIANGLES);
			this->vectorPoint[vectorIndexFaces[i][0]]->drawGl();
			this->vectorPoint[vectorIndexFaces[i][1]]->drawGl();
			this->vectorPoint[vectorIndexFaces[i][2]]->drawGl();
		glEnd();
	}
}

void
MeshOff::resizePoints(Point3D *head, Point3D *tail, float scalar)
{
	Vector3D vector3d(*head, *tail);

	vector3d *= scalar;
	*tail = vector3d + *head;
}

void
MeshOff::loadFileToStringStream(string fileName, stringstream &stringCin)
{
	ifstream file(fileName.c_str());

	copy(istreambuf_iterator<char>(file),
	     istreambuf_iterator<char>(),
	     ostreambuf_iterator<char>(stringCin));
	file.close();
}

void
MeshOff::loadPoints(stringstream &stringCin, unsigned int numPoints)
{
	float x, y, z;
	int r, g, b, a;

	for(unsigned int i = 0; i < numPoints; i++)
	{
		stringCin >> x >> y >> z;

		Point3D *point = new Point3D(x, y, z);

		if(this->haveColor)
			stringCin >> r >> g >> b >> a;
		else
			r = g = b = a = 255;

		point->setColor(r, g, b, a);

		this->vectorPoint.push_back(point);
	}
}

void
MeshOff::loadFaces(stringstream &stringCin, unsigned int numFaces)
{
	int indexPoint;
	unsigned int numIndex;

	for(unsigned int i = 0; i < numFaces; i++)
	{
		vectorIndexFaces.push_back(vector<int>());
		stringCin >> numIndex;

		for(unsigned int j = 0; j < numIndex; j++)
		{
			stringCin >> indexPoint;
			vectorIndexFaces[i].push_back(indexPoint);
		}
	}
}

#endif