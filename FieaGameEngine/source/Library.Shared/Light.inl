namespace FieaGameEngine
{

	//inline const DirectX::XMFLOAT4& Light::Color() const
	//{
	//	return mColor;
	//}

	inline const glm::vec4& Light::Color() const
	{
		return mColor;
	}

	//inline DirectX::PackedVector::XMCOLOR Light::XmColor() const
	//{
	//	return DirectX::PackedVector::XMCOLOR(reinterpret_cast<const float*>(&mColor));
	//}

	//inline DirectX::XMVECTOR Light::ColorVector() const
	//{
	//	return XMLoadFloat4(&mColor);
	//}

	inline void Light::SetColor(float r, float g, float b, float a)
	{
		mColor = glm::vec4(r, g, b, a);
	}

	inline void Light::SetColor(const DirectX::XMFLOAT4& color)
	{
		mColor = glm::vec4(color.x, color.y, color.z, color.w);
	}

	inline void Light::SetColor(const glm::vec4& color)
	{
		mColor = color;
	}

	//inline void Light::SetColor(const DirectX::PackedVector::XMCOLOR& color)
	//{
	//	mColor = ColorHelper::ToFloat4(color, true);
	//}

	//inline void Light::SetColor(DirectX::FXMVECTOR color)
	//{
	//	XMStoreFloat4(&mColor, color);
	//}
}