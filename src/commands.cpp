#include "commands.h"

CommandStruct::CommandStruct() :
   m_id(t_commands::statusReq),
   m_argument("")
{
   
}

CommandStruct::CommandStruct(t_commands id, std::string argument) :
   m_id(id),
   m_argument(argument)
{
   
}

