
#pragma once

#include "required.h"
#include "client.h"
#include "dataelement.h"

namespace valvegen
{
	/// <summary>
	/// Class ClassNode.
	/// Represents a class in memory and all its data alements (member variables)
	/// </summary>
	class ClassNode
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ClassNode"/> class.
		/// </summary>
		ClassNode();

		/// <summary>
		/// Cleans up this instance.
		/// </summary>
		void Cleanup();

		/// <summary>
		/// Sets the parent.
		/// </summary>
		/// <param name="parent">The parent.</param>
		void SetParent(ClassNode* parent);

		/// <summary>
		/// Sets the name of the class.
		/// </summary>
		/// <param name="name">The name.</param>
		void SetClassName(std::string name);

		/// <summary>
		/// Creates a data element (member variable)
		/// </summary>
		/// <param name="offset">The offset.</param>
		/// <param name="name">The name.</param>
		/// <param name="dtype">The dtype.</param>
		/// <param name="stride">The stride.</param>
		/// <returns>valvegen.DataElement *.</returns>
		DataElement* CreateDataElement(DWORD offset, std::string name, DATA_TYPE dtype, DWORD stride = 0);

		/// <summary>
		/// Creates the data element class instance.
		/// </summary>
		/// <param name="offset">The offset.</param>
		/// <param name="name">The name.</param>
		/// <param name="varname">The varname.</param>
		/// <param name="dtype">The dtype.</param>
		/// <returns>valvegen.DataElement *.</returns>
		DataElement* CreateDataElementClassInstance(DWORD offset, std::string name, std::string varname, DATA_TYPE dtype);

		/// <summary>
		/// Gets the name of the class
		/// </summary>
		/// <returns>std.string.</returns>
		std::string GetBaseName();

		/// <summary>
		/// Reorder the members by offset value
		/// </summary>
		void ShuffleMembers();

		/// <summary>
		/// Ouputs the header.
		/// </summary>
		/// <param name="of">The of.</param>
		void OuputHeader(std::ofstream& of);

		/// <summary>
		/// Resolves the includes.
		/// </summary>
		/// <param name="of">The of.</param>
		void ResolveIncludes(std::ofstream& of);

		/// <summary>
		/// Determines whether this instance has inheritence.
		/// </summary>
		/// <returns>bool.</returns>
		bool HasInheritence();

		/// <summary>
		/// Gets the name of the inherited class.
		/// </summary>
		/// <param name="index">The index.</param>
		/// <returns>std.string.</returns>
		std::string GetInheritedClassName(UINT index = 0);

		/// <summary>
		/// Gets the number of parent (inherited) class.
		/// </summary>
		/// <returns>UINT.</returns>
		UINT GetNumParents();

		/// <summary>
		/// Gets the parent at an index.
		/// </summary>
		/// <param name="index">The index.</param>
		/// <returns>valvegen.ClassNode *.</returns>
		ClassNode* GetParent(UINT index = 0);

		/// <summary>
		/// Gets the data elements.
		/// </summary>
		/// <returns>std.vector&lt;DataElement*&gt;&.</returns>
		std::vector<DataElement*>& GetDataElements();

		/// <summary>
		/// Gets the size of the inherited class.
		/// </summary>
		/// <param name="index">The index.</param>
		/// <returns>DWORD.</returns>
		DWORD GetInheritedClassSize(UINT index = 0);

		/// <summary>
		/// Gets the size of the class.
		/// </summary>
		/// <returns>DWORD.</returns>
		DWORD GetClassSize() const;

		/// <summary>
		/// Shuffles the parents.
		/// </summary>
		void ShuffleParents();

	private:
		/// <summary>
		/// The parents_
		/// </summary>
		std::vector<ClassNode*> parents_;

		/// <summary>
		/// The class_name_
		/// </summary>
		std::string class_name_;

		/// <summary>
		/// The data_elements_
		/// </summary>
		std::vector<DataElement*> data_elements_;
	};
}
