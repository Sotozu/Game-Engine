#pragma once
#include <DirectXMath.h>


struct Vertex
{
	Vertex() : _pos{0.0f,0.0f} {}
	Vertex(float x, float y) :
		_pos(x, y) {}

	DirectX::XMFLOAT2 _pos;
};

