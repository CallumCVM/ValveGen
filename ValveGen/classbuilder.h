

#pragma once

#include "required.h"
#include "singleton.h"
#include "classnode.h"
#include "client.h"

namespace valvegen
{
	/// <summary>
	/// Class ClassBuilder. 
	/// Responsible for creating the class nodes and compiling the SDK.
	/// </summary>
	/// <seealso cref="Singleton{ClassBuilder}" />
	class ClassBuilder : public Singleton<ClassBuilder>
	{
		friend class Singleton<ClassBuilder>;

	private:
		/// <summary>
		/// Prevents a default instance of the <see cref="ClassBuilder"/> class from being created.
		/// </summary>
		ClassBuilder();

	public:
		/// <summary>
		/// Cleans up this instance.
		/// </summary>
		void Cleanup();

		/// <summary>
		/// Creates the classes by iterating a list of ClientClass returned from the game egine.
		/// </summary>
		/// <returns>bool.</returns>
		bool CreateClasses(HINSTANCE module_instance);

		/// <summary>
		/// Creates the nodes recursively
		/// </summary>
		/// <param name="head">The head.</param>
		/// <param name="parent">The parent.</param>
		void CreateNodes(RecvTable* head, ClassNode* parent = nullptr);

		/// <summary>
		/// Creates a single node and set the parent accordingly (if there is one)
		/// </summary>
		/// <param name="instance">The instance.</param>
		/// <param name="parent">The parent.</param>
		/// <returns>valvegen.ClassNode *.</returns>
		ClassNode* CreateNode(RecvTable* instance, ClassNode* parent = nullptr);

		/// <summary>
		/// Finds a node by name
		/// </summary>
		/// <param name="name">The name.</param>
		/// <returns>valvegen.ClassNode *.</returns>
		ClassNode* FindNode(std::string name);

		/// <summary>
		/// Compiles all the class nodes into an SDK on disk.
		/// </summary>
		void CreateSDK(HINSTANCE module_instance);

		/// <summary>
		/// Adds a data element to a class node
		/// </summary>
		/// <param name="prop">The property.</param>
		/// <param name="current_node">The current_node.</param>
		void AddElement(RecvProp* prop, ClassNode* current_node);

	private:
		/// <summary>
		/// The vector of nodes_ we have created
		/// </summary>
		std::vector<ClassNode*> nodes_;
	};
}
