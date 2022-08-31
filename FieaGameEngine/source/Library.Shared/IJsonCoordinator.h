#pragma once
#include <json/json.h>
#include <cstddef>
#include <fstream>
#include <string.h>
#include <gsl/pointers>

#include "IJsonParseHelper.h"
#include "Vector.h"
#include "RTTI.h"

namespace FieaGameEngine
{

	class IJsonCoordinator final //don't want to inherit
	{

	public:
		/// <summary>
		/// Used in FindHelper
		/// First: Used to hold a pointer to a parse helper that is in the helpers vector.
		/// Second: Used to hold an index to a parse helper that is in the helpers vector
		/// </summary>
		using pairType = std::pair<const IJsonParseHelper*, std::int32_t>;


		IJsonCoordinator() = default;
		~IJsonCoordinator();
		IJsonCoordinator(IJsonCoordinator&& rhs) noexcept = default;
		IJsonCoordinator& operator=(IJsonCoordinator&& rhs) noexcept = default;

		IJsonCoordinator& operator=(IJsonCoordinator& rhs) = delete;


		/// <summary>
		/// Will create a copy of the current coordinator on the heap.
		/// </summary>
		/// <returns>
		/// Returns a copy of a coordinator that was created on the heap.
		/// </returns>
		gsl::owner<IJsonCoordinator*> Clone();

		/// <summary>
		/// Adds a helper of the type specified in the template argument.
		/// Helper is created and managed by the coordinator.
		/// </summary>
		/// <typeparam name="HelperType">The type of helper you would like to add to the coordinator</typeparam>
		/// <returns>
		/// True if the helper was added. Means no existing helper of the same kind was being managed by the coordinator.
		/// False if a helper of the type being passed already exists and is managed by the coordinator.
		/// </returns>
		template <typename HelperType>
		bool AddHelper();

		/// <summary>
		/// Removes a helper of the type specified in the template argument.
		/// Helper managed by the coordinator and is removed.
		/// </summary>
		/// <typeparam name="HelperType">The type of helper you would like to remove from the coordinator</typeparam>
		/// <returns>
		/// True if the helper was removed. Means a helper of the type being passed exists and is successfully removed.
		/// False if a helper of the type being passed does NOT exists.
		/// </returns>
		template <typename HelperType>
		bool RemoveHelper();

		/// <summary>
		/// De-serializes the contents of the string stream (istream) using Json.
		/// Breaks down a string stream and converts it to Json value. parses each element of the Json value.
		/// </summary>
		/// <typeparam name="HelperTypeData">The deserialized data that is contained by a class that is binded to the helper class used to populate it.</typeparam>
		/// <param name="inputStream">String stream of Json data</param>
		/// <returns>
		/// If successful returns a pointer to the class containing the de-serialized data.
		/// If NOT successful returns nullptr.
		/// </returns>
		template <typename HelperTypeData>
		HelperTypeData* Parse(std::istream& inputStream);

		/// <summary>
		/// De-serializes the contents of the file using Json.
		/// Opens a file with the name passed into the parameter.
		/// Converts the file into a string stream and then into a Json value and parses each element.
		/// </summary>
		/// <typeparam name="HelperTypeData">The deserialized data that is contained by a class that is binded to the helper class used to populate it.</typeparam>
		/// <param name="inputStream">String stream of Json data</param>
		/// <returns>
		/// If successful returns a pointer to the class containing the de-serialized data.
		/// If NOT successful returns nullptr.
		/// </returns>
		template <typename HelperTypeData>
		HelperTypeData* ParseFromFile(const std::string& filename);

		/// <summary>
		/// De-serializes the contents of the string using Json.
		/// Converts a string into a Json value and parses each element.
		/// </summary>
		/// <typeparam name="HelperTypeData">The deserialized data that is contained by a class that is binded to the helper class used to populate it.</typeparam>
		/// <param name="inputStream">String stream of Json data</param>
		/// <returns>
		/// If successful returns a pointer to the class containing the de-serialized data.
		/// If NOT successful returns nullptr.
		/// </returns>
		template <typename HelperTypeData>
		HelperTypeData* ParseFromString(const std::string& stringData);

		/// <summary>
		/// Returns the number of helpers that the coordinator manages.
		/// </summary>
		/// <returns>The number of helpers managed by the coordinator</returns>
		std::size_t NumberOfHelpers();



		/// <summary>
		/// Takes a template parameter of a class derived from IJasonParseHelper.
		/// Searches the list of helpers in the ParseCoordinate to determine if that specific parse helper exists within the vector of parse helpers.
		/// </summary>
		/// <returns>
		/// A pair of IJsonParseHelper* that points to the parse helper found and a int for the index of where the parse helper was found in the vector of parse helpers.
		/// IJasonParseHelper* will return nullptr if the parse helper was NOT found.
		/// The index specifying int will return -1 if the helper was not found.
		/// </returns>
		template <typename HelperType>
		pairType FindHelper();

	private:
		/// <summary>
		/// Used with the publicly accessible clone method.
		/// </summary>
		/// <param name="rhs"></param>
		IJsonCoordinator(IJsonCoordinator & rhs);


		/// <summary>
		/// Links the pointer _myHelpersData in the Coordinator to the specified helper data in the typeparam.
		/// </summary>
		/// <typeparam name="HelperTypeData">The helpers data</typeparam>
		/// <returns>
		/// If found a pointer to a helpers data.
		/// If not found returns nullptr.
		/// </returns>
		template<typename HelperTypeData>
		bool LinkHelperData();

		/// <summary>
		/// String is parsed and data is formated and stored into a class the is unique to the helper that is parsing the data.
		/// </summary>
		/// <typeparam name="HelperTypeData">This is the class that holds the formated data of the parssed string</typeparam>
		/// <param name="stringData">String that we wish to parse and retrieve data from</param>
		/// <returns>
		/// A pointer to a class that contains the formatted data.
		/// False if a 
		/// </returns>
		template <typename HelperTypeData>
		HelperTypeData* Parse(const std::string& name, Json::Value& value);

		/// <summary>
		/// Holds pointers to the helpers that the coordinator has access to.
		/// These helpers are created by the user and become the responsibility of the coordinator to delete.
		/// </summary>
		Vector<FieaGameEngine::IJsonParseHelper*> _myHelpers;

		/// <summary>
		/// When the a helper signifies that it can handle the data this pointer will point to that helpers data.
		/// This allows for the coordinator to have access to the data.
		/// Once the process is finished then this pointer will be set to nullptr breaking the connection to the helpers data.
		/// </summary>
		HelperData* _myHelperData{nullptr};


		/// <summary>
		/// Takes on Json key (name) and value (root) and processes it for de-serialization.
		/// </summary>
		/// <typeparam name="HelperTypeData">The deserialized data that is contained by a class that is binded to the helper class used to populate it.</typeparam>
		/// <param name="name">The Json key</param>
		/// <param name="root">The Json value</param>
		template <typename HelperTypeData>
		void ParseArray(const std::string& name, Json::Value& root);
	
		/// <summary>
		/// This Json value must be of type "object".
		/// Takes on Json value (root) and processes it for de-serialization.
		/// </summary>
		/// <typeparam name="HelperTypeData">The deserialized data that is contained by a class that is binded to the helper class used to populate it.</typeparam>
		/// <param name="root">The Json value of type "object"</param>
		template <typename HelperTypeData>
		HelperTypeData* ParseMembers(Json::Value& root);

	};

}

#include "IJsonCoordinator.inl"