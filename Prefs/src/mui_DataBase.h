#define MUIM_Databases_SetStates                (TAGBASE_PREFS | 0x1050)
#define MUIM_Databases_Modification             (TAGBASE_PREFS | 0x1051)

#define MUIV_Databases_SetStates_Groups            0
#define MUIV_Databases_SetStates_Protocols         1
#define MUIV_Databases_SetStates_Services          2
#define MUIV_Databases_SetStates_InetAccess        3
#define MUIV_Databases_SetStates_Inetd             4
#define MUIV_Databases_SetStates_Hosts             5
#define MUIV_Databases_SetStates_Networks          6
#define MUIV_Databases_SetStates_Rpcs              7

#define MUIV_Databases_Modification_NewGroup          1
#define MUIV_Databases_Modification_GroupName         2
#define MUIV_Databases_Modification_GroupNumber       3
#define MUIV_Databases_Modification_GroupMembers      4
#define MUIV_Databases_Modification_ProtocolName      5
#define MUIV_Databases_Modification_ProtocolID        6
#define MUIV_Databases_Modification_ProtocolAliases   7
#define MUIV_Databases_Modification_ServiceName       8
#define MUIV_Databases_Modification_ServicePort       9
#define MUIV_Databases_Modification_ServiceAliases    10
#define MUIV_Databases_Modification_ServiceProtocol   11
#define MUIV_Databases_Modification_InetAccessService 12
#define MUIV_Databases_Modification_InetAccessHost    13
#define MUIV_Databases_Modification_InetAccess        14
#define MUIV_Databases_Modification_InetAccessLog     15
#define MUIV_Databases_Modification_InetdService      16
#define MUIV_Databases_Modification_InetdUser         17
#define MUIV_Databases_Modification_InetdServer       18
#define MUIV_Databases_Modification_InetdArgs         19
#define MUIV_Databases_Modification_InetdActive       20
#define MUIV_Databases_Modification_InetdProtocol     21
#define MUIV_Databases_Modification_InetdSocket       22
#define MUIV_Databases_Modification_InetdWait         23
#define MUIV_Databases_Modification_HostAddr          24
#define MUIV_Databases_Modification_HostName          25
#define MUIV_Databases_Modification_HostAliases       26
#define MUIV_Databases_Modification_NetworkName       27
#define MUIV_Databases_Modification_NetworkID         28
#define MUIV_Databases_Modification_NetworkAliases    29
#define MUIV_Databases_Modification_RpcName           30
#define MUIV_Databases_Modification_RpcID             31
#define MUIV_Databases_Modification_RpcAliases        32
#define MUIV_Databases_Modification_NewProtocol       33
#define MUIV_Databases_Modification_NewService        34
#define MUIV_Databases_Modification_NewInetAccess     35
#define MUIV_Databases_Modification_NewInetd          36
#define MUIV_Databases_Modification_NewHost           37
#define MUIV_Databases_Modification_NewNetwork        38
#define MUIV_Databases_Modification_NewRpc            39

struct MUIP_Databases_SetStates        { ULONG MethodID; LONG page; };
struct MUIP_Databases_Modification     { ULONG MethodID; LONG what; };

struct Databases_Data
{
   Object *CY_Pager;
   Object *GR_Pager;

   Object *GR_Groups;
   Object *LV_Groups;
   Object *LI_Groups;
   Object *BT_NewGroup;
   Object *BT_RemoveGroup;
   Object *STR_GroupName;
   Object *STR_GroupNumber;
   Object *STR_GroupMembers;

   Object *GR_Protocols;
   Object *LV_Protocols;
   Object *LI_Protocols;
   Object *BT_NewProtocol;
   Object *BT_RemoveProtocol;
   Object *STR_ProtocolName;
   Object *STR_ProtocolID;
   Object *STR_ProtocolAliases;

   Object *GR_Services;
   Object *LV_Services;
   Object *LI_Services;
   Object *BT_NewService;
   Object *BT_RemoveService;
   Object *STR_ServiceName;
   Object *STR_ServicePort;
   Object *STR_ServiceAliases;
   Object *LV_ServiceProtocol;
   Object *LI_ServiceProtocol;

   Object *GR_InetAccess;
   Object *LV_InetAccess;
   Object *LI_InetAccess;
   Object *BT_NewInetAccess;
   Object *BT_RemoveInetAccess;
   Object *STR_InetAccessService;
   Object *STR_InetAccessHost;
   Object *CY_InetAccess;
   Object *CH_InetAccessLog;

   Object *GR_Inetd;
   Object *LV_Inetd;
   Object *LI_Inetd;
   Object *BT_NewInetd;
   Object *BT_RemoveInetd;
   Object *STR_InetdService;
   Object *STR_InetdUser;
   Object *PA_InetdServer;
   Object *STR_InetdServer;
   Object *STR_InetdArgs;
   Object *LV_InetdProtocol;
   Object *LI_InetdProtocol;
   Object *CY_InetdSocket;
   Object *CY_InetdWait;
   Object *CH_InetdActive;

   Object *GR_Hosts;
   Object *LV_Hosts;
   Object *LI_Hosts;
   Object *BT_NewHost;
   Object *BT_RemoveHost;
   Object *STR_HostAddr;
   Object *STR_HostName;
   Object *STR_HostAliases;

   Object *GR_Networks;
   Object *LV_Networks;
   Object *LI_Networks;
   Object *BT_NewNetwork;
   Object *BT_RemoveNetwork;
   Object *STR_NetworkName;
   Object *STR_NetworkID;
   Object *STR_NetworkAliases;

   Object *GR_Rpc;
   Object *LV_Rpcs;
   Object *LI_Rpcs;
   Object *BT_NewRpc;
   Object *BT_RemoveRpc;
   Object *STR_RpcName;
   Object *STR_RpcID;
   Object *STR_RpcAliases;
};

