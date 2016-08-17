
#pragma once

#include "required.h"

namespace valvegen
{
	/* matches dt_common.h */
	enum DATA_TYPE
	{
		INT,
		FLOAT,
		VECTOR,
		VECTORXY,
		STRING,
		ARRAY,
		CLASS,
		UNKNOWN
	};

	/// <summary>
	/// Class DataElement. Represents a member variable
	/// </summary>
	class DataElement
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="DataElement"/> class.
		/// </summary>
		DataElement();

		/// <summary>
		/// Gets the array terminator.
		/// </summary>
		/// <returns>std.string.</returns>
		std::string GetArrayTerminator();

		/// <summary>
		/// Gets the name of the type.
		/// </summary>
		/// <returns>std.string.</returns>
		std::string GetTypeName();

		/// <summary>
		/// Gets the size of the element.
		/// </summary>
		/// <returns>int.</returns>
		int GetElementSize();

	public:
		/// <summary>
		/// The dtype_
		/// </summary>
		DATA_TYPE dtype_;
		/// <summary>
		/// The offset_
		/// </summary>
		DWORD offset_;
		/// <summary>
		/// The name_
		/// </summary>
		std::string name_;
		/// <summary>
		/// The array_size_
		/// </summary>
		int array_size_;
		/// <summary>
		/// The array_stride_
		/// </summary>
		int array_stride_;
		/// <summary>
		/// The instance_name_
		/// </summary>
		std::string instance_name_; // for class instances
	};
}
