#version 330

in vec4 Position;
out vec4 vPosition;
uniform mat4 m_ModelviewProjection;

void main()
{
    gl_Position = m_ModelviewProjection * Position;
    vPosition = Position;
}