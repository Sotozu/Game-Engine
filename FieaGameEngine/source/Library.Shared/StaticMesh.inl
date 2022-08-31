#include "StaticMesh.h"

namespace FieaGameEngine
{


	inline std::string StaticMesh::ToString() const { return "StaticMesh"s; }

	inline gsl::owner<StaticMesh*> StaticMesh::Clone() const { return new StaticMesh(*this); }


	inline void StaticMesh::Init(const std::string& name, const std::string& filePath)
	{
		_name = name;
		_meshFilePath = filePath;
	}

	inline void StaticMesh::Init(const std::string& filePath)
	{
		_meshFilePath = filePath;
	}

}