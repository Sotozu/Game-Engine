#include "ActionMeshRenderer.h"

namespace FieaGameEngine
{


	inline std::string ActionMeshRenderer::ToString() const { return "ActionMeshRenderer"s; }

	inline gsl::owner<ActionMeshRenderer*> ActionMeshRenderer::Clone() const { return new ActionMeshRenderer(*this); }


	inline void ActionMeshRenderer::Init(const std::string& name, const std::string& filePath)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(filePath);
	}

	inline void ActionMeshRenderer::Init(const std::string& filePath)
	{
		UNREFERENCED_PARAMETER(filePath);
	}

}