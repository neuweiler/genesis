/// includes
#include "/includes.h"
#pragma header

#include "/Genesis.h"
#include "rev.h"
#include "Strings.h"
#include "mui.h"
#include "mui_DataBase.h"
#include "mui_PasswdReq.h"
#include "protos.h"

///
/// external variables
extern struct MUI_CustomClass *CL_PasswdReq;
extern Object *app;
extern Object *win;
extern BOOL changed_passwd, changed_group, changed_hosts, changed_protocols,
            changed_services, changed_inetd, changed_networks, changed_rpc, changed_inetaccess;
extern struct Hook des_hook;

///

/// UsersList_ConstructFunc
struct User * SAVEDS UsersList_ConstructFunc(register __a2 APTR pool, register __a1 struct User *src)
{
   struct User *new;

   if((new = (struct User *)AllocVec(sizeof(struct User), MEMF_ANY | MEMF_CLEAR)) && src && src != (APTR)-1)
      memcpy(new, src, sizeof(struct User));
   return(new);
}

///
/// UsersList_DisplayFunc
SAVEDS LONG UsersList_DisplayFunc(register __a2 char **array, register __a1 struct User *user)
{
   if(user)
   {
      static char buf1[7], buf2[7];

      *array++ = user->Login;
      *array++ = user->Password;
      sprintf(buf1, "%ld", user->UserID);
      *array++ = buf1;
      sprintf(buf2, "%ld", user->GroupID);
      *array++ = buf2;
      *array++ = user->Name;
      *array++ = user->HomeDir;
      *array++ = user->Shell;
      *array   = (user->Disabled ? "disabled" : "enabled");
   }
   else
   {
      *array++ = GetStr("  \033bLogin");
      *array++ = GetStr("  \033bencr. Password");
      *array++ = GetStr("  \033bUser ID");
      *array++ = GetStr("  \033bGroup ID");
      *array++ = GetStr("  \033bFull Name");
      *array++ = GetStr("  \033bHome dir");
      *array++ = GetStr("  \033bShell");
      *array   = GetStr("  \033bStatus");
   }
   return(NULL);
}

///
/// GroupsList_ConstructFunc
struct Group * SAVEDS GroupsList_ConstructFunc(register __a2 APTR pool, register __a1 struct Group *src)
{
   struct Group *new;

   if((new = (struct Group *)AllocVec(sizeof(struct Group), MEMF_ANY | MEMF_CLEAR)) && src && src != (APTR)-1)
      memcpy(new, src, sizeof(struct Group));
   else
      strcpy(new->Password, "*");
   return(new);
}

///
/// GroupsList_DisplayFunc
SAVEDS LONG GroupsList_DisplayFunc(register __a2 char **array, register __a1 struct Group *group)
{
   if(group)
   {
      static char buf[7];

      *array++ = group->Name;
//      *array++ = group->Password;
      sprintf(buf, "%ld", group->ID);
      *array++ = buf;
      *array   = group->Members;
   }
   else
   {
      *array++ = GetStr("  \033bName");
//      *array++ = GetStr("  \033bencr. Password");
      *array++ = GetStr("  \033bID");
      *array   = GetStr("  \033bMembers");
   }
   return(NULL);
}

///
/// ProtocolList_ConstructFunc
struct Protocol * SAVEDS ProtocolList_ConstructFunc(register __a2 APTR pool, register __a1 struct Protocol *src)
{
   struct Protocol *new;

   if((new = (struct Protocol *)AllocVec(sizeof(struct Protocol), MEMF_ANY | MEMF_CLEAR)) && src && src != (APTR)-1)
      memcpy(new, src, sizeof(struct Protocol));
   return(new);
}

///
/// ProtocolList_DisplayFunc
SAVEDS LONG ProtocolList_DisplayFunc(register __a2 char **array, register __a1 struct Protocol *protocol)
{
   if(protocol)
   {
      static char buf[7];

      sprintf(buf, "%ld", protocol->ID);
      *array++ = protocol->Name;
      *array++ = buf;
      *array   = protocol->Aliases;
   }
   else
   {
      *array++ = GetStr(MSG_TX_Name);
      *array++ = GetStr(MSG_TX_ID);
      *array   = GetStr(MSG_TX_Aliases);
   }
   return(NULL);
}

///
/// ServiceList_ConstructFunc
struct Service * SAVEDS ServiceList_ConstructFunc(register __a2 APTR pool, register __a1 struct Service *src)
{
   struct Service *new;

   if((new = (struct Service *)AllocVec(sizeof(struct Service), MEMF_ANY | MEMF_CLEAR)) && src && src != (APTR)-1)
      memcpy(new, src, sizeof(struct Service));
   return(new);
}

///
/// ServiceList_DisplayFunc
SAVEDS LONG ServiceList_DisplayFunc(register __a2 char **array, register __a1 struct Service *service)
{
   if(service)
   {
      static char buf[7];

      sprintf(buf, "%ld", service->Port);
      *array++ = service->Name;
      *array++ = buf;
      *array++ = service->Protocol;
      *array   = service->Aliases;
   }
   else
   {
      *array++ = GetStr(MSG_TX_Name);
      *array++ = GetStr(MSG_TX_Port);
      *array++ = GetStr(MSG_TX_Protocol);
      *array   = GetStr(MSG_TX_Aliases);
   }
   return(NULL);
}

///
/// InetAccessList_ConstructFunc
struct InetAccess * SAVEDS InetAccessList_ConstructFunc(register __a2 APTR pool, register __a1 struct InetAccess *src)
{
   struct InetAccess *new;

   if((new = (struct InetAccess *)AllocVec(sizeof(struct InetAccess), MEMF_ANY | MEMF_CLEAR)) && src && src != (APTR)-1)
      memcpy(new, src, sizeof(struct InetAccess));
   return(new);
}

///
/// InetAccessList_DisplayFunc
SAVEDS LONG InetAccessList_DisplayFunc(register __a2 char **array, register __a1 struct InetAccess *inet_access)
{
   if(inet_access)
   {
      *array++ = inet_access->Service;
      *array++ = inet_access->Host;
      *array++ = (inet_access->Access ? "allow" : "deny");
      *array   = (inet_access->Log ? "Log" : "");
   }
   else
   {
      *array++ = GetStr(MSG_TX_Service);
      *array++ = "\033bHost/Mask";
      *array++ = "\033bAccess";
      *array   = "\033bLog";
   }
   return(NULL);
}

///
/// InetdList_ConstructFunc
struct Inetd * SAVEDS InetdList_ConstructFunc(register __a2 APTR pool, register __a1 struct Inetd *src)
{
   struct Inetd *new;

   if((new = (struct Inetd *)AllocVec(sizeof(struct Inetd), MEMF_ANY | MEMF_CLEAR)) && src && src != (APTR)-1)
      memcpy(new, src, sizeof(struct Inetd));
   return(new);
}

///
/// InetdList_DisplayFunc
SAVEDS LONG InetdList_DisplayFunc(register __a2 char **array, register __a1 struct Inetd *inetd)
{
   if(inetd)
   {
      *array++ = inetd->Service;
      *array++ = (inetd->Socket ? "dgram" : "stream");
      *array++ = inetd->Protocol;
      *array++ = (inetd->Wait ? (inetd->Wait == 2 ? "dos" : "wait") : "nowait");
      *array++ = inetd->User;
      *array++ = inetd->Server;
      *array++ = inetd->Args;
      *array   = (inetd->Active ? GetStr(MSG_TX_Enabled) : GetStr(MSG_TX_Disabled));
   }
   else
   {
      *array++ = GetStr(MSG_TX_Service);
      *array++ = GetStr(MSG_TX_Socket);
      *array++ = GetStr(MSG_TX_Protocol);
      *array++ = GetStr(MSG_TX_Wait);
      *array++ = GetStr(MSG_TX_User);
      *array++ = GetStr(MSG_TX_Server);
      *array++ = GetStr(MSG_TX_Args);
      *array   = GetStr(MSG_TX_Status);
   }
   return(NULL);
}

///
/// HostList_ConstructFunc
struct Host * SAVEDS HostList_ConstructFunc(register __a2 APTR pool, register __a1 struct Host *src)
{
   struct Host *new;

   if((new = (struct Host *)AllocVec(sizeof(struct Host), MEMF_ANY | MEMF_CLEAR)) && src && src != (APTR)-1)
      memcpy(new, src, sizeof(struct Host));
   return(new);
}

///
/// HostList_DisplayFunc
SAVEDS LONG HostList_DisplayFunc(register __a2 char **array, register __a1 struct Host *host)
{
   if(host)
   {
      *array++ = host->Addr;
      *array++ = host->Name;
      *array   = host->Aliases;
   }
   else
   {
      *array++ = GetStr(MSG_TX_IPAddr);
      *array++ = GetStr(MSG_TX_Name);
      *array   = GetStr(MSG_TX_Aliases);
   }
   return(NULL);
}

///
/// NetworkList_ConstructFunc
struct Network * SAVEDS NetworkList_ConstructFunc(register __a2 APTR pool, register __a1 struct Network *src)
{
   struct Network *new;

   if((new = (struct Network *)AllocVec(sizeof(struct Network), MEMF_ANY | MEMF_CLEAR)) && src && src != (APTR)-1)
      memcpy(new, src, sizeof(struct Network));
   return(new);
}

///
/// NetworkList_DisplayFunc
SAVEDS LONG NetworkList_DisplayFunc(register __a2 char **array, register __a1 struct Network *network)
{
   if(network)
   {
      static char buf[11];

      sprintf(buf, "%ld", network->Number);
      *array++ = network->Name;
      *array++ = buf;
      *array   = network->Aliases;
   }
   else
   {
      *array++ = GetStr(MSG_TX_Name);
      *array++ = GetStr(MSG_TX_ID);
      *array   = GetStr(MSG_TX_Aliases);
   }
   return(NULL);
}

///
/// RpcList_ConstructFunc
struct Rpc * SAVEDS RpcList_ConstructFunc(register __a2 APTR pool, register __a1 struct Rpc *src)
{
   struct Rpc *new;

   if((new = (struct Rpc *)AllocVec(sizeof(struct Rpc), MEMF_ANY | MEMF_CLEAR)) && src && src != (APTR)-1)
      memcpy(new, src, sizeof(struct Rpc));
   return(new);
}

///
/// RpcList_DisplayFunc
SAVEDS LONG RpcList_DisplayFunc(register __a2 char **array, register __a1 struct Rpc *rpc)
{
   if(rpc)
   {
      static char buf[11];

      sprintf(buf, "%ld", rpc->Number);
      *array++ = rpc->Name;
      *array++ = buf;
      *array   = rpc->Aliases;
   }
   else
   {
      *array++ = GetStr(MSG_TX_Name);
      *array++ = GetStr(MSG_TX_ID);
      *array   = GetStr(MSG_TX_Aliases);
   }
   return(NULL);
}

///

/// protocol_pos
LONG protocol_pos(Object *list, STRPTR string)
{
   LONG pos = 0;
   struct Protocol *protocol;

   FOREVER
   {
      DoMethod(list, MUIM_NList_GetEntry, pos, &protocol);
      if(!protocol)
         break;
      if(!stricmp(protocol->Name, string))
         break;
      pos++;
   }
   return(protocol ? pos : MUIV_NList_Active_Off);
}

///

/// Databases_PasswdReqFinish
ULONG Databases_PasswdReqFinish(struct IClass *cl, Object *obj, struct MUIP_Genesis_Finish *msg)
{
   struct Databases_Data *data = INST_DATA(cl, obj);

   if(msg->status == 1)
   {
      struct PasswdReq_Data *pw_data = INST_DATA(CL_PasswdReq->mcc_Class, msg->obj);

      if(strcmp((STRPTR)xget(pw_data->STR_pw1, MUIA_String_Contents), (STRPTR)xget(pw_data->STR_pw2, MUIA_String_Contents)))
      {
         MUI_Request(app, win, NULL, NULL, "*_Okay", "The passwords you have\nentered are not equal.");
         setstring(pw_data->STR_pw1, "");
         setstring(pw_data->STR_pw2, "");
         set(msg->obj, MUIA_Window_ActiveObject, pw_data->STR_pw1);
         return(NULL);
      }
      else
      {
         struct User *user;

         DoMethod(data->LV_Users, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &user);
         if(user && strlen((STRPTR)xget(pw_data->STR_pw1, MUIA_String_Contents)))
         {
            if(!UserGroupBase)
               UserGroupBase = OpenLibrary(USERGROUPNAME, 0);
               
            if(UserGroupBase)
            {
               strcpy(user->Password, crypt((STRPTR)xget(pw_data->STR_pw1, MUIA_String_Contents), user->Password));
               CloseLibrary(UserGroupBase);
               UserGroupBase = NULL;
            }
         }
         else
            MUI_Request(app, win, NULL, NULL, "*_Abort", "Unable to change password.");
      }
      DoMethod(data->LV_Users, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
   }
   set(msg->obj, MUIA_Window_Open, FALSE);
   DoMethod(app, OM_REMMEMBER, msg->obj),
   MUI_DisposeObject(msg->obj);
   set(app, MUIA_Application_Sleep, FALSE);

   return(NULL);
}

///
/// Databases_SetStates
ULONG Databases_SetStates(struct IClass *cl, Object *obj, struct MUIP_Databases_SetStates *msg)
{
   struct Databases_Data *data = INST_DATA(cl, obj);

   switch(msg->page)
   {
      case MUIV_Databases_SetStates_Users:
      {
         struct User *user;

         changed_passwd = TRUE;

         DoMethod(data->LV_Users, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &user);
         if(user)
         {
            set(data->BT_ChangePassword, MUIA_Disabled, user->Disabled);
            set(data->BT_RemovePassword, MUIA_Disabled, !*user->Password || user->Disabled);

            nnset(data->STR_Login   , MUIA_String_Contents  , user->Login);
            nnset(data->STR_Name    , MUIA_String_Contents  , user->Name);
            nnset(data->PA_HomeDir  , MUIA_String_Contents  , user->HomeDir);
            nnset(data->STR_Shell   , MUIA_String_Contents  , user->Shell);
            set(data->CH_Disabled   , MUIA_Selected         , user->Disabled);
            nnset(data->STR_UserID  , MUIA_String_Integer   , user->UserID);
            nnset(data->STR_GroupID , MUIA_String_Integer   , user->GroupID);
         }
         else
         {
            set(data->BT_ChangePassword, MUIA_Disabled, TRUE);
            set(data->BT_RemovePassword, MUIA_Disabled, TRUE);

            nnset(data->STR_Login   , MUIA_String_Contents  , "");
            nnset(data->STR_Name    , MUIA_String_Contents  , "");
            nnset(data->PA_HomeDir  , MUIA_String_Contents  , "");
            nnset(data->STR_Shell   , MUIA_String_Contents  , "");
            nnset(data->CH_Disabled , MUIA_Selected         , FALSE);
            nnset(data->STR_UserID  , MUIA_String_Integer   , 0);
            nnset(data->STR_GroupID , MUIA_String_Integer   , 0);
         }
         set(data->BT_RemoveUser , MUIA_Disabled, !user);
         set(data->STR_Login     , MUIA_Disabled, !user);
         set(data->STR_Name      , MUIA_Disabled, !user);
         set(data->PA_HomeDir    , MUIA_Disabled, !user);
         set(data->STR_Shell     , MUIA_Disabled, !user);
         set(data->CH_Disabled   , MUIA_Disabled, !user);
         set(data->STR_UserID    , MUIA_Disabled, !user);
         set(data->STR_GroupID   , MUIA_Disabled, !user);
         set(win, MUIA_Window_ActiveObject, data->STR_Login);
      }
      break;

      case MUIV_Databases_SetStates_Groups:
      {
         struct Group *group;

         changed_group = TRUE;

         DoMethod(data->LV_Groups, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &group);
         if(group)
         {
            nnset(data->STR_GroupName   , MUIA_String_Contents  , group->Name);
            nnset(data->STR_GroupNumber , MUIA_String_Integer   , group->ID);
            nnset(data->STR_GroupMembers, MUIA_String_Contents  , group->Members);
         }
         else
         {
            nnset(data->STR_GroupName   , MUIA_String_Contents  , "");
            nnset(data->STR_GroupNumber , MUIA_String_Integer   , 0);
            nnset(data->STR_GroupMembers, MUIA_String_Integer   , "");
         }
         set(data->BT_RemoveGroup      , MUIA_Disabled, !group);
         set(data->STR_GroupName       , MUIA_Disabled, !group);
         set(data->STR_GroupNumber     , MUIA_Disabled, !group);
         set(data->STR_GroupMembers    , MUIA_Disabled, !group);
         set(win, MUIA_Window_ActiveObject, data->STR_GroupName);
      }
      break;

      case MUIV_Databases_SetStates_Protocols:
      {
         struct Protocol *protocol;
         LONG pos;

         changed_protocols = TRUE;

         DoMethod(data->LV_Protocols, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &protocol);
         if(protocol)
         {
            set(data->STR_ProtocolName, MUIA_String_Contents, protocol->Name);
            set(data->STR_ProtocolID, MUIA_String_Integer, protocol->ID);
            set(data->STR_ProtocolAliases, MUIA_String_Contents, protocol->Aliases);
         }
         else
         {
            set(data->STR_ProtocolName, MUIA_String_Contents, "");
            set(data->STR_ProtocolID, MUIA_String_Integer, 0);
            set(data->STR_ProtocolAliases, MUIA_String_Contents, "");
         }
         set(data->BT_RemoveProtocol, MUIA_Disabled, !protocol);
         set(data->STR_ProtocolName, MUIA_Disabled, !protocol);
         set(data->STR_ProtocolID, MUIA_Disabled, !protocol);
         set(data->STR_ProtocolAliases, MUIA_Disabled, !protocol);

         // copy the protocol names to the two other lists
         set(data->LV_Services, MUIA_NList_Active, MUIV_NList_Active_Off);
         set(data->LV_Inetd, MUIA_NList_Active, MUIV_NList_Active_Off);
         DoMethod(data->LV_ServiceProtocol, MUIM_NList_Clear);
         DoMethod(data->LV_InetdProtocol, MUIM_NList_Clear);
         pos = 0;
         FOREVER
         {
            DoMethod(data->LV_Protocols, MUIM_NList_GetEntry, pos++, &protocol);
            if(!protocol)
               break;
            DoMethod(data->LV_ServiceProtocol, MUIM_NList_InsertSingle, protocol->Name, MUIV_NList_Insert_Bottom);
            DoMethod(data->LV_InetdProtocol, MUIM_NList_InsertSingle, protocol->Name, MUIV_NList_Insert_Bottom);
         }
         set(win, MUIA_Window_ActiveObject, data->STR_ProtocolName);
      }
         break;

      case MUIV_Databases_SetStates_Services:
      {
         struct Service *service;

         changed_services = TRUE;

         DoMethod(data->LV_Services, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &service);
         if(service)
         {
            set(data->STR_ServiceName, MUIA_String_Contents, service->Name);
            set(data->STR_ServicePort, MUIA_String_Integer, service->Port);
            set(data->STR_ServiceAliases, MUIA_String_Contents, service->Aliases);
            set(data->LV_ServiceProtocol, MUIA_NList_Active, protocol_pos(data->LV_ServiceProtocol, service->Protocol));
         }
         else
         {
            set(data->STR_ServiceName, MUIA_String_Contents, "");
            set(data->STR_ServicePort, MUIA_String_Integer, 0);
            set(data->STR_ServiceAliases, MUIA_String_Contents, "");
            set(data->LV_ServiceProtocol, MUIA_NList_Active, MUIV_NList_Active_Off);
         }
         set(data->BT_RemoveService, MUIA_Disabled, !service);
         set(data->STR_ServiceName, MUIA_Disabled, !service);
         set(data->STR_ServicePort, MUIA_Disabled, !service);
         set(data->STR_ServiceAliases, MUIA_Disabled, !service);
         set(data->LV_ServiceProtocol, MUIA_Disabled, !service);
         set(win, MUIA_Window_ActiveObject, data->STR_ServiceName);
      }
         break;

      case MUIV_Databases_SetStates_InetAccess:
      {
         struct InetAccess *inet_access;

         changed_inetaccess = TRUE;

         DoMethod(data->LV_InetAccess, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inet_access);
         if(inet_access)
         {
            set(data->STR_InetAccessService, MUIA_String_Contents, inet_access->Service);
            set(data->STR_InetAccessHost, MUIA_String_Contents, inet_access->Host);
            set(data->CY_InetAccess, MUIA_Cycle_Active, (inet_access->Access ? 0 : 1));
            set(data->CH_InetAccessLog, MUIA_Selected, inet_access->Log);
         }
         else
         {
            set(data->STR_InetAccessService, MUIA_String_Contents, "");
            set(data->STR_InetAccessHost, MUIA_String_Contents, "");
            set(data->CY_InetAccess, MUIA_Cycle_Active, 0);
            set(data->CH_InetAccessLog, MUIA_Selected, FALSE);
         }
         set(data->BT_RemoveInetAccess, MUIA_Disabled, !inet_access);
         set(data->STR_InetAccessService, MUIA_Disabled, !inet_access);
         set(data->STR_InetAccessHost, MUIA_Disabled, !inet_access);
         set(data->CY_InetAccess, MUIA_Disabled, !inet_access);
         set(data->CH_InetAccessLog, MUIA_Disabled, !inet_access);
         set(win, MUIA_Window_ActiveObject, data->STR_InetAccessService);
      }
         break;

      case MUIV_Databases_SetStates_Inetd:
      {
         struct Inetd *inetd;

         changed_inetd = TRUE;

         DoMethod(data->LV_Inetd, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inetd);
         if(inetd)
         {
            set(data->STR_InetdService, MUIA_String_Contents, inetd->Service);
            set(data->STR_InetdUser, MUIA_String_Contents, inetd->User);
            set(data->PA_InetdServer, MUIA_String_Contents, inetd->Server);
            set(data->STR_InetdArgs, MUIA_String_Contents, inetd->Args);
            set(data->CH_InetdActive, MUIA_Selected, inetd->Active);
            set(data->LV_InetdProtocol, MUIA_NList_Active, protocol_pos(data->LV_InetdProtocol, inetd->Protocol));
            set(data->CY_InetdSocket, MUIA_Cycle_Active, inetd->Socket);
            set(data->CY_InetdWait, MUIA_Cycle_Active, inetd->Wait);
         }
         else
         {
            set(data->STR_InetdService, MUIA_String_Contents, "");
            set(data->STR_InetdUser, MUIA_String_Contents, "");
            set(data->PA_InetdServer, MUIA_String_Contents, "");
            set(data->STR_InetdArgs, MUIA_String_Contents, "");
            set(data->CH_InetdActive, MUIA_Selected, FALSE);
            set(data->LV_InetdProtocol, MUIA_NList_Active, MUIV_NList_Active_Off);
            set(data->CY_InetdSocket, MUIA_Cycle_Active, 0);
            set(data->CY_InetdWait, MUIA_Cycle_Active, 0);
         }
         set(data->BT_RemoveInetd, MUIA_Disabled, !inetd);
         set(data->STR_InetdService, MUIA_Disabled, !inetd);
         set(data->STR_InetdUser, MUIA_Disabled, !inetd);
         set(data->PA_InetdServer, MUIA_Disabled, !inetd);
         set(data->STR_InetdArgs, MUIA_Disabled, !inetd);
         set(data->CH_InetdActive, MUIA_Disabled, !inetd);
         set(data->LV_InetdProtocol, MUIA_Disabled, !inetd);
         set(data->CY_InetdSocket, MUIA_Disabled, !inetd);
         set(data->CY_InetdWait, MUIA_Disabled, !inetd);
         set(win, MUIA_Window_ActiveObject, data->STR_InetdService);
      }
         break;

      case MUIV_Databases_SetStates_Hosts:
      {
         struct Host *host;

         changed_hosts = TRUE;

         DoMethod(data->LV_Hosts, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &host);
         if(host)
         {
            set(data->STR_HostAddr, MUIA_String_Contents, host->Addr);
            set(data->STR_HostName, MUIA_String_Contents, host->Name);
            set(data->STR_HostAliases, MUIA_String_Contents, host->Aliases);
         }
         else
         {
            set(data->STR_HostAddr, MUIA_String_Contents, 0);
            set(data->STR_HostName, MUIA_String_Contents, "");
            set(data->STR_HostAliases, MUIA_String_Contents, "");
         }
         set(data->BT_RemoveHost, MUIA_Disabled, !host);
         set(data->STR_HostAddr, MUIA_Disabled, !host);
         set(data->STR_HostName, MUIA_Disabled, !host);
         set(data->STR_HostAliases, MUIA_Disabled, !host);
         set(win, MUIA_Window_ActiveObject, data->STR_HostAddr);
      }
         break;

      case MUIV_Databases_SetStates_Networks:
      {
         struct Network *network;

         changed_networks = TRUE;

         DoMethod(data->LV_Networks, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &network);
         if(network)
         {
            set(data->STR_NetworkName, MUIA_String_Contents, network->Name);
            set(data->STR_NetworkID, MUIA_String_Integer, network->Number);
            set(data->STR_NetworkAliases, MUIA_String_Contents, network->Aliases);
         }
         else
         {
            set(data->STR_NetworkName, MUIA_String_Contents, "");
            set(data->STR_NetworkID, MUIA_String_Integer, 0);
            set(data->STR_NetworkAliases, MUIA_String_Contents, "");
         }
         set(data->BT_RemoveNetwork, MUIA_Disabled, !network);
         set(data->STR_NetworkName, MUIA_Disabled, !network);
         set(data->STR_NetworkID, MUIA_Disabled, !network);
         set(data->STR_NetworkAliases, MUIA_Disabled, !network);
         set(win, MUIA_Window_ActiveObject, data->STR_NetworkName);
      }
         break;

      case MUIV_Databases_SetStates_Rpcs:
      {
         struct Rpc *rpc;

         changed_rpc = TRUE;

         DoMethod(data->LV_Rpcs, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &rpc);
         if(rpc)
         {
            set(data->STR_RpcName, MUIA_String_Contents, rpc->Name);
            set(data->STR_RpcID, MUIA_String_Integer, rpc->Number);
            set(data->STR_RpcAliases, MUIA_String_Contents, rpc->Aliases);
         }
         else
         {
            set(data->STR_RpcName, MUIA_String_Contents, "");
            set(data->STR_RpcID, MUIA_String_Integer, 0);
            set(data->STR_RpcAliases, MUIA_String_Contents, "");
         }
         set(data->BT_RemoveRpc, MUIA_Disabled, !rpc);
         set(data->STR_RpcName, MUIA_Disabled, !rpc);
         set(data->STR_RpcID, MUIA_Disabled, !rpc);
         set(data->STR_RpcAliases, MUIA_Disabled, !rpc);
         set(win, MUIA_Window_ActiveObject, data->STR_RpcName);
      }
         break;
   }
   return(NULL);
}

///
/// Databases_Modification
ULONG Databases_Modification(struct IClass *cl, Object *obj, struct MUIP_Databases_Modification *msg)
{
   struct Databases_Data *data = INST_DATA(cl, obj);
   struct User *user;
   struct Group *group;
   struct Protocol *protocol;
   struct Service *service;
   struct InetAccess *inet_access;
   struct Inetd *inetd;
   struct Host *host;
   struct Network *network;
   struct Rpc *rpc;

   switch(msg->what)
   {
      case MUIV_Databases_Modification_NewUser:
         DoMethod(data->LV_Users, MUIM_NList_InsertSingle, -1, MUIV_NList_Insert_Bottom);
         set(data->LV_Users, MUIA_NList_Active, MUIV_NList_Active_Bottom);
         set(win, MUIA_Window_ActiveObject, data->STR_Login);
         break;
      case MUIV_Databases_Modification_Login:
         DoMethod(data->LV_Users, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &user);
         if(user)
         {
            strcpy(user->Login, (STRPTR)xget(data->STR_Login, MUIA_String_Contents));
            DoMethod(data->LV_Users, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_FullName:
         DoMethod(data->LV_Users, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &user);
         if(user)
            strcpy(user->Name, (STRPTR)xget(data->STR_Name, MUIA_String_Contents));
         DoMethod(data->LV_Users, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         break;
      case MUIV_Databases_Modification_HomeDir:
         DoMethod(data->LV_Users, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &user);
         if(user)
            strcpy(user->HomeDir, (STRPTR)xget(data->PA_HomeDir, MUIA_String_Contents));
         DoMethod(data->LV_Users, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         break;
      case MUIV_Databases_Modification_Shell:
         DoMethod(data->LV_Users, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &user);
         if(user)
            strcpy(user->Shell, (STRPTR)xget(data->STR_Shell, MUIA_String_Contents));
         DoMethod(data->LV_Users, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         break;
      case MUIV_Databases_Modification_UserID:
         DoMethod(data->LV_Users, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &user);
         if(user)
            user->UserID = xget(data->STR_UserID, MUIA_String_Integer);
         DoMethod(data->LV_Users, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         break;
      case MUIV_Databases_Modification_GroupID:
         DoMethod(data->LV_Users, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &user);
         if(user)
            user->GroupID = xget(data->STR_GroupID, MUIA_String_Integer);
         DoMethod(data->LV_Users, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         break;
      case MUIV_Databases_Modification_Disable:
         DoMethod(data->LV_Users, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &user);
         if(user)
         {
            user->Disabled = xget(data->CH_Disabled, MUIA_Selected);
            set(data->BT_ChangePassword, MUIA_Disabled, user->Disabled);
            set(data->BT_RemovePassword, MUIA_Disabled, !*user->Password || user->Disabled);
            DoMethod(data->LV_Users, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_ChangePassword:
      {
         Object *window;

         set(app, MUIA_Application_Sleep, TRUE);
         if(window = NewObject(CL_PasswdReq->mcc_Class, NULL, MUIA_Genesis_Originator, obj, TAG_DONE))
         {
            DoMethod(app, OM_ADDMEMBER, window);
            set(window, MUIA_Window_Open, TRUE);
         }
      }
      break;
      case MUIV_Databases_Modification_RemovePassword:
         DoMethod(data->LV_Users, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &user);
         if(user)
         {
            if(MUI_Request(app, win, NULL, NULL, GetStr(MSG_BT_ReallyRemovePasswd), GetStr(MSG_TX_ReallyRemovePasswd)))
            {
               MUI_Request(app, win, NULL, NULL, GetStr(MSG_BT_Okay), GetStr(MSG_TX_PasswordWarning));
               *user->Password = NULL;
               set(data->BT_RemovePassword, MUIA_Disabled, TRUE);
            }
            DoMethod(data->LV_Users, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;


      case MUIV_Databases_Modification_NewGroup:
         DoMethod(data->LV_Groups, MUIM_NList_InsertSingle, -1, MUIV_NList_Insert_Bottom);
         set(data->LV_Groups, MUIA_NList_Active, MUIV_NList_Active_Bottom);
         set(win, MUIA_Window_ActiveObject, data->STR_GroupName);
         break;
      case MUIV_Databases_Modification_GroupName:
         DoMethod(data->LV_Groups, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &group);
         if(group)
         {
            strcpy(group->Name, (STRPTR)xget(data->STR_GroupName, MUIA_String_Contents));
            DoMethod(data->LV_Groups, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_GroupNumber:
         DoMethod(data->LV_Groups, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &group);
         if(group)
            group->ID = xget(data->STR_GroupNumber, MUIA_String_Integer);
         DoMethod(data->LV_Groups, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         break;
      case MUIV_Databases_Modification_GroupMembers:
         DoMethod(data->LV_Groups, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &group);
         if(group)
            strcpy(group->Members, (STRPTR)xget(data->STR_GroupMembers, MUIA_String_Contents));
         DoMethod(data->LV_Groups, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         break;


      case MUIV_Databases_Modification_ProtocolName:
         DoMethod(data->LV_Protocols, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &protocol);
         if(protocol)
         {
            strcpy(protocol->Name, (STRPTR)xget(data->STR_ProtocolName, MUIA_String_Contents));
            DoMethod(data->LV_Protocols, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_ProtocolID:
         DoMethod(data->LV_Protocols, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &protocol);
         if(protocol)
         {
            protocol->ID = xget(data->STR_ProtocolID, MUIA_String_Integer);
            DoMethod(data->LV_Protocols, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_ProtocolAliases:
         DoMethod(data->LV_Protocols, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &protocol);
         if(protocol)
         {
            strcpy(protocol->Aliases, (STRPTR)xget(data->STR_ProtocolAliases, MUIA_String_Contents));
            DoMethod(data->LV_Protocols, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;


      case MUIV_Databases_Modification_ServiceName:
         DoMethod(data->LV_Services, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &service);
         if(service)
         {
            strcpy(service->Name, (STRPTR)xget(data->STR_ServiceName, MUIA_String_Contents));
            DoMethod(data->LV_Services, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_ServicePort:
         DoMethod(data->LV_Services, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &service);
         if(service)
         {
            service->Port = xget(data->STR_ServicePort, MUIA_String_Integer);
            DoMethod(data->LV_Services, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_ServiceAliases:
         DoMethod(data->LV_Services, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &service);
         if(service)
         {
            strcpy(service->Aliases, (STRPTR)xget(data->STR_ServiceAliases, MUIA_String_Contents));
            DoMethod(data->LV_Services, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_ServiceProtocol:
         DoMethod(data->LV_Services, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &service);
         if(service)
         {
            DoMethod(data->LV_ServiceProtocol, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &protocol);
            if(protocol)
            {
               strcpy(service->Protocol, protocol->Name);
               DoMethod(data->LV_Services, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
            }
         }
         break;


      case MUIV_Databases_Modification_InetAccessService:
         DoMethod(data->LV_InetAccess, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inet_access);
         if(inet_access)
         {
            strcpy(inet_access->Service, (STRPTR)xget(data->STR_InetAccessService, MUIA_String_Contents));
            DoMethod(data->LV_InetAccess, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_InetAccessHost:
         DoMethod(data->LV_InetAccess, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inet_access);
         if(inet_access)
         {
            strcpy(inet_access->Host, (STRPTR)xget(data->STR_InetAccessHost, MUIA_String_Contents));
            DoMethod(data->LV_InetAccess, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_InetAccess:
         DoMethod(data->LV_InetAccess, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inet_access);
         if(inet_access)
         {
            inet_access->Access = !xget(data->CY_InetAccess, MUIA_Cycle_Active);
            DoMethod(data->LV_InetAccess, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_InetAccessLog:
         DoMethod(data->LV_InetAccess, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inet_access);
         if(inet_access)
         {
            inet_access->Log = xget(data->CH_InetAccessLog, MUIA_Selected);
            DoMethod(data->LV_InetAccess, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;


      case MUIV_Databases_Modification_InetdService:
         DoMethod(data->LV_Inetd, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inetd);
         if(inetd)
         {
            strcpy(inetd->Service, (STRPTR)xget(data->STR_InetdService, MUIA_String_Contents));
            DoMethod(data->LV_Inetd, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_InetdUser:
         DoMethod(data->LV_Inetd, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inetd);
         if(inetd)
         {
            strcpy(inetd->User, (STRPTR)xget(data->STR_InetdUser, MUIA_String_Contents));
            DoMethod(data->LV_Inetd, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_InetdServer:
         DoMethod(data->LV_Inetd, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inetd);
         if(inetd)
         {
            strcpy(inetd->Server, (STRPTR)xget(data->PA_InetdServer, MUIA_String_Contents));
            DoMethod(data->LV_Inetd, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_InetdArgs:
         DoMethod(data->LV_Inetd, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inetd);
         if(inetd)
         {
            strcpy(inetd->Args, (STRPTR)xget(data->STR_InetdArgs, MUIA_String_Contents));
            DoMethod(data->LV_Inetd, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_InetdActive:
         DoMethod(data->LV_Inetd, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inetd);
         if(inetd)
         {
            inetd->Active = xget(data->CH_InetdActive, MUIA_Selected);
            DoMethod(data->LV_Inetd, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_InetdProtocol:
         DoMethod(data->LV_Inetd, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inetd);
         if(inetd)
         {
            DoMethod(data->LV_InetdProtocol, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &protocol);
            if(protocol)
            {
               strcpy(inetd->Protocol, protocol->Name);
               DoMethod(data->LV_Inetd, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
            }
         }
         break;
      case MUIV_Databases_Modification_InetdSocket:
         DoMethod(data->LV_Inetd, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inetd);
         if(inetd)
         {
            inetd->Socket = xget(data->CY_InetdSocket, MUIA_Cycle_Active);
            DoMethod(data->LV_Inetd, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_InetdWait:
         DoMethod(data->LV_Inetd, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &inetd);
         if(inetd)
         {
            inetd->Wait = xget(data->CY_InetdWait, MUIA_Cycle_Active);
            DoMethod(data->LV_Inetd, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;


      case MUIV_Databases_Modification_HostAddr:
         DoMethod(data->LV_Hosts, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &host);
         if(host)
         {
            strcpy(host->Addr, (STRPTR)xget(data->STR_HostAddr, MUIA_String_Contents));
            DoMethod(data->LV_Hosts, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_HostName:
         DoMethod(data->LV_Hosts, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &host);
         if(host)
         {
            strcpy(host->Name, (STRPTR)xget(data->STR_HostName, MUIA_String_Contents));
            DoMethod(data->LV_Hosts, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_HostAliases:
         DoMethod(data->LV_Hosts, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &host);
         if(host)
         {
            strcpy(host->Aliases, (STRPTR)xget(data->STR_HostAliases, MUIA_String_Contents));
            DoMethod(data->LV_Hosts, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;


      case MUIV_Databases_Modification_NetworkName:
         DoMethod(data->LV_Networks, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &network);
         if(network)
         {
            strcpy(network->Name, (STRPTR)xget(data->STR_NetworkName, MUIA_String_Contents));
            DoMethod(data->LV_Networks, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_NetworkID:
         DoMethod(data->LV_Networks, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &network);
         if(network)
         {
            network->Number = xget(data->STR_NetworkID, MUIA_String_Integer);
            DoMethod(data->LV_Networks, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_NetworkAliases:
         DoMethod(data->LV_Networks, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &network);
         if(network)
         {
            strcpy(network->Aliases, (STRPTR)xget(data->STR_NetworkAliases, MUIA_String_Contents));
            DoMethod(data->LV_Networks, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;


      case MUIV_Databases_Modification_RpcName:
         DoMethod(data->LV_Rpcs, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &rpc);
         if(rpc)
         {
            strcpy(rpc->Name, (STRPTR)xget(data->STR_RpcName, MUIA_String_Contents));
            DoMethod(data->LV_Rpcs, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_RpcID:
         DoMethod(data->LV_Rpcs, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &rpc);
         if(rpc)
         {
            rpc->Number = xget(data->STR_RpcID, MUIA_String_Integer);
            DoMethod(data->LV_Rpcs, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;
      case MUIV_Databases_Modification_RpcAliases:
         DoMethod(data->LV_Rpcs, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &rpc);
         if(rpc)
         {
            strcpy(rpc->Aliases, (STRPTR)xget(data->STR_RpcAliases, MUIA_String_Contents));
            DoMethod(data->LV_Rpcs, MUIM_NList_Redraw, MUIV_NList_Redraw_Active);
         }
         break;


      case MUIV_Databases_Modification_NewProtocol:
         DoMethod(data->LV_Protocols, MUIM_NList_InsertSingle, -1, MUIV_NList_Insert_Bottom);
         set(data->LV_Protocols, MUIA_NList_Active, MUIV_NList_Active_Bottom);
         break;
      case MUIV_Databases_Modification_NewService:
         DoMethod(data->LV_Services, MUIM_NList_InsertSingle, -1, MUIV_NList_Insert_Bottom);
         set(data->LV_Services, MUIA_NList_Active, MUIV_NList_Active_Bottom);
         break;
      case MUIV_Databases_Modification_NewInetAccess:
         DoMethod(data->LV_InetAccess, MUIM_NList_InsertSingle, -1, MUIV_NList_Insert_Bottom);
         set(data->LV_InetAccess, MUIA_NList_Active, MUIV_NList_Active_Bottom);
         break;
      case MUIV_Databases_Modification_NewInetd:
         DoMethod(data->LV_Inetd, MUIM_NList_InsertSingle, -1, MUIV_NList_Insert_Bottom);
         set(data->LV_Inetd, MUIA_NList_Active, MUIV_NList_Active_Bottom);
         break;
      case MUIV_Databases_Modification_NewHost:
         DoMethod(data->LV_Hosts, MUIM_NList_InsertSingle, -1, MUIV_NList_Insert_Bottom);
         set(data->LV_Hosts, MUIA_NList_Active, MUIV_NList_Active_Bottom);
         break;
      case MUIV_Databases_Modification_NewNetwork:
         DoMethod(data->LV_Networks, MUIM_NList_InsertSingle, -1, MUIV_NList_Insert_Bottom);
         set(data->LV_Networks, MUIA_NList_Active, MUIV_NList_Active_Bottom);
         break;
      case MUIV_Databases_Modification_NewRpc:
         DoMethod(data->LV_Rpcs, MUIM_NList_InsertSingle, -1, MUIV_NList_Insert_Bottom);
         set(data->LV_Rpcs, MUIA_NList_Active, MUIV_NList_Active_Bottom);
         break;
   }
   return(NULL);
}

///
/// Databases_New
ULONG Databases_New(struct IClass *cl, Object *obj, struct opSet *msg)
{
   static const struct Hook UsersList_ConstructHook= { { 0,0 }, (VOID *)UsersList_ConstructFunc , NULL, NULL };
   static const struct Hook UsersList_DisplayHook  = { { 0,0 }, (VOID *)UsersList_DisplayFunc   , NULL, NULL };
   static const struct Hook GroupsList_ConstructHook= { { 0,0 }, (VOID *)GroupsList_ConstructFunc , NULL, NULL };
   static const struct Hook GroupsList_DisplayHook  = { { 0,0 }, (VOID *)GroupsList_DisplayFunc   , NULL, NULL };
   static const struct Hook ProtocolList_ConstructHook= { { 0,0 }, (VOID *)ProtocolList_ConstructFunc , NULL, NULL };
   static const struct Hook ProtocolList_DisplayHook  = { { 0,0 }, (VOID *)ProtocolList_DisplayFunc   , NULL, NULL };
   static const struct Hook ServiceList_ConstructHook = { { 0,0 }, (VOID *)ServiceList_ConstructFunc  , NULL, NULL };
   static const struct Hook ServiceList_DisplayHook   = { { 0,0 }, (VOID *)ServiceList_DisplayFunc    , NULL, NULL };
   static const struct Hook InetdList_ConstructHook   = { { 0,0 }, (VOID *)InetdList_ConstructFunc , NULL, NULL };
   static const struct Hook InetdList_DisplayHook  = { { 0,0 }, (VOID *)InetdList_DisplayFunc      , NULL, NULL };
   static const struct Hook InetAccessList_ConstructHook   = { { 0,0 }, (VOID *)InetAccessList_ConstructFunc , NULL, NULL };
   static const struct Hook InetAccessList_DisplayHook  = { { 0,0 }, (VOID *)InetAccessList_DisplayFunc      , NULL, NULL };
   static const struct Hook HostList_ConstructHook= { { 0,0 }, (VOID *)HostList_ConstructFunc   , NULL, NULL };
   static const struct Hook HostList_DisplayHook   = { { 0,0 }, (VOID *)HostList_DisplayFunc , NULL, NULL };
   static const struct Hook NetworkList_ConstructHook= { { 0,0 }, (VOID *)NetworkList_ConstructFunc   , NULL, NULL };
   static const struct Hook NetworkList_DisplayHook   = { { 0,0 }, (VOID *)NetworkList_DisplayFunc , NULL, NULL };
   static const struct Hook RpcList_ConstructHook= { { 0,0 }, (VOID *)RpcList_ConstructFunc  , NULL, NULL };
   static const struct Hook RpcList_DisplayHook = { { 0,0 }, (VOID *)RpcList_DisplayFunc  , NULL, NULL };
   static STRPTR STR_CY_Access[3];
   static STRPTR STR_CY_Socket[3];
   static STRPTR STR_CY_Wait[4];
   struct Databases_Data tmp;

   STR_CY_Access[0] = "allow";
   STR_CY_Access[1] = "deny";
   STR_CY_Access[2] = NULL;

   STR_CY_Socket[0] = "stream";
   STR_CY_Socket[1] = "dgram";
   STR_CY_Socket[2] = NULL;

   STR_CY_Wait[0] = "nowait";
   STR_CY_Wait[1] = "wait";
   STR_CY_Wait[2] = "dos";
   STR_CY_Wait[3] = NULL;

   if(obj = tmp.GR_Passwd = (Object *)DoSuperNew(cl, obj,
MUIA_InnerLeft, 0,
MUIA_InnerRight, 0,
MUIA_InnerBottom, 0,
MUIA_InnerTop, 0,
      Child, VGroup,
         MUIA_Group_Spacing, 0,
         Child, tmp.LV_Users = NListviewObject,
            MUIA_CycleChain            , 1,
            MUIA_NListview_NList         , tmp.LI_Users = NListObject,
               MUIA_Frame              , MUIV_Frame_InputList,
               MUIA_NList_DragType      , MUIV_NList_DragType_Default,
               MUIA_NList_DragSortable  , TRUE,
               MUIA_NList_DisplayHook   , &UsersList_DisplayHook,
               MUIA_NList_ConstructHook , &UsersList_ConstructHook,
               MUIA_NList_DestructHook  , &des_hook,
               MUIA_NList_Format        , "BAR,BAR,BAR,BAR,BAR,BAR,BAR,",
               MUIA_NList_Title         , TRUE,
            End,
         End,
         Child, HGroup,
            MUIA_Group_Spacing, 0,
            Child, tmp.BT_NewUser = MakeButton(MSG_BT_New),
            Child, tmp.BT_RemoveUser = MakeButton(MSG_BT_Remove),
         End,
      End,
      Child, ColGroup(2),
         Child, MakeKeyLabel2("  Login:", "  l"),
         Child, HGroup,
            Child, tmp.STR_Login = MakeKeyString(NULL, 81, "  l"),
            Child, MakeKeyLabel2(MSG_LA_DisableUser, MSG_CC_DisableUser),
            Child, tmp.CH_Disabled = MakeKeyCheckMark(FALSE, MSG_CC_DisableUser),
         End,
         Child, MakeKeyLabel2(MSG_LA_FullName, MSG_CC_FullName),
         Child, tmp.STR_Name = MakeKeyString("", 80, MSG_CC_FullName),
         Child, MakeKeyLabel2(MSG_LA_HomeDir, MSG_CC_HomeDir),
         Child, HGroup,
            Child, tmp.PA_HomeDir = MakePopAsl(tmp.STR_HomeDir = MakeKeyString("", MAXPATHLEN, MSG_CC_FullName), GetStr(MSG_TX_ChooseHomeDir), TRUE),
            Child, HGroup,
               Child, MakeKeyLabel2(MSG_LA_Shell, MSG_CC_Shell),
               Child, tmp.STR_Shell = MakeKeyString("", 80, MSG_CC_Shell),
            End,
         End,
         Child, MakeKeyLabel2(MSG_LA_UserID, MSG_CC_UserID),
         Child, HGroup,
            Child, tmp.STR_UserID = TextinputObject,
               StringFrame,
               MUIA_ControlChar     , *GetStr(MSG_CC_UserID),
               MUIA_CycleChain      , 1,
               MUIA_String_MaxLen   , 7,
               MUIA_String_Integer  , 0,
               MUIA_String_Accept   , "1234567890",
            End,
            Child, tmp.BT_ChangePassword = MakeButton(MSG_BT_ChangePassword),
         End,
         Child, MakeKeyLabel2(MSG_LA_UserGID, MSG_CC_UserGID),
         Child, HGroup,
            Child, tmp.STR_GroupID        = MakeKeyString(NULL, 7, MSG_CC_UserGID),
            Child, tmp.BT_RemovePassword  = MakeButton(MSG_BT_RemovePassword),
         End,
      End,
      TAG_MORE, msg->ops_AttrList))
   {
      struct Databases_Data *data = INST_DATA(cl,obj);

      *data = tmp;

      data->GR_Groups = VGroup,
MUIA_InnerLeft, 0,
MUIA_InnerRight, 0,
MUIA_InnerBottom, 0,
MUIA_InnerTop, 0,
         Child, VGroup,
            MUIA_Group_Spacing, 0,
            Child, data->LV_Groups = NListviewObject,
               MUIA_CycleChain            , 1,
               MUIA_NListview_NList         , data->LI_Groups = NListObject,
                  MUIA_Frame              , MUIV_Frame_InputList,
                  MUIA_NList_DragType     , MUIV_NList_DragType_Default,
                  MUIA_NList_DragSortable  , TRUE,
                  MUIA_NList_DisplayHook   , &GroupsList_DisplayHook,
                  MUIA_NList_ConstructHook , &GroupsList_ConstructHook,
                  MUIA_NList_DestructHook  , &des_hook,
//                  MUIA_NList_Format        , "BAR,BAR,BAR,",
MUIA_NList_Format        , "BAR,BAR,",
                  MUIA_NList_Title         , TRUE,
               End,
            End,
            Child, HGroup,
               MUIA_Group_Spacing, 0,
               Child, data->BT_NewGroup = MakeButton(MSG_BT_New),
               Child, data->BT_RemoveGroup = MakeButton(MSG_BT_Remove),
            End,
         End,
         Child, ColGroup(2),
            Child, MakeKeyLabel2(MSG_LA_GroupName, MSG_CC_GroupName),
            Child, data->STR_GroupName   = MakeKeyString("", 40, MSG_CC_GroupName),
            Child, MakeKeyLabel2(MSG_LA_GroupID, MSG_CC_GroupID),
            Child, data->STR_GroupNumber = MakeKeyString("0", 6, MSG_CC_GroupID),
            Child, MakeKeyLabel2(MSG_LA_GroupMembers, "  m"),
            Child, data->STR_GroupMembers = MakeKeyString(NULL, 1020, "  m"),
         End,
      End;

      data->GR_Hosts = VGroup,
MUIA_InnerLeft, 0,
MUIA_InnerRight, 0,
MUIA_InnerBottom, 0,
MUIA_InnerTop, 0,
         Child, VGroup,
            MUIA_Group_Spacing, 0,
            Child, data->LV_Hosts = NListviewObject,
               MUIA_CycleChain            , 1,
               MUIA_NListview_NList         , data->LI_Hosts = NListObject,
                  MUIA_Frame              , MUIV_Frame_InputList,
                  MUIA_NList_DragType      , MUIV_NList_DragType_Default,
                  MUIA_NList_DragSortable  , TRUE,
                  MUIA_NList_DisplayHook   , &HostList_DisplayHook,
                  MUIA_NList_ConstructHook , &HostList_ConstructHook,
                  MUIA_NList_DestructHook  , &des_hook,
                  MUIA_NList_Format        , "BAR,BAR,",
                  MUIA_NList_Title         , TRUE,
               End,
            End,
            Child, HGroup,
               MUIA_Group_Spacing, 0,
               Child, data->BT_NewHost = MakeButton(MSG_BT_New),
               Child, data->BT_RemoveHost = MakeButton(MSG_BT_Remove),
            End,
         End,
         Child, ColGroup(2),
            Child, MakeKeyLabel2(MSG_LA_IPAddr, MSG_CC_IPAddr),
            Child, data->STR_HostAddr = TextinputObject,
               MUIA_ControlChar     , *GetStr(MSG_CC_IPAddr),
               MUIA_CycleChain      , 1,
               StringFrame,
               MUIA_String_MaxLen   , 16,
               MUIA_String_Accept   , "1234567890.",
            End,
            Child, MakeKeyLabel2(MSG_LA_Name, MSG_CC_Name),
            Child, data->STR_HostName = MakeKeyString("", 40, MSG_CC_Name),
            Child, MakeKeyLabel2(MSG_LA_Aliases, MSG_CC_Aliases),
            Child, data->STR_HostAliases = MakeKeyString("", 80, MSG_CC_Aliases),
         End,
      End;

      data->GR_Protocols = VGroup,
MUIA_InnerLeft, 0,
MUIA_InnerRight, 0,
MUIA_InnerBottom, 0,
MUIA_InnerTop, 0,
         Child, VGroup,
            MUIA_Group_Spacing, 0,
            Child, data->LV_Protocols = NListviewObject,
               MUIA_CycleChain            , 1,
               MUIA_NListview_NList         , data->LI_Protocols = NListObject,
                  MUIA_Frame              , MUIV_Frame_InputList,
                  MUIA_NList_DragType      , MUIV_NList_DragType_Default,
                  MUIA_NList_DragSortable  , TRUE,
                  MUIA_NList_DisplayHook   , &ProtocolList_DisplayHook,
                  MUIA_NList_ConstructHook , &ProtocolList_ConstructHook,
                  MUIA_NList_DestructHook  , &des_hook,
                  MUIA_NList_Format        , "BAR,BAR,",
                  MUIA_NList_Title         , TRUE,
               End,
            End,
            Child, HGroup,
               MUIA_Group_Spacing, 0,
               Child, data->BT_NewProtocol = MakeButton(MSG_BT_New),
               Child, data->BT_RemoveProtocol = MakeButton(MSG_BT_Remove),
            End,
         End,
         Child, ColGroup(2),
            Child, MakeKeyLabel2(MSG_LA_Name, MSG_CC_Name),
            Child, data->STR_ProtocolName = MakeKeyString("", 40, MSG_CC_Name),
            Child, MakeKeyLabel2(MSG_LA_ID, MSG_CC_ID),
            Child, data->STR_ProtocolID = TextinputObject,
               MUIA_ControlChar     , *GetStr(MSG_CC_ID),
               MUIA_CycleChain      , 1,
               StringFrame,
               MUIA_String_MaxLen   , 7,
               MUIA_String_Integer  , 0,
               MUIA_String_Accept   , "1234567890",
            End,
            Child, MakeKeyLabel2(MSG_LA_Aliases, MSG_CC_Aliases),
            Child, data->STR_ProtocolAliases = MakeKeyString("", 80, MSG_CC_Aliases),
         End,
      End;

      data->GR_Services = VGroup,
MUIA_InnerLeft, 0,
MUIA_InnerRight, 0,
MUIA_InnerBottom, 0,
MUIA_InnerTop, 0,
         Child, VGroup,
            MUIA_Group_Spacing, 0,
            Child, data->LV_Services = NListviewObject,
               MUIA_CycleChain            , 1,
               MUIA_NListview_NList         , data->LI_Services = NListObject,
                  MUIA_Frame              , MUIV_Frame_InputList,
                  MUIA_NList_DragType      , MUIV_NList_DragType_Default,
                  MUIA_NList_DragSortable  , TRUE,
                  MUIA_NList_DisplayHook   , &ServiceList_DisplayHook,
                  MUIA_NList_ConstructHook , &ServiceList_ConstructHook,
                  MUIA_NList_DestructHook  , &des_hook,
                  MUIA_NList_Format        , "BAR,BAR,BAR,",
                  MUIA_NList_Title         , TRUE,
               End,
            End,
            Child, HGroup,
               MUIA_Group_Spacing, 0,
               Child, data->BT_NewService = MakeButton(MSG_BT_New),
               Child, data->BT_RemoveService = MakeButton(MSG_BT_Remove),
            End,
         End,
         Child, HGroup,
            Child, ColGroup(2),
               Child, MakeKeyLabel2(MSG_LA_Name, MSG_CC_Name),
               Child, data->STR_ServiceName = MakeKeyString("", 40, MSG_CC_Name),
               Child, MakeKeyLabel2(MSG_LA_Port, MSG_CC_Port),
               Child, data->STR_ServicePort = TextinputObject,
                  MUIA_ControlChar     , *GetStr(MSG_CC_Port),
                  MUIA_CycleChain      , 1,
                  StringFrame,
                  MUIA_String_MaxLen   , 7,
                  MUIA_String_Integer  , 0,
                  MUIA_String_Accept   , "1234567890",
               End,
               Child, MakeKeyLabel2(MSG_LA_Aliases, MSG_CC_Aliases),
               Child, data->STR_ServiceAliases = MakeKeyString("", 80, MSG_CC_Aliases),
            End,
            Child, BalanceObject, End,
            Child, VGroup,
               MUIA_Weight, 20,
               Child, data->LV_ServiceProtocol = NListviewObject,
                  MUIA_CycleChain   , 1,
                  MUIA_NListview_NList, data->LI_ServiceProtocol = NListObject,
                     MUIA_Frame, MUIV_Frame_InputList,
                  End,
               End,
            End,
         End,
      End;

      data->GR_InetAccess = VGroup,
MUIA_InnerLeft, 0,
MUIA_InnerRight, 0,
MUIA_InnerBottom, 0,
MUIA_InnerTop, 0,
         Child, VGroup,
            MUIA_Group_Spacing, 0,
            Child, data->LV_InetAccess = NListviewObject,
               MUIA_CycleChain            , 1,
               MUIA_NListview_NList         , data->LI_InetAccess = NListObject,
                  MUIA_Frame              , MUIV_Frame_InputList,
                  MUIA_NList_DragType     , MUIV_NList_DragType_Default,
                  MUIA_NList_DragSortable  , TRUE,
                  MUIA_NList_DisplayHook   , &InetAccessList_DisplayHook,
                  MUIA_NList_ConstructHook , &InetAccessList_ConstructHook,
                  MUIA_NList_DestructHook  , &des_hook,
                  MUIA_NList_Format        , "BAR,BAR,BAR,",
                  MUIA_NList_Title         , TRUE,
               End,
            End,
            Child, HGroup,
               MUIA_Group_Spacing, 0,
               Child, data->BT_NewInetAccess = MakeButton(MSG_BT_New),
               Child, data->BT_RemoveInetAccess = MakeButton(MSG_BT_Remove),
            End,
         End,
         Child, HGroup,
            Child, ColGroup(2),
               Child, MakeKeyLabel2(MSG_LA_Service, MSG_CC_Service),
               Child, data->STR_InetAccessService   = MakeKeyString("", 40, MSG_CC_Service),
               Child, MakeKeyLabel2("  Host:", "  h"),
               Child, data->STR_InetAccessHost      = MakeKeyString("", 40, "  h"),
               Child, MakeKeyLabel2("  Access:", "  a"),
               Child, HGroup,
                  Child, data->CY_InetAccess = Cycle(STR_CY_Access),
                  Child, MakeKeyLabel2("  Log:", "  l"),
                  Child, data->CH_InetAccessLog = MakeKeyCheckMark(TRUE, "  l"),
               End,
            End,
         End,
      End;

      data->GR_Inetd = VGroup,
MUIA_InnerLeft, 0,
MUIA_InnerRight, 0,
MUIA_InnerBottom, 0,
MUIA_InnerTop, 0,
         Child, VGroup,
            MUIA_Group_Spacing, 0,
            Child, data->LV_Inetd = NListviewObject,
               MUIA_CycleChain            , 1,
               MUIA_NListview_NList         , data->LI_Inetd = NListObject,
                  MUIA_Frame              , MUIV_Frame_InputList,
                  MUIA_NList_DragType     , MUIV_NList_DragType_Default,
                  MUIA_NList_DragSortable  , TRUE,
                  MUIA_NList_DisplayHook   , &InetdList_DisplayHook,
                  MUIA_NList_ConstructHook , &InetdList_ConstructHook,
                  MUIA_NList_DestructHook  , &des_hook,
                  MUIA_NList_Format        , "BAR,BAR,BAR,BAR,BAR,BAR,BAR,",
                  MUIA_NList_Title         , TRUE,
               End,
            End,
            Child, HGroup,
               MUIA_Group_Spacing, 0,
               Child, data->BT_NewInetd = MakeButton(MSG_BT_New),
               Child, data->BT_RemoveInetd = MakeButton(MSG_BT_Remove),
            End,
         End,
         Child, HGroup,
            Child, ColGroup(2),
               Child, MakeKeyLabel2(MSG_LA_Service, MSG_CC_Service),
               Child, data->STR_InetdService   = MakeKeyString("", 40, MSG_CC_Service),
               Child, MakeKeyLabel2(MSG_LA_User, MSG_CC_User),
               Child, data->STR_InetdUser      = MakeKeyString("", 40, MSG_CC_User),
               Child, MakeKeyLabel2(MSG_LA_Server, MSG_CC_Server),
               Child, data->PA_InetdServer     = MakePopAsl(data->STR_InetdServer = MakeKeyString("", 80, MSG_CC_Server), GetStr(MSG_LA_Server), FALSE),
               Child, MakeKeyLabel2(MSG_LA_Args, MSG_CC_Args),
               Child, data->STR_InetdArgs      = MakeKeyString("", 80, MSG_CC_Args),
               Child, MakeKeyLabel1(MSG_LA_Enabled, MSG_CC_Enabled),
               Child, HGroup,
                  Child, data->CH_InetdActive = MakeKeyCheckMark(TRUE, MSG_CC_Enabled),
                  Child, HVSpace,
               End,
            End,
            Child, BalanceObject, End,
            Child, VGroup,
               MUIA_Weight, 20,
               Child, data->LV_InetdProtocol = NListviewObject,
                  MUIA_CycleChain   , 1,
                  MUIA_NListview_NList, data->LI_InetdProtocol = NListObject,
                     MUIA_Frame, MUIV_Frame_InputList,
                  End,
               End,
               Child, data->CY_InetdSocket  = Cycle(STR_CY_Socket),
               Child, data->CY_InetdWait    = Cycle(STR_CY_Wait),
            End,
         End,
      End;

      data->GR_Networks = VGroup,
MUIA_InnerLeft, 0,
MUIA_InnerRight, 0,
MUIA_InnerBottom, 0,
MUIA_InnerTop, 0,
         Child, VGroup,
            MUIA_Group_Spacing, 0,
            Child, data->LV_Networks = NListviewObject,
               MUIA_CycleChain            , 1,
               MUIA_NListview_NList         , data->LI_Networks = NListObject,
                  MUIA_Frame              , MUIV_Frame_InputList,
                  MUIA_NList_DragType      , MUIV_NList_DragType_Default,
                  MUIA_NList_DragSortable  , TRUE,
                  MUIA_NList_DisplayHook   , &NetworkList_DisplayHook,
                  MUIA_NList_ConstructHook , &NetworkList_ConstructHook,
                  MUIA_NList_DestructHook  , &des_hook,
                  MUIA_NList_Format        , "BAR,BAR,",
                  MUIA_NList_Title         , TRUE,
               End,
            End,
            Child, HGroup,
               MUIA_Group_Spacing, 0,
               Child, data->BT_NewNetwork = MakeButton(MSG_BT_New),
               Child, data->BT_RemoveNetwork = MakeButton(MSG_BT_Remove),
            End,
         End,
         Child, ColGroup(2),
            Child, MakeKeyLabel2(MSG_LA_Name, MSG_CC_Name),
            Child, data->STR_NetworkName = MakeKeyString("", 40, MSG_CC_Name),
            Child, MakeKeyLabel2(MSG_LA_ID, MSG_CC_ID),
            Child, data->STR_NetworkID = TextinputObject,
               MUIA_ControlChar     , *GetStr(MSG_CC_ID),
               MUIA_CycleChain      , 1,
               StringFrame,
               MUIA_String_MaxLen   , 5,
               MUIA_String_Integer  , 0,
               MUIA_String_Accept   , "1234567890",
            End,
            Child, MakeKeyLabel2(MSG_LA_Aliases, MSG_CC_Aliases),
            Child, data->STR_NetworkAliases = MakeKeyString("", 80, MSG_CC_Aliases),
         End,
      End;

      data->GR_Rpc = VGroup,
MUIA_InnerLeft, 0,
MUIA_InnerRight, 0,
MUIA_InnerBottom, 0,
MUIA_InnerTop, 0,
         Child, VGroup,
            MUIA_Group_Spacing, 0,
            Child, data->LV_Rpcs = NListviewObject,
               MUIA_CycleChain            , 1,
               MUIA_NListview_NList         , data->LI_Rpcs = NListObject,
                  MUIA_Frame              , MUIV_Frame_InputList,
                  MUIA_NList_DragType      , MUIV_NList_DragType_Default,
                  MUIA_NList_DragSortable  , TRUE,
                  MUIA_NList_DisplayHook   , &RpcList_DisplayHook,
                  MUIA_NList_ConstructHook , &RpcList_ConstructHook,
                  MUIA_NList_DestructHook  , &des_hook,
                  MUIA_NList_Format        , "BAR,BAR,",
                  MUIA_NList_Title         , TRUE,
               End,
            End,
            Child, HGroup,
               MUIA_Group_Spacing, 0,
               Child, data->BT_NewRpc = MakeButton(MSG_BT_New),
               Child, data->BT_RemoveRpc = MakeButton(MSG_BT_Remove),
            End,
         End,
         Child, ColGroup(2),
            Child, MakeKeyLabel2(MSG_LA_Name, MSG_CC_Name),
            Child, data->STR_RpcName = MakeKeyString("", 40, MSG_CC_Name),
            Child, MakeKeyLabel2(MSG_LA_ID, MSG_CC_ID),
            Child, data->STR_RpcID = TextinputObject,
               MUIA_ControlChar     , *GetStr(MSG_CC_ID),
               MUIA_CycleChain      , 1,
               StringFrame,
               MUIA_String_MaxLen   , 10,
               MUIA_String_Integer  , 0,
               MUIA_String_Accept   , "1234567890",
            End,
            Child, MakeKeyLabel2(MSG_LA_Aliases, MSG_CC_Aliases),
            Child, data->STR_RpcAliases = MakeKeyString("", 80, MSG_CC_Aliases),
         End,
      End;

      if(data->GR_Hosts && data->GR_Protocols && data->GR_Services && data->GR_Inetd &&
         data->GR_Networks && data->GR_Rpc)
      {
         set(data->STR_Login       , MUIA_String_AttachedList, data->LV_Users);
         set(data->STR_GroupName   , MUIA_String_AttachedList, data->LV_Groups);
         set(data->STR_ProtocolName, MUIA_String_AttachedList, data->LV_Protocols);
         set(data->STR_HostAddr    , MUIA_String_AttachedList, data->LV_Hosts);
         set(data->STR_ServiceName , MUIA_String_AttachedList, data->LV_Services);
         set(data->STR_InetdService, MUIA_String_AttachedList, data->LV_Inetd);
         set(data->STR_NetworkName , MUIA_String_AttachedList, data->LV_Networks);
         set(data->STR_RpcName     , MUIA_String_AttachedList, data->LV_Rpcs);

         set(data->CY_InetAccess   , MUIA_CycleChain, 1);
         set(data->CH_InetAccessLog, MUIA_CycleChain, 1);

         set(data->CH_Disabled     , MUIA_CycleChain, 1);
         set(data->CY_InetdSocket  , MUIA_CycleChain, 1);
         set(data->CY_InetdWait    , MUIA_CycleChain, 1);
         set(data->CH_InetdActive  , MUIA_CycleChain, 1);

         set(data->BT_ChangePassword, MUIA_Weight, 10);
         set(data->BT_RemovePassword, MUIA_Weight, 10);

         set(data->LV_Hosts           , MUIA_ShortHelp, GetStr(MSG_Help_Hosts));
         set(data->BT_NewHost         , MUIA_ShortHelp, GetStr(MSG_Help_New));
         set(data->BT_RemoveHost      , MUIA_ShortHelp, GetStr(MSG_Help_Remove));
         set(data->STR_HostAddr       , MUIA_ShortHelp, GetStr(MSG_Help_Hosts_IPAddr));
         set(data->STR_HostName       , MUIA_ShortHelp, GetStr(MSG_Help_Name));
         set(data->STR_HostAliases    , MUIA_ShortHelp, GetStr(MSG_Help_Aliases));


         set(data->LV_Users           , MUIA_ShortHelp, GetStr(MSG_Help_Users_List));
         set(data->BT_NewUser         , MUIA_ShortHelp, GetStr(MSG_Help_New));
         set(data->BT_RemoveUser      , MUIA_ShortHelp, GetStr(MSG_Help_Remove));
         set(data->STR_Login          , MUIA_ShortHelp, GetStr(MSG_Help_Users_Username));
         set(data->STR_Name           , MUIA_ShortHelp, GetStr(MSG_Help_Users_Fullname));
         set(data->PA_HomeDir         , MUIA_ShortHelp, GetStr(MSG_Help_Users_HomeDir));
         set(data->STR_Shell          , MUIA_ShortHelp, GetStr(MSG_Help_Users_Shell));
         set(data->CH_Disabled        , MUIA_ShortHelp, GetStr(MSG_Help_Users_Disable));
         set(data->STR_UserID         , MUIA_ShortHelp, GetStr(MSG_Help_Users_UserID));
         set(data->BT_ChangePassword  , MUIA_ShortHelp, GetStr(MSG_Help_Users_ChangePassword));
         set(data->BT_RemovePassword  , MUIA_ShortHelp, GetStr(MSG_Help_Users_RemovePassword));
         set(data->STR_GroupID        , MUIA_ShortHelp, GetStr(MSG_Help_Users_GroupID));
         set(data->LV_Groups          , MUIA_ShortHelp, GetStr(MSG_Help_Groups_List));
         set(data->BT_NewGroup        , MUIA_ShortHelp, GetStr(MSG_Help_New));
         set(data->BT_RemoveGroup     , MUIA_ShortHelp, GetStr(MSG_Help_Remove));
         set(data->STR_GroupName      , MUIA_ShortHelp, GetStr(MSG_Help_Groups_Name));
         set(data->STR_GroupNumber    , MUIA_ShortHelp, GetStr(MSG_Help_Groups_Number));
         set(data->STR_GroupMembers   , MUIA_ShortHelp, GetStr(MSG_Help_Groups_Members));

         set(data->LV_Protocols       , MUIA_ShortHelp, GetStr(MSG_Help_Protocols));
         set(data->BT_NewProtocol     , MUIA_ShortHelp, GetStr(MSG_Help_New));
         set(data->BT_RemoveProtocol  , MUIA_ShortHelp, GetStr(MSG_Help_Remove));
         set(data->STR_ProtocolName   , MUIA_ShortHelp, GetStr(MSG_Help_Name));
         set(data->STR_ProtocolID     , MUIA_ShortHelp, GetStr(MSG_Help_ID));
         set(data->STR_ProtocolAliases, MUIA_ShortHelp, GetStr(MSG_Help_Aliases));

         set(data->LV_Services        , MUIA_ShortHelp, GetStr(MSG_Help_Services));
         set(data->BT_NewService      , MUIA_ShortHelp, GetStr(MSG_Help_New));
         set(data->BT_RemoveService   , MUIA_ShortHelp, GetStr(MSG_Help_Remove));
         set(data->STR_ServiceName    , MUIA_ShortHelp, GetStr(MSG_Help_Name));
         set(data->STR_ServicePort    , MUIA_ShortHelp, GetStr(MSG_Help_Port));
         set(data->STR_ServiceAliases , MUIA_ShortHelp, GetStr(MSG_Help_Aliases));
         set(data->LV_ServiceProtocol , MUIA_ShortHelp, GetStr(MSG_Help_Protocol));

         set(data->LV_Inetd           , MUIA_ShortHelp, GetStr(MSG_Help_Inetd));
         set(data->BT_NewInetd        , MUIA_ShortHelp, GetStr(MSG_Help_New));
         set(data->BT_RemoveInetd     , MUIA_ShortHelp, GetStr(MSG_Help_Remove));
         set(data->STR_InetdService   , MUIA_ShortHelp, GetStr(MSG_Help_Service));
         set(data->STR_InetdUser      , MUIA_ShortHelp, GetStr(MSG_Help_Inetd_User));
         set(data->PA_InetdServer     , MUIA_ShortHelp, GetStr(MSG_Help_Inetd_Server));
         set(data->STR_InetdArgs      , MUIA_ShortHelp, GetStr(MSG_Help_Args));
         set(data->CH_InetdActive     , MUIA_ShortHelp, GetStr(MSG_Help_Enabled));
         set(data->LV_InetdProtocol   , MUIA_ShortHelp, GetStr(MSG_Help_Protocol));
         set(data->CY_InetdSocket     , MUIA_ShortHelp, GetStr(MSG_Help_Socket));
         set(data->CY_InetdWait       , MUIA_ShortHelp, GetStr(MSG_Help_Wait));

         set(data->LV_Networks        , MUIA_ShortHelp, GetStr(MSG_Help_Networks));
         set(data->BT_NewNetwork      , MUIA_ShortHelp, GetStr(MSG_Help_New));
         set(data->BT_RemoveNetwork   , MUIA_ShortHelp, GetStr(MSG_Help_Remove));
         set(data->STR_NetworkName    , MUIA_ShortHelp, GetStr(MSG_Help_Name));
         set(data->STR_NetworkID      , MUIA_ShortHelp, GetStr(MSG_Help_ID));
         set(data->STR_NetworkAliases , MUIA_ShortHelp, GetStr(MSG_Help_Aliases));

         set(data->LV_Rpcs            , MUIA_ShortHelp, GetStr(MSG_Help_Rpcs));
         set(data->BT_NewRpc          , MUIA_ShortHelp, GetStr(MSG_Help_New));
         set(data->BT_RemoveRpc       , MUIA_ShortHelp, GetStr(MSG_Help_Remove));
         set(data->STR_RpcName        , MUIA_ShortHelp, GetStr(MSG_Help_Name));
         set(data->STR_RpcID          , MUIA_ShortHelp, GetStr(MSG_Help_ID));
         set(data->STR_RpcAliases     , MUIA_ShortHelp, GetStr(MSG_Help_Aliases));

         DoMethod(data->LV_Users      , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_SetStates, MUIV_Databases_SetStates_Users);
         DoMethod(data->LV_Groups     , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_SetStates, MUIV_Databases_SetStates_Groups);
         DoMethod(data->LV_Protocols  , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_SetStates, MUIV_Databases_SetStates_Protocols);
         DoMethod(data->LV_Services   , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_SetStates, MUIV_Databases_SetStates_Services);
         DoMethod(data->LV_InetAccess , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_SetStates, MUIV_Databases_SetStates_InetAccess);
         DoMethod(data->LV_Inetd      , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_SetStates, MUIV_Databases_SetStates_Inetd);
         DoMethod(data->LV_Hosts      , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_SetStates, MUIV_Databases_SetStates_Hosts);
         DoMethod(data->LV_Networks   , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_SetStates, MUIV_Databases_SetStates_Networks);
         DoMethod(data->LV_Rpcs       , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_SetStates, MUIV_Databases_SetStates_Rpcs);

         DoMethod(data->STR_Login           , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_Login);
         DoMethod(data->STR_Name            , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_FullName);
         DoMethod(data->PA_HomeDir          , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_HomeDir);
         DoMethod(data->STR_Shell           , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_Shell);
         DoMethod(data->STR_UserID          , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_UserID);
         DoMethod(data->STR_GroupID         , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_GroupID);
         DoMethod(data->CH_Disabled         , MUIM_Notify, MUIA_Selected        , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_Disable);
         DoMethod(data->BT_ChangePassword   , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_ChangePassword);
         DoMethod(data->BT_RemovePassword   , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_RemovePassword);
         DoMethod(data->STR_GroupName       , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_GroupName);
         DoMethod(data->STR_GroupNumber     , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_GroupNumber);
         DoMethod(data->STR_GroupMembers    , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_GroupMembers);
         DoMethod(data->STR_ProtocolName    , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_ProtocolName);
         DoMethod(data->STR_ProtocolID      , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_ProtocolID);
         DoMethod(data->STR_ProtocolAliases , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_ProtocolAliases);
         DoMethod(data->STR_ServiceName     , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_ServiceName);
         DoMethod(data->STR_ServicePort     , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_ServicePort);
         DoMethod(data->STR_ServiceAliases  , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_ServiceAliases);
         DoMethod(data->LV_ServiceProtocol  , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_ServiceProtocol);
         DoMethod(data->STR_InetAccessService, MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetAccessService);
         DoMethod(data->STR_InetAccessHost  , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetAccessHost);
         DoMethod(data->CY_InetAccess       , MUIM_Notify, MUIA_Cycle_Active    , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetAccess);
         DoMethod(data->CH_InetAccessLog    , MUIM_Notify, MUIA_Selected        , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetAccessLog);
         DoMethod(data->STR_InetdService    , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetdService);
         DoMethod(data->STR_InetdUser       , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetdUser);
         DoMethod(data->PA_InetdServer      , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetdServer);
         DoMethod(data->STR_InetdArgs       , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetdArgs);
         DoMethod(data->CH_InetdActive      , MUIM_Notify, MUIA_Selected        , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetdActive);
         DoMethod(data->LV_InetdProtocol    , MUIM_Notify, MUIA_NList_Active     , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetdProtocol);
         DoMethod(data->CY_InetdSocket      , MUIM_Notify, MUIA_Cycle_Active    , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetdSocket);
         DoMethod(data->CY_InetdWait        , MUIM_Notify, MUIA_Cycle_Active    , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_InetdWait);
         DoMethod(data->STR_HostAddr        , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_HostAddr);
         DoMethod(data->STR_HostName        , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_HostName);
         DoMethod(data->STR_HostAliases     , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_HostAliases);
         DoMethod(data->STR_NetworkName     , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NetworkName);
         DoMethod(data->STR_NetworkID       , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NetworkID);
         DoMethod(data->STR_NetworkAliases  , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NetworkAliases);
         DoMethod(data->STR_RpcName         , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_RpcName);
         DoMethod(data->STR_RpcID           , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_RpcID);
         DoMethod(data->STR_RpcAliases      , MUIM_Notify, MUIA_String_Contents , MUIV_EveryTime  , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_RpcAliases);
         DoMethod(data->BT_NewUser          , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NewUser);
         DoMethod(data->BT_NewGroup         , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NewGroup);
         DoMethod(data->BT_NewProtocol      , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NewProtocol);
         DoMethod(data->BT_NewService       , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NewService);
         DoMethod(data->BT_NewInetAccess    , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NewInetAccess);
         DoMethod(data->BT_NewInetd         , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NewInetd);
         DoMethod(data->BT_NewHost          , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NewHost);
         DoMethod(data->BT_NewNetwork       , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NewNetwork);
         DoMethod(data->BT_NewRpc           , MUIM_Notify, MUIA_Pressed         , FALSE           , obj, 2, MUIM_Databases_Modification, MUIV_Databases_Modification_NewRpc);
         DoMethod(data->BT_RemoveUser       , MUIM_Notify, MUIA_Pressed         , FALSE           , data->LV_Users, 2, MUIM_NList_Remove, MUIV_NList_Remove_Active);
         DoMethod(data->BT_RemoveGroup      , MUIM_Notify, MUIA_Pressed         , FALSE           , data->LV_Groups, 2, MUIM_NList_Remove, MUIV_NList_Remove_Active);
         DoMethod(data->BT_RemoveProtocol   , MUIM_Notify, MUIA_Pressed         , FALSE           , data->LV_Protocols, 2, MUIM_NList_Remove, MUIV_NList_Remove_Active);
         DoMethod(data->BT_RemoveService    , MUIM_Notify, MUIA_Pressed         , FALSE           , data->LV_Services, 2, MUIM_NList_Remove, MUIV_NList_Remove_Active);
         DoMethod(data->BT_RemoveInetAccess , MUIM_Notify, MUIA_Pressed         , FALSE           , data->LV_InetAccess, 2, MUIM_NList_Remove, MUIV_NList_Remove_Active);
         DoMethod(data->BT_RemoveInetd      , MUIM_Notify, MUIA_Pressed         , FALSE           , data->LV_Inetd, 2, MUIM_NList_Remove, MUIV_NList_Remove_Active);
         DoMethod(data->BT_RemoveHost       , MUIM_Notify, MUIA_Pressed         , FALSE           , data->LV_Hosts, 2, MUIM_NList_Remove, MUIV_NList_Remove_Active);
         DoMethod(data->BT_RemoveNetwork    , MUIM_Notify, MUIA_Pressed         , FALSE           , data->LV_Networks, 2, MUIM_NList_Remove, MUIV_NList_Remove_Active);
         DoMethod(data->BT_RemoveRpc        , MUIM_Notify, MUIA_Pressed         , FALSE           , data->LV_Rpcs, 2, MUIM_NList_Remove, MUIV_NList_Remove_Active);

         DoMethod(data->STR_Login       , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_Name);
         DoMethod(data->STR_Name        , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_HomeDir);
         DoMethod(data->STR_HomeDir     , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_Shell);
         DoMethod(data->STR_Shell       , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_UserID);
         DoMethod(data->STR_UserID      , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_GroupID);
         DoMethod(data->STR_GroupName   , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_GroupNumber);
         DoMethod(data->STR_GroupNumber , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_GroupMembers);
         DoMethod(data->STR_HostAddr    , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_HostName);
         DoMethod(data->STR_HostName    , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_HostAliases);
         DoMethod(data->STR_ProtocolName, MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_ProtocolID);
         DoMethod(data->STR_ProtocolID  , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_ProtocolAliases);
         DoMethod(data->STR_ServiceName , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_ServicePort);
         DoMethod(data->STR_ServicePort , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_ServiceAliases);
         DoMethod(data->STR_InetAccessService, MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_InetAccessHost);
         DoMethod(data->STR_InetdService, MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_InetdUser);
         DoMethod(data->STR_InetdUser   , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_InetdServer);
         DoMethod(data->STR_InetdServer , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_InetdArgs);
         DoMethod(data->STR_NetworkName , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_NetworkID);
         DoMethod(data->STR_NetworkID   , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_NetworkAliases);
         DoMethod(data->STR_RpcName     , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_RpcID);
         DoMethod(data->STR_RpcID       , MUIM_Notify, MUIA_String_Acknowledge , MUIV_EveryTime, win, 3, MUIM_Set, MUIA_Window_ActiveObject, data->STR_RpcAliases);
      }
      else
         obj = NULL;
   }
   return((ULONG)obj);
}

///
/// Databases_Dispatcher
SAVEDS ULONG Databases_Dispatcher(register __a0 struct IClass *cl, register __a2 Object *obj, register __a1 Msg msg)
{
   if(msg->MethodID == OM_NEW)
      return(Databases_New              (cl, obj, (APTR)msg));
   if(msg->MethodID == MUIM_Databases_SetStates)
      return(Databases_SetStates        (cl, obj, (APTR)msg));
   if(msg->MethodID == MUIM_Databases_Modification)
      return(Databases_Modification     (cl, obj, (APTR)msg));
   if(msg->MethodID == MUIM_Genesis_Finish)
      return(Databases_PasswdReqFinish  (cl, obj, (APTR)msg));

   return(DoSuperMethodA(cl, obj, msg));
}

///

