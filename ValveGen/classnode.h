
#pragma once

#include "required.h"
#include "client.h"

namespace valvegen
{
	class ClassNode
	{
	public:
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

		struct DataElement
		{
			DataElement() :
				dtype_(UNKNOWN),
				array_size_(0),
				array_stride_(0)
			{}

			DATA_TYPE dtype_;
			DWORD offset_;
			std::string name_;
			int array_size_;
			int array_stride_;
			std::string instance_name_; // for class instances

			std::string GetArrayTerminator()
			{
				if (dtype_ != ARRAY)
					return "";

				std::stringstream ss;
				ss << "[" << (array_size_ + 1) << "]";
				return ss.str();
			}

			std::string GetTypeName()
			{
				switch (dtype_)
				{
				case INT:
					return "int    ";

				case FLOAT:
					return "float  ";

				case VECTOR:
					return "vector3";

				case VECTORXY:
					return "vector2";

				case STRING:
					return "char*  ";

				case ARRAY:
				{
					/* guess the type name from the field size (hacky) */
					switch (array_stride_)
					{
					default:
					case 1: return "bool   ";
					case 2: return "short  ";
					case 4: return "int    ";
					case 8: return "__int64";
					}
				}

				case CLASS:
					return instance_name_;

				default:
					return "";
				}
			}

			int GetElementSize();
		};

	public:
		ClassNode();

		void SetParent(ClassNode* parent);

		void SetClassName(std::string name);

		DataElement* CreateDataElement(DWORD offset, std::string name, DATA_TYPE dtype, DWORD stride=0);

		DataElement* CreateDataElementClassInstance(DWORD offset, std::string name, std::string varname, DATA_TYPE dtype);

		std::string GetBaseName();

		void ShuffleMembers();

		void OuputHeader(std::ofstream& of);

		void ResolveIncludes(std::ofstream& of);

		bool HasInheritence();

		std::string GetInheritedClassName(UINT index = 0);

		UINT GetNumParents();

		ClassNode* GetParent(UINT index = 0);

		std::vector<DataElement*>& GetDataElements();

		DWORD GetInheritedClassSize(UINT index = 0);

		DWORD GetClassSize() const;

		void ShuffleParents();

		void CreateInlineDeclaration(DataElement* element);

	private:
		std::vector<ClassNode*> parents_;

		std::string class_name_;

		std::vector<DataElement*> data_elements_;

		std::vector<DataElement*> inline_declarations_;
	};
}
