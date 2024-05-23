#pragma once
#include <stdint.h>

namespace PNet
{
	enum PacketType : uint16_t
	{
		PT_Invalid,
		PT_ChatMessage,
		PT_Integer,
		PT_IntegerArray,
		PT_LogIn,
		PT_Register,
		PT_Port,
		PT_SendUserData_ServerPerspective,
		PT_SaveUserData_ServerPerspective, 
		PT_ChangeUserCredentials,
	};
}