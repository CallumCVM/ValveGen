

#include "dataelement.h"
#include "classbuilder.h"

namespace valvegen
{
	DataElement::DataElement() :
		dtype_(UNKNOWN),
		array_size_(0),
		array_stride_(0)
	{}

	std::string DataElement::GetArrayTerminator()
	{
		if (dtype_ != ARRAY)
			return "";

		std::stringstream ss;
		ss << "[" << (array_size_ + 1) << "]";
		return ss.str();
	}

	std::string DataElement::GetTypeName()
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

	int DataElement::GetElementSize()
	{
		DWORD size = 0;

		switch (dtype_)
		{
		case INT:
		case FLOAT:
			size += sizeof(int);
			break;

		case STRING:
			size += sizeof(PCHAR);
			break;

		case VECTOR:
			size += sizeof(float) * 3;
			break;

		case VECTORXY:
			size += sizeof(float) * 2;
			break;

		case ARRAY:
			size += (array_size_ + 1) * array_stride_;
			break;

		case CLASS:
		{
			ClassNode* class_descriptor = valvegen::ClassBuilder::Instance()->FindNode(instance_name_);

			if (class_descriptor)
				size += class_descriptor->GetClassSize();
			break;
		}
		}

		return size;
	}
}