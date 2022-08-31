
namespace FieaGameEngine
{
	inline gsl::owner<BasicMaterial*> BasicMaterial::Clone() const
	{
		return new BasicMaterial{ *this };
	}

	inline std::string BasicMaterial::ToString() const
	{
		return "BasicMaterial"s;
	}

}