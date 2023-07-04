#include "netvar.h"
#include "../Interface/interfaces.h"
#include <iostream>

#include <ctype.h>
#include <format>

void SetupNetvars()
{

	for (auto clientClass = interfaces::g_Client->GetAllClasses(); clientClass; clientClass = clientClass->next)
	{
		if (clientClass->recvTable)
			Dump(clientClass->networkName, clientClass->recvTable, 0);
	}
}

void Dump(const char* BaseClass, RecvTable* table, std::uint32_t offset) 
{
	for (int i = 0; i < table->propsCount; i++)
	{
		auto prop = &table->props[i];
		if (!prop || isdigit(prop->varName[0]))
			continue;
		if (fnv::Hash(prop->varName) == fnv::HashConst("baseclass"))
			continue;
		if (prop->recvType == SendPropType::DATATABLE && prop->dataTable && prop->dataTable->tableName[0] == 'D')
			Dump(BaseClass, prop->dataTable, offset + prop->offset);

		const auto netvarName = std::format("{}->{}", BaseClass, prop->varName);

		if (false)
		{
			std::cout << netvarName << std::endl;
		}

		netvars[fnv::Hash(netvarName.c_str())] = offset + prop->offset;
	}
}


