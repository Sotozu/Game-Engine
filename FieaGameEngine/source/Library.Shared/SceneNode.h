#pragma once

#include "RTTI.h"
#include <DirectXMath.h>
#include <vector>

namespace FieaGameEngine
{
	class OutputStreamHelper;
	class InputStreamHelper;

	class SceneNode : public RTTI
	{
		RTTI_DECLARATIONS(SceneNode, RTTI)

	public:
		explicit SceneNode(const std::string& name = std::string());
		SceneNode(const std::string& name, const DirectX::XMFLOAT4X4& transform);
		SceneNode(const SceneNode&) = default;
		SceneNode(SceneNode&&) = default;
		SceneNode& operator=(const SceneNode&) = default;
		SceneNode& operator=(SceneNode&&) = default;
		virtual ~SceneNode() = default;
		
		const std::string& Name() const;
		std::weak_ptr<SceneNode> GetParent() const;
		std::vector<std::shared_ptr<SceneNode>>& Children();
		const DirectX::XMFLOAT4X4& Transform() const;
		DirectX::XMMATRIX TransformMatrix() const;

		void SetParent(std::shared_ptr<SceneNode> parent);

		void SetTransform(const DirectX::XMFLOAT4X4& transform);
		void SetTransform(DirectX::CXMMATRIX transform);

		virtual void Save(OutputStreamHelper& streamHelper);

		/// <summary>
		/// True when they are the SAME scene node.
		/// False when they are NOT THE SAME scene node.
		/// </summary>
		/// <param name="rhs">The scene node to compare against.</param>
		/// <returns>
		/// True when they are the SAME scene node.
		/// False when they are NOT THE SAME scene node.
		/// </returns>
		bool operator==(const SceneNode& rhs) const;

		/// <summary>
		/// True when they are NOT THE SAME scene node.
		/// False when they are the SAME scene node.
		/// </summary>
		/// <param name="rhs">The scene node to compare against.</param>
		/// <returns>
		/// True when they are NOT THE SAME scene node.
		/// False when they are the SAME scene node.
		/// </returns>		
		bool operator!=(const SceneNode& rhs) const;
	protected:
		virtual void Load(InputStreamHelper& streamHelper);

		std::string mName;
		std::weak_ptr<SceneNode> mParent;
		std::vector<std::shared_ptr<SceneNode>> mChildren;
		DirectX::XMFLOAT4X4 mTransform;		
	};
}
