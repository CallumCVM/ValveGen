
#pragma once

#include "required.h"
#include "singleton.h"

namespace valvegen
{
	struct EngineInterface
	{
		LPVOID				callback;
		PCHAR				name;
		EngineInterface*	next;
	};

	class DVariant
	{
	public:
		union
		{
			float	m_Float;
			long	m_Int;
			char	*m_pString;
			void	*m_pData;
			float	m_Vector[3];
			__int64	m_Int64;
		};
		int	m_Type;
	};

	struct RecvProp;

	class CRecvProxyData
	{
	public:
		const RecvProp	*m_pRecvProp;
		DVariant		m_Value;
		int				m_iElement;
		int				m_ObjectID;
	};


	typedef void(*RecvVarProxyFn)(const CRecvProxyData *pData, void *pStruct, void *pOut);

	struct RecvTable
	{
		RecvProp		*m_pProps;
		int				m_nProps;
		void			*m_pDecoder;
		char			*m_pNetTableName;
		bool			m_bInitialized;
		bool			m_bInMainList;
	};

	struct RecvProp
	{
		char					*m_pVarName;
		int						m_RecvType;
		int						m_Flags;
		int						m_StringBufferSize;
		bool					m_bInsideArray;
		const void				*m_pExtraData;
		RecvProp				*m_pArrayProp;
		void*					m_ArrayLengthProxy;
		void*					m_ProxyFn;
		void*					m_DataTableProxyFn;
		RecvTable				*m_pDataTable;
		int						m_Offset;
		int						m_ElementStride;
		int						m_nElements;
		const char				*m_pParentArrayPropName;
	};

	struct ClientClass
	{
		void*			m_pCreateFn;
		void*			m_pCreateEventFn;
		char			*m_pNetworkName;
		RecvTable		*m_pRecvTable;
		ClientClass		*m_pNext;
		int				m_ClassID;
	};

	/// <summary>
	/// Class Client.
	/// </summary>
	/// <seealso cref="Singleton{Client}" />
	class Client : public Singleton<Client>
	{
		friend class Singleton<Client>;
	private:
		/// <summary>
		/// Prevents a default instance of the <see cref="Client"/> class from being created.
		/// </summary>
		Client();

	public:
		/// <summary>
		/// Waits for client to become ready.
		/// </summary>
		void WaitForClientToBecomeReady();

		/// <summary>
		/// Initializes the client interface.
		/// </summary>
		/// <returns>bool.</returns>
		bool InitClientInterface();

		/// <summary>
		/// Gets all classes.
		/// </summary>
		/// <returns>valvegen.ClientClass *.</returns>
		ClientClass* GetAllClasses();

	private:
		/// <summary>
		/// The client_dll_handle_
		/// </summary>
		HMODULE client_dll_handle_;
		/// <summary>
		/// The class_ptr_
		/// </summary>
		LPVOID class_ptr_;
	};
}
