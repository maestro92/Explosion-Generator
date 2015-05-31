#include "mesh.h"

mesh::mesh(vector<vertexData>* vd,vector<unsigned int>* id,vector<textureData>* td)
{
	data=*vd;
	indices=*id;
	if(td)
		textures=*td;

    /// we generate the Vertex buffer object
    /// we then fill up the vertex buffer object
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,data.size()*sizeof(vertexData), &data[0],GL_STATIC_DRAW);

    /// we generate the Index buffer object
    /// we then fill up the Index buffer object
    /// indexes should be bound to GL_ELEMENT_ARRAY_BUFFER
	glGenBuffers(1,&IND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);

    /// we unbind the buffers, so that no one accidentally unbind the buffers
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

mesh::~mesh()
{
	glDeleteBuffers(1,&VBO);
	glDeleteBuffers(1,&IND);
}


/*
inputs to a vertex shader are called vertex attributes
which is exactly what this function means
    glEnableVertexAttribArray(vertex);

Each input to a vertex shader has an index location called an attribute index
The input in shaders is defined using the layout statement



*/



void mesh::draw()
{

	int vertex=0;
	int normal=1;
	int tangent=2; //2
	int color=3; //3
	int UV=4; //4

	//texture0
	//texture1...
	string str="texture";
//	cout << "texture size" << textures.size() << endl;
	for(int i=0;i<textures.size();i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D,textures[i].id);

//////////////
//		glUniform1i(glGetUniformLocation(programId,(str+(char)(i+'0')).c_str()),i);
//////////////
        cout << (str+(char)(i+'0')).c_str() << endl;
	}

    /// this means we're going to use these two buffer object
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);


    /// http://arcsynthesis.org/gltut/Basics/Tut01%20Following%20the%20Data.html
    /// glEnableVertexAttribArray is

    /// glVertexAttribPointer tells how OpenGL interprets your data in the VBO object
    /// when rendering, OpenGL pulls vertex data from arrays stored in buffer objects.
    /// what we need to tell OpenGL is what form our vertex array in the buffer object is stored in
    /// meaning we need to tell OpenGL how to interpret the array of data stored in the buffer
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),0);

	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(3*sizeof(float)));

	glEnableVertexAttribArray(tangent);
	glVertexAttribPointer(tangent,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(6*sizeof(float)));

	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(9*sizeof(float)));

	glEnableVertexAttribArray(UV);
	glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(12*sizeof(float)));

    /// glDrawElements, you need to supply an index buffer
    /// glDrawArrays submits the vertices in linear order
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(vertex);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(tangent);
	glDisableVertexAttribArray(color);
	glDisableVertexAttribArray(UV);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}





void mesh::drawSpecial()
{

	int vertex=0;
	int normal=1;
	int tangent=2; //2
	int color=3; //3
	int UV=4; //4

	//texture0
	//texture1...
	/*
	string str="texture";
//	cout << "texture size" << textures.size() << endl;
	for(int i=0;i<textures.size();i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D,textures[i].id);

//////////////
//		glUniform1i(glGetUniformLocation(programId,(str+(char)(i+'0')).c_str()),i);
//////////////
        cout << (str+(char)(i+'0')).c_str() << endl;
	}
*/



    /// this means we're going to use these two buffer object
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);


    /// http://arcsynthesis.org/gltut/Basics/Tut01%20Following%20the%20Data.html
    /// glEnableVertexAttribArray is

    /// glVertexAttribPointer tells how OpenGL interprets your data in the VBO object
    /// when rendering, OpenGL pulls vertex data from arrays stored in buffer objects.
    /// what we need to tell OpenGL is what form our vertex array in the buffer object is stored in
    /// meaning we need to tell OpenGL how to interpret the array of data stored in the buffer
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),0);

	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(3*sizeof(float)));

	glEnableVertexAttribArray(tangent);
	glVertexAttribPointer(tangent,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(6*sizeof(float)));

	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(9*sizeof(float)));

	glEnableVertexAttribArray(UV);
	glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(12*sizeof(float)));

    /// glDrawElements, you need to supply an index buffer
    /// glDrawArrays submits the vertices in linear order
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(vertex);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(tangent);
	glDisableVertexAttribArray(color);
	glDisableVertexAttribArray(UV);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

}



void mesh::drawLines()
{

	int vertex=0;
	int normal=1;
	int tangent=2; //2
	int color=3; //3
	int UV=4; //4

	//texture0
	//texture1...
	string str="texture";
//	cout << "texture size" << textures.size() << endl;
	for(int i=0;i<textures.size();i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D,textures[i].id);

//////////////
//		glUniform1i(glGetUniformLocation(programId,(str+(char)(i+'0')).c_str()),i);
//////////////
        cout << (str+(char)(i+'0')).c_str() << endl;
	}

    /// this means we're going to use these two buffer object
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);


    /// http://arcsynthesis.org/gltut/Basics/Tut01%20Following%20the%20Data.html
    /// glEnableVertexAttribArray is

    /// glVertexAttribPointer tells how OpenGL interprets your data in the VBO object
    /// when rendering, OpenGL pulls vertex data from arrays stored in buffer objects.
    /// what we need to tell OpenGL is what form our vertex array in the buffer object is stored in
    /// meaning we need to tell OpenGL how to interpret the array of data stored in the buffer
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),0);

	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(3*sizeof(float)));

	glEnableVertexAttribArray(tangent);
	glVertexAttribPointer(tangent,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(6*sizeof(float)));

	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(9*sizeof(float)));

	glEnableVertexAttribArray(UV);
	glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(12*sizeof(float)));

    /// glDrawElements, you need to supply an index buffer
    /// glDrawArrays submits the vertices in linear order
	glDrawElements(GL_LINES,indices.size(),GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(vertex);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(tangent);
	glDisableVertexAttribArray(color);
	glDisableVertexAttribArray(UV);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}






void mesh::draw(unsigned int programId)
{

	//attribute vec3 vertex

	/// this is similar to the glGetuniform location
	int vertex=glGetAttribLocation(programId,"vertex"); //0
	int normal=glGetAttribLocation(programId,"normal"); //1
	int tangent=glGetAttribLocation(programId,"tangent"); //2
	int color=glGetAttribLocation(programId,"color"); //3
	int UV=glGetAttribLocation(programId,"UV"); //4

/*
    cout << "vertex is " << vertex << endl;
    cout << "normal is " << normal << endl;
    cout << "tangent is " << tangent << endl;
    cout << "color is " << color << endl;
    cout << "UV is " << UV << endl << endl;
*/


/*
	int vertex=0;
	int normal=1;
	int tangent=2; //2
	int color=3; //3
	int UV=4; //4
*/
	//texture0
	//texture1...
	string str="texture";
//	cout << "texture size" << textures.size() << endl;
	for(int i=0;i<textures.size();i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D,textures[i].id);
		glUniform1i(glGetUniformLocation(programId,(str+(char)(i+'0')).c_str()),i);
        cout << (str+(char)(i+'0')).c_str() << endl;
	}

    /// this means we're going to use these two buffer object
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);


    /// http://arcsynthesis.org/gltut/Basics/Tut01%20Following%20the%20Data.html
    /// glEnableVertexAttribArray is

    /// glVertexAttribPointer tells how OpenGL interprets your data in the VBO object
    /// when rendering, OpenGL pulls vertex data from arrays stored in buffer objects.
    /// what we need to tell OpenGL is what form our vertex array in the buffer object is stored in
    /// meaning we need to tell OpenGL how to interpret the array of data stored in the buffer
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),0);

	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(3*sizeof(float)));

	glEnableVertexAttribArray(tangent);
	glVertexAttribPointer(tangent,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(6*sizeof(float)));

	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(9*sizeof(float)));

	glEnableVertexAttribArray(UV);
	glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(12*sizeof(float)));

    /// glDrawElements, you need to supply an index buffer
    /// glDrawArrays submits the vertices in linear order
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(vertex);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(tangent);
	glDisableVertexAttribArray(color);
	glDisableVertexAttribArray(UV);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


}






void mesh::draw2()
{


    /// this means we're going to use these two buffer object
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);


    /// http://arcsynthesis.org/gltut/Basics/Tut01%20Following%20the%20Data.html
    /// glEnableVertexAttribArray is

    /// glVertexAttribPointer tells how OpenGL interprets your data in the VBO object
    /// when rendering, OpenGL pulls vertex data from arrays stored in buffer objects.
    /// what we need to tell OpenGL is what form our vertex array in the buffer object is stored in
    /// meaning we need to tell OpenGL how to interpret the array of data stored in the buffer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(3*sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(6*sizeof(float)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(9*sizeof(float)));

    /// glDrawElements, you need to supply an index buffer
    /// glDrawArrays submits the vertices in linear order
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


}

