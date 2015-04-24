#include "EG_Quad.h"


EG_Quad::EG_Quad()
{

}

EG_Quad::~EG_Quad()
{

}

void EG_Quad::init()
{
    /// assigning a quad for rendering textures
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		//1.
		tmp.position = glm::vec3(-1.0,1.0,0.0);
		tmp.U=0;
		tmp.V=1;
		vertices.push_back(tmp);
		//2.
		tmp.position = glm::vec3(-1.0,-1.0,0.0);
		tmp.U=0;
		tmp.V=0;
		vertices.push_back(tmp);
		//3.
		tmp.position = glm::vec3(1.0,-1.0,0.0);
		tmp.U=1;
		tmp.V=0;
		vertices.push_back(tmp);
		//4.
		tmp.position = glm::vec3(1.0,1.0,0.0);
		tmp.U=1;
		tmp.V=1;
		vertices.push_back(tmp);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
		quad=new mesh(&vertices,&indices);
	}
}



void EG_Quad::init(int w, int h)
{
    init(w, h, 0, 0, 1);
}


void EG_Quad::init(int w, int h, float cx, float cy, float cw)
{
    /// assigning a quad for rendering textures
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;

        float cx1 = cx;  float cx2 = cx+1;
        float cy1 = cy;  float cy2 = cy+1;
        //1.
		tmp.position = glm::vec3(0.0, h, 0.0);
		tmp.U = cx1 * cw;
		tmp.V = cy1 * cw;
		vertices.push_back(tmp);
		//2.
		tmp.position = glm::vec3(0.0, 0.0, 0.0);
		tmp.U = cx1 * cw;
		tmp.V = cy2 * cw;
		vertices.push_back(tmp);

		//3.
		tmp.position = glm::vec3(w, 0.0, 0.0);
		tmp.U = cx2 * cw;
		tmp.V = cy2 * cw;
		vertices.push_back(tmp);
        //4.
		tmp.position = glm::vec3(w, h, 0.0);
		tmp.U = cx2 * cw;
		tmp.V = cy1 * cw;
		vertices.push_back(tmp);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
		quad=new mesh(&vertices,&indices);
	}
}

