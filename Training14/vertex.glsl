#version 330 core

layout (location = 0) in vec3 vPos; //--- ���� ���α׷����� �޾ƿ� ���� ��ǥ��
layout (location = 1) in vec3 in_Color; 
out vec3 out_Color; 

uniform mat4 modelTransform; //--- �𵨸� ��ȯ ���: uniform ������ ����

void main()
{
	gl_Position = modelTransform * vec4(vPos, 1.0); //--- ��ǥ���� modelTransform ��ȯ�� �����Ѵ�.
	out_Color = in_Color;
}
