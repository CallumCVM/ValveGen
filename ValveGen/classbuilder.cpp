
#include "classbuilder.h"
#include "client.h"

namespace valvegen
{
	ClassBuilder::ClassBuilder()
	{}

	bool ClassBuilder::CreateClasses()
	{
		auto class_head = Client::Instance()->GetAllClasses();

		if (class_head == nullptr)
			return false;

		for (; class_head != nullptr; class_head = class_head->m_pNext)
		{
			CreateNodes(class_head->m_pRecvTable, nullptr);
		}

		CreateSDK();

		return true;
	}

	ClassNode* ClassBuilder::CreateNode(RecvTable* instance, ClassNode* parent /*= nullptr*/)
	{
		std::string fixed_name = instance->m_pNetTableName;

		if (fixed_name.find("DT_") == 0)
			fixed_name.replace(fixed_name.begin(), fixed_name.begin() + 3, "C");

		if (fixed_name == "CLocalActiveWeaponData")
		{
			bool a = false;
		}

		for (auto& n : nodes_)
		{
			if (n->GetBaseName() == fixed_name)
			{
				
				if (parent)
					parent->SetParent(n);

				//return nullptr;
				return n;
			}
		}
		auto node = new ClassNode();
		nodes_.push_back(node);		

		return node;
	}

	void ClassBuilder::AddElement(RecvProp* prop, ClassNode* current_node)
	{
		if (prop->m_RecvType == ClassNode::CLASS)
		{
			if (!strcmp(prop->m_pVarName, "baseclass"))
				CreateNodes(prop->m_pDataTable, current_node);
			else
			{
				if (prop->m_pDataTable->m_pProps->m_pVarName[0] == '0')
				{
					DWORD last_array_offset = 0;
					int num_array_elements = 0;
					ClassNode::DataElement* array_member = nullptr;

					for (auto i = 0; i < prop->m_pDataTable->m_nProps; ++i)
					{
						DWORD stride = prop->m_pDataTable->m_pProps[i].m_Offset - last_array_offset;

						array_member = current_node->CreateDataElement(prop->m_Offset, prop->m_pVarName, ClassNode::DATA_TYPE::ARRAY, stride);

						last_array_offset = prop->m_pDataTable->m_pProps[i].m_Offset;

						num_array_elements++;
					}

					array_member->array_size_ = num_array_elements - 1;
				}
				else
				{
					CreateNodes(prop->m_pDataTable);

					current_node->CreateDataElementClassInstance(prop->m_Offset, prop->m_pDataTable->m_pNetTableName, prop->m_pVarName, static_cast<ClassNode::DATA_TYPE>(prop->m_RecvType));
				}

			}
		}
		else
		{
			/* Create a new variable */
			current_node->CreateDataElement(prop->m_Offset, prop->m_pVarName, static_cast<ClassNode::DATA_TYPE>(prop->m_RecvType));
		}
	}

	void ClassBuilder::CreateNodes(RecvTable* head, ClassNode* parent /*= nullptr*/)
	{
		ClassNode* current_node = CreateNode(head, parent);

		// it has already been created so ignore it
		if (!current_node)
			return;

		current_node->SetClassName(head->m_pNetTableName);

		if (head->m_pProps != nullptr)
		{
			if (current_node->GetBaseName() == "CAttributeContainer")
			{
				bool a = false;
			}

			for (auto i = 0; i < head->m_nProps; ++i)
			{
				auto prop = &head->m_pProps[i];

				if (prop == nullptr)
					continue;

				if (!strcmp(prop->m_pVarName,"m_AttributeManager"))
				{
					bool a = false;
				}

				AddElement(prop, current_node);
			}

			/* Shuffle the members so that they are in offset order */
			current_node->ShuffleMembers();
		}
	}

	void ClassBuilder::CreateSDK()
	{
		if (nodes_.size() == 0)
			return;

		/* Create SDK directory */

		for(auto& n : nodes_)
		{
			if (n->GetBaseName() == "CAttributeContainer")
			{
				bool a = false;
			}

			std::string output_file = n->GetBaseName() + ".h";

			std::ofstream of("D:\\ValveGen\\" + output_file, std::ios::out);

			if (!of.is_open())
				continue;

			n->OuputHeader(of);

			n->ShuffleParents();

			n->ResolveIncludes(of);

			/* namespace */
			of << "namespace valvegen" << std::endl << "{" << std::endl;

			of << "#pragma pack(push,1)" << std::endl;

			/* classname */
			of << "\tclass " << n->GetBaseName();

			DWORD inherited_size = 0;

			/* Get the parents in order */
			

			if (n->HasInheritence())
			{
				of << " : " << std::endl;				

				UINT number_of_parents = n->GetNumParents();
				for (UINT i = 0; i < number_of_parents; ++i)
				{
					if(number_of_parents > 1 && i < number_of_parents-1)
						of << "\t\t public " << n->GetInheritedClassName(i) << ", // 0x" << std::hex << n->GetInheritedClassSize(i) << std::endl;
					else
						of << "\t\t public " << n->GetInheritedClassName(i) << " // 0x" << std::hex << n->GetInheritedClassSize(i) << std::endl;

					/* make note of the inherited size so we can pad to it */
					if(i == number_of_parents-1)
						inherited_size = n->GetInheritedClassSize(i);
				}
			}
			else
			{
				of << std::endl;
			}

			of << "\t{" << std::endl << "\tpublic:" << std::endl;

			DWORD last_offset = inherited_size;

			for (unsigned int i = 0; i < n->GetDataElements().size(); ++i)
			{
				auto e = n->GetDataElements().at(i);

				/* check for padding */
				if (e->offset_ > last_offset)
				{
					DWORD padding = e->offset_ - last_offset;

					of << "\t\tunsigned char _0x" << std::hex << last_offset << "[0x" << std::hex << padding << "];" << std::endl;
				}

				of << "\t\t" << e->GetTypeName() << "\t" << e->name_ << e->GetArrayTerminator() << "; // 0x" << std::hex << e->offset_ << std::endl;

				last_offset = e->offset_ + e->GetElementSize();
			}

			of << "\t};" << std::endl;

			of << "#pragma pack(pop)" << std::endl;

			of << "}";

			of.close();
		}

	}

	ClassNode* ClassBuilder::FindNode(std::string name)
	{
		for (auto& n : nodes_)
		{
			if (n->GetBaseName() == name)
				return n;
		}
		return nullptr;
	}
}