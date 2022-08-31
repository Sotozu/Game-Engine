#pragma once

#include "GameObject.h"
#include "ColorHelper.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

namespace FieaGameEngine
{
	class Light : public GameObject
	{
		RTTI_DECLARATIONS(Light, GameObject)

	public:

#pragma region REQUIRED METHODS TO SUPPORT RTTI AND ATTRIBUTED DESIGN

		/// <summary>
		/// Clones the calling GameObject. Heap constructs this object and it's on the user to manage its memory.
		/// Overridden function from Scope.
		/// </summary>
		/// <returns>
		/// A pointer to heap allocated clone of the calling GameObject.
		/// </returns>
		gsl::owner<Light*> Clone() const override;

		/// <summary>
		/// Override function from GameObject
		/// Update method will update the GameObject and then recursively update all it's children GameObjects.
		/// Made virtual so any derived classes from GameObject can have their own update method and be called polymorphically.
		/// </summary>
		/// <param name="gameTime">A reference to the game time</param>
		void Update(const GameState& gameState) override;

		/// <summary>
		/// Override function from GameObject
		/// </summary>
		/// <param name="gameState"></param>
		void Draw(const GameState& gameState) override;

		/// <summary>
		/// Overridden function from RTTI.
		/// Compares one object with another.
		/// </summary>
		/// <param name="rhs">The object to compare against</param>
		/// <returns>
		/// TRUE if the object is equivalent.
		/// FALSE if the object is not equivalent
		/// </returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Overridden function from RTTI.
		/// Returns a string that identifies this class.
		/// </summary>
		/// <returns>A string that identifies this class</returns>
		inline std::string ToString() const override;

		/// <summary>
		/// Method is used for registering this types signatures and for classes that inherit from GameObject.
		/// Used by TypeRegistry class
		/// </summary>
		/// <returns>
		/// This classes unique signatures.
		/// </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Macros is defined in TypeRegistry header file.
		/// This is used to register this class with the TypeRegistry.
		/// Will start a recursive call that will register all classes that are inherited.
		/// </summary>
		/// <param name="">Derived Class</param>
		/// <param name="">Base Class</param>
		StaticRegisterMethod(Light, GameObject);
	protected:

		/// <summary>
		/// Protected constructor that is implemented in inheriting Objects who need to pass on their TypeId to Attributed.
		/// </summary>
		/// <param name="typeId">The derived classes TypeId</param>
		Light(std::size_t typeId);

	public:

#pragma endregion
		Light();

		Light(const Light&) = default;
		Light& operator=(const Light&) = default;
		Light& operator=(Light&&) = default;
		Light(Light&&) = default;
		virtual ~Light() = default;

		//const DirectX::XMFLOAT4& Color() const;
		const glm::vec4& Color() const;
		//DirectX::PackedVector::XMCOLOR XmColor() const;
		//DirectX::XMVECTOR ColorVector() const;
		void SetColor(float r, float g, float b, float a);
		//void SetColor(const glm::vec4& color);
		void SetColor(const DirectX::XMFLOAT4& color);
		void SetColor(const glm::vec4& color);
		//void SetColor(const DirectX::PackedVector::XMCOLOR& color);
		//void SetColor(DirectX::FXMVECTOR color);

	protected:
		glm::vec4 mColor;
		//DirectX::XMFLOAT4 mColor;
	};

	/// <summary>
	/// Macro is defined in Factory header file
	/// Will create a class that creates this type of object and registers itself with the Factory Group. 
	/// </summary>
	/// <param name="">Concrete to be made in the factory</param>
	/// <param name="">The group to which this concrete object belongs to. The base class designed to be the defining type for the group.</param>
	ConcreteFactory(Light, Scope);

}

#include "Light.inl"