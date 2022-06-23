/// includes
#include "/includes.h"
#include <workbench/WBStart.h>

#include "Strings.h"
#include "/Genesis.h"
#include "/genesis.lib/libraries/genesis.h"
#include "/genesis.lib/pragmas/genesis_lib.h"
#include "/genesis.lib/proto/genesis.h"
#include "/genesis.lib/pragmas/nc_lib.h"
#include "mui.h"
#include "mui_MainWindow.h"
#include "sana.h"
#include "protos.h"
#include <proto/icon.h>

///
/// defines
enum { ARG_PROVIDER, ARG_USER, ARG_PASSWORD, ARG_WAITSTACK, ARGS };
#define ARGTEMPLATE "PROVIDER/K,USER/K,PW=PASSWORD/K,WAITSTACK/N"

///
/// external variables
extern struct Library *LocaleBase, *LockSocketBase, *GenesisBase;
extern struct ExecBase *SysBase;
extern struct Process *proc;
extern struct Catalog *cat;
extern struct MUI_CustomClass *CL_MainWindow;
extern Object *win, *app, *status_win;
extern const char AmiTCP_PortName[];
extern struct MsgPort *MainPort;
extern struct Config Config;
extern struct User *current_user;
extern int waitstack;
extern char default_provider[];
extern struct CommandLineInterface *LocalCLI;
extern struct Library *NetConnectBase;

///

/// xget
LONG xget(Object *obj,ULONG attribute)
{
   LONG x;
   get(obj,attribute,&x);
   return(x);
}

///
/// DoSuperNew
ULONG DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...)
{
   return(DoSuperMethod(cl,obj,OM_NEW,&tag1,NULL));
}

///
/// MakeButton
Object *MakeButton(STRPTR string)
{
   Object *obj = MUI_MakeObject(MUIO_Button, GetStr(string));
   if(obj)
      set(obj,MUIA_CycleChain,1);
   return(obj);
}

///
/// MakeText
Object *MakeText(STRPTR string, BOOL set_min)
{
   Object *obj = TextObject,
      TextFrame,
      MUIA_Background, MUII_TextBack,
      MUIA_Text_Contents,  string,
      MUIA_Text_SetMin  ,  set_min,
   End;

   return(obj);
}

///
/// GetStr
STRPTR GetStr(STRPTR idstr)
{
   STRPTR local;

   local = idstr + 2;

   if(cat)
      return((STRPTR)GetCatalogStr(cat, *(UWORD *)idstr, local));

   return(local);
}

///
/// EscapeString
VOID EscapeString(STRPTR buffer, STRPTR str)
{
/* with backlash '\' :
   B   - backspace
   T   - horizontal tab
   N   - newline
   V   - vertical tab
   F   - formfeed
   R   - return
   E   - escape (ASCII 27 decimal)
   Xnn - character represented by hex value nn.
*/
   ULONG len;
   UWORD offs;
   STRPTR help;

   offs  = 0;
   len   = 80;
   help  = str;

   while(*help && len)
   {
      if(*help != '\\')
      {
         buffer[offs++] = *help++;
      }
      else
      {
         help++;     /* auf cmd-char stellen.. */
         if(*help == 'b' || *help == 'B')
            buffer[offs++] = '\b';
         else if(*help == 't' || *help == 'T')
            buffer[offs++] = '\t';
         else if(*help == 'n' || *help == 'N')
            buffer[offs++] = '\n';
         else if(*help == 'v' || *help == 'V')
            buffer[offs++] = '\v';
         else if(*help == 'f' || *help == 'F')
            buffer[offs++] = '\f';
         else if(*help == 'r' || *help == 'R')
            buffer[offs++] = '\r';
         else if(*help == 'e' || *help == 'E')
            buffer[offs++] = 27;
         else if(*help == 'x' || *help == 'X')
         {
            UWORD i;
            UBYTE cnt = 0;
            UBYTE c;

            for(i=0; i<2; i++)
            {
               c = *help;
               cnt <<= 4;     /* mit 16 multiplizieren */

               if(c >= '0' && c <= '9')
                  cnt += c - '0';
               else
               {
                  if(c >= 'A' && c <= 'F')
                     cnt += 9 + (c - ('A'-1));
                  else
                  {
                     if(c >= 'a' && c <= 'f')
                        cnt += 9 + (c - ('a'-1));
                  }
               }
               help++;
            }
            buffer[offs++] = cnt;
         }
         else if(*help == '\\')
            buffer[offs++] = '\\';

         help++;
      }
      len--;
   }
   buffer[offs] = '\0';
}

///
/// extract_arg
STRPTR extract_arg(STRPTR string, STRPTR buffer, LONG len, char sep)
{
   STRPTR ptr1, ptr2;

   strncpy(buffer, string, len);

   ptr1 = strchr(buffer, (sep ? sep : ' '));
   ptr2 = strchr(buffer, 9);

   if(ptr2 && ((ptr2 < ptr1) || !ptr1))
      ptr1 = ptr2;
   if(ptr1)
      *ptr1 = NULL;

   string += strlen(buffer);

   while(*string == ' ' || *string == 9 || (sep ? *string == sep : NULL))
      string++;

   return((*string ? string : NULL));
}

///

/// get_programicon
struct DiskObject *get_programicon(VOID)
{
   struct DiskObject *Icon = NULL;

   if(WBenchMsg)
   {
      if(WBenchMsg->sm_ArgList)
      {
         if(WBenchMsg->sm_ArgList->wa_Name)
         {
            if(Icon = GetDiskObjectNew(WBenchMsg->sm_ArgList->wa_Name))
            {
               if(Icon->do_Type != WBTOOL)
               {
                  FreeDiskObject(Icon);
                  Icon = NULL;
               }
            }

            if(!Icon)
            {
               BPTR NewLock;

               if(NewLock = Lock("PROGDIR:",ACCESS_READ))
               {
                  BPTR OldLock;

                  OldLock = CurrentDir(NewLock);

                  if(Icon = GetDiskObjectNew(WBenchMsg->sm_ArgList->wa_Name))
                  {
                     if(Icon->do_Type != WBTOOL)
                     {
                        FreeDiskObject(Icon);
                        Icon = NULL;
                     }
                  }

                  if(!Icon)
                  {
                     if(Icon = GetDiskObjectNew("GENESiS"))
                     {
                        if(Icon->do_Type != WBTOOL)
                        {
                           FreeDiskObject(Icon);
                           Icon = NULL;
                        }
                     }
                  }
                  CurrentDir(OldLock);
                  UnLock(NewLock);
               }
            }
         }
      }
   }

   if(!Icon)
   {
      if(Icon = GetDiskObjectNew("GENESiS"))
      {
         if(Icon->do_Type != WBTOOL)
         {
            FreeDiskObject(Icon);
            Icon = NULL;
         }
      }

      if(!Icon)
      {
         if(Icon = GetDiskObjectNew("PROGDIR:GENESiS"))
         {
            if(Icon->do_Type != WBTOOL)
            {
               FreeDiskObject(Icon);
               Icon = NULL;
            }
         }
      }
   }

/* last chance ... */
   if(!Icon)
      Icon = GetDefDiskObject(WBTOOL);

   return(Icon);
}

///
/// parse_arguments
BOOL parse_arguments(VOID)
{
   BOOL success = FALSE;

   if(!LocalCLI) // see main() why not proc->pr_CLI
   {
      STRPTR *ArgArray;
      struct RDArgs *ArgsPtr;

      if(ArgArray = (STRPTR *)AllocVec(sizeof(STRPTR) * ARGS, MEMF_ANY | MEMF_CLEAR))
      {
         if(ArgsPtr = (struct RDArgs *)AllocVec(sizeof(struct RDArgs), MEMF_ANY | MEMF_CLEAR))
         {
            if(ReadArgs(ARGTEMPLATE, (LONG *)ArgArray, ArgsPtr))
            {
               success = TRUE;

               if(ArgArray[ARG_PROVIDER])
                  strncpy(default_provider, ArgArray[ARG_PROVIDER], 80);
               if(ArgArray[ARG_USER])
               {
                  if(current_user)
                     FreeUser(current_user);
                  if(current_user = GetUser(ArgArray[ARG_USER], ArgArray[ARG_PASSWORD], NULL, NULL))
                     SetGlobalUser(current_user);
                  else
                     success = FALSE;
               }
                  ;
               if(ArgArray[ARG_WAITSTACK])
                  waitstack = *((LONG *)ArgArray[ARG_WAITSTACK]);

               FreeArgs(ArgsPtr);
            }
            FreeVec(ArgsPtr);
         }
         FreeVec(ArgArray);
      }
   }
   else
   {
      if(IconBase = OpenLibrary("icon.library",0))
      {
         struct DiskObject *Icon;

         if(Icon = (struct DiskObject *)get_programicon())
         {
            STRPTR Type;

            success = TRUE;

            if(Type = FindToolType(Icon->do_ToolTypes, "PROVIDER"))
               strncpy(default_provider, Type, 80);
            if(Type = FindToolType(Icon->do_ToolTypes, "USER"))
            {
               STRPTR password;

               password = FindToolType(Icon->do_ToolTypes, "PASSWORD");
               if(current_user)
                  FreeUser(current_user);
               if(current_user = GetUser(Type, password, NULL, NULL))
                  SetGlobalUser(current_user);
               else
                  success = FALSE;
            }
            if(Type = FindToolType(Icon->do_ToolTypes, "WAITSTACK"))
               waitstack = atol(Type);

            FreeDiskObject(Icon);
         }

         CloseLibrary(IconBase);
         IconBase = NULL;
      }
   }
   return(success);
}

///
/// load_reconnect
BOOL load_reconnect(VOID)
{
   struct ParseConfig_Data pc_data;
   struct MainWindow_Data *data = INST_DATA(CL_MainWindow->mcc_Class, win);
   struct Interface *iface = NULL;
   BOOL success = FALSE;

   if(ParseConfig("AmiTCP:db/reconnect.conf", &pc_data))
   {
      while(ParseNext(&pc_data))
      {
         if(!stricmp(pc_data.pc_argument, "ISP"))
         {
Printf("setting tx_provider\n");
            DoMethod(win, MUIM_MainWindow_ChangeProvider, pc_data.pc_contents, FALSE);
Printf("returned from set(tx_provider\n");
            strncpy(default_provider, pc_data.pc_contents, 80);
            success = TRUE;
            break;
         }
      }
      if(success)
      {
Printf("continuing parse reconnect\n");
         while(ParseNext(&pc_data))
         {
            if(!stricmp(pc_data.pc_argument, "DomainName"))
            {
               if(!find_server_by_name(&data->isp.isp_domainnames, pc_data.pc_contents))
                  add_server(&data->isp.isp_domainnames, pc_data.pc_contents);
            }
            if(!stricmp(pc_data.pc_argument, "NameServer"))
            {
               if(!find_server_by_name(&data->isp.isp_nameservers, pc_data.pc_contents))
                  add_server(&data->isp.isp_nameservers, pc_data.pc_contents);
            }
            if(!stricmp(pc_data.pc_argument, "Interface"))
            {
               if(iface = find_by_name(&data->isp.isp_ifaces, pc_data.pc_contents))
               {
                  if(iface->if_flags & IFL_IsOnline)
                     iface = NULL;
                  else
                     iface->if_flags |= IFL_PutOnline;
Printf("iface: '%ls'\n", iface->if_name);
               }
Printf("contents: '%ls'\n", pc_data.pc_contents);
            }
            if(!stricmp(pc_data.pc_argument, "IPAddr") && iface)
               strncpy(iface->if_addr, pc_data.pc_contents, sizeof(iface->if_addr));
            if(!stricmp(pc_data.pc_argument, "DestIP") && iface)
               strncpy(iface->if_dst, pc_data.pc_contents, sizeof(iface->if_dst));
            if(!stricmp(pc_data.pc_argument, "Gateway") && iface)
               strncpy(iface->if_gateway, pc_data.pc_contents, sizeof(iface->if_gateway));
         }
      }
      ParseEnd(&pc_data);
   }
   return(success);
}

///
/// clear_config
VOID clear_config(struct Config *conf)
{
   struct MainWindow_Data *mw_data = INST_DATA(CL_MainWindow->mcc_Class, win);
   APTR display_item;

   if(conf->cnf_startup)
      FreeVec(conf->cnf_startup);
   if(conf->cnf_shutdown)
      FreeVec(conf->cnf_shutdown);

   bzero(conf, sizeof(struct Config));

   strcpy(conf->cnf_serialdevice, "serial.device");
   strcpy(conf->cnf_initstring, "AT&F&D2");
   strcpy(conf->cnf_dialprefix, "ATDT");
   conf->cnf_baudrate        = 38400;
   conf->cnf_redialattempts  = 10;
   conf->cnf_redialdelay     = 5;
   conf->cnf_flags = CFL_7Wire | CFL_RadBoogie | CFL_ShowLog | CFL_ShowLeds | CFL_ShowConnect |
      CFL_ShowOnlineTime | CFL_ShowButtons | CFL_ShowProvider | CFL_ShowUser | CFL_ShowStatusWin |
      CFL_ShowSerialInput | CFL_StartupOpenWin | CFL_StartupInetd | CFL_StartupLoopback |
      CFL_StartupTCP;

   display_item = (APTR)DoMethod(mw_data->MN_Strip, MUIM_FindUData, MEN_DISPLAY);
   DoMethod(display_item, MUIM_SetUData, MEN_TIMEONLINE  , MUIA_Menuitem_Checked, TRUE);
   DoMethod(display_item, MUIM_SetUData, MEN_LEDS        , MUIA_Menuitem_Checked, TRUE);
   DoMethod(display_item, MUIM_SetUData, MEN_CONNECT     , MUIA_Menuitem_Checked, TRUE);
   DoMethod(display_item, MUIM_SetUData, MEN_PROVIDER    , MUIA_Menuitem_Checked, TRUE);
   DoMethod(display_item, MUIM_SetUData, MEN_USER        , MUIA_Menuitem_Checked, TRUE);
   DoMethod(display_item, MUIM_SetUData, MEN_LOG         , MUIA_Menuitem_Checked, TRUE);
   DoMethod(display_item, MUIM_SetUData, MEN_BUTTONS     , MUIA_Menuitem_Checked, TRUE);
   DoMethod(display_item, MUIM_SetUData, MEN_STATUS      , MUIA_Menuitem_Checked, TRUE);
   DoMethod(display_item, MUIM_SetUData, MEN_SERIAL      , MUIA_Menuitem_Checked, TRUE);
}

///
/// clear_isp
VOID clear_isp(struct ISP *isp)
{
   if(isp->isp_ifaces.mlh_TailPred != (struct MinNode *)&isp->isp_ifaces)
   {
      struct Interface *iface1, *iface2;

      iface1 = (struct Interface *)isp->isp_ifaces.mlh_Head;
      while(iface2 = (struct Interface *)iface1->if_node.mln_Succ)
      {
         if(iface1->if_sana2configtext)
            FreeVec(iface1->if_sana2configtext);
         if(iface1->if_configparams)
            FreeVec(iface1->if_configparams);

         if(iface1->if_events.mlh_TailPred != (struct MinNode *)&iface1->if_events)
         {
            struct ScriptLine *sl1, *sl2;

            sl1 = (struct ScriptLine *)iface1->if_events.mlh_Head;
            while(sl2 = (struct ScriptLine *)sl1->sl_node.mln_Succ)
            {
               Remove((struct Node *)sl1);
               FreeVec(sl1);
               sl1 = sl2;
            }
         }
         Remove((struct Node *)iface1);
         FreeVec(iface1);
         iface1 = iface2;
      }
   }

   if(isp->isp_loginscript.mlh_TailPred != (struct MinNode *)&isp->isp_loginscript)
   {
      struct ScriptLine *sl1, *sl2;

      sl1 = (struct ScriptLine *)isp->isp_loginscript.mlh_Head;
      while(sl2 = (struct ScriptLine *)sl1->sl_node.mln_Succ)
      {
         Remove((struct Node *)sl1);
         FreeVec(sl1);
         sl1 = sl2;
      }
   }

   bzero(isp, sizeof(struct ISP));
   NewList((struct List *)&isp->isp_nameservers);
   NewList((struct List *)&isp->isp_domainnames);
   NewList((struct List *)&isp->isp_ifaces);
   NewList((struct List *)&isp->isp_loginscript);
}

///
/// iterate_ifacelist
VOID iterate_ifacelist(struct MinList *list, int set_mode)
{
   struct Interface *iface;

   if(list->mlh_TailPred != (struct MinNode *)list)
   {
      iface = (struct Interface *)list->mlh_Head;
      while(iface->if_node.mln_Succ)
      {
         switch(set_mode)
         {
            case 0:  // put all offline
               if(iface->if_flags & IFL_IsOnline)
                  iface->if_flags |= IFL_PutOffline;
               break;
            case 1:  // put only IFL_AutoOnline online
               if((iface->if_flags & IFL_AutoOnline) && !(iface->if_flags & IFL_IsOnline))
                  iface->if_flags |= IFL_PutOnline;
               break;
            case 2:  // put all online
               if(!(iface->if_flags & IFL_IsOnline))
                  iface->if_flags |= IFL_PutOnline;
               break;
         }
         iface = (struct Interface *)iface->if_node.mln_Succ;
      }
   }
}

///
/// is_one_online
BOOL is_one_online(struct MinList *list)
{
   struct Interface *iface;

   if(list->mlh_TailPred != (struct MinNode *)list)
   {
      iface = (struct Interface *)list->mlh_Head;
      while(iface->if_node.mln_Succ)
      {
         if(iface->if_flags & IFL_IsOnline)
            return(TRUE);
         iface = (struct Interface *)iface->if_node.mln_Succ;
      }
   }
   return(FALSE);
}

///
/// find_by_name
struct Interface *find_by_name(struct MinList *list, STRPTR name)
{
   struct Interface *iface;

   if(list->mlh_TailPred != (struct MinNode *)list)
   {
      iface = (struct Interface *)list->mlh_Head;
      while(iface->if_node.mln_Succ)
      {
         if(!strcmp(iface->if_name, name))
            return(iface);
         iface = (struct Interface *)iface->if_node.mln_Succ;
      }
   }
   return(NULL);
}

///
/// add_server
struct ServerEntry *add_server(struct MinList *list, STRPTR name)
{
   struct ServerEntry *server;

   if(!name || !*name || !strcmp(name, "0.0.0.0"))
      return(NULL);

   if(server = AllocVec(sizeof(struct ServerEntry), MEMF_ANY))
   {
      strncpy(server->se_name, name, sizeof(server->se_name) - 1);
      AddTail((struct List *)list, (struct Node *)server);
   }
   return(server);
}

///
/// find_server_by_name
struct ServerEntry *find_server_by_name(struct MinList *list, STRPTR name)
{
   struct ServerEntry *server;

   if(list->mlh_TailPred != (struct MinNode *)list)
   {
      server = (struct ServerEntry *)list->mlh_Head;
      while(server->se_node.mln_Succ)
      {
         if(!strcmp(server->se_name, name))
            return(server);
         server = (struct ServerEntry *)server->se_node.mln_Succ;
      }
   }
   return(NULL);
}

///

/// run_async
BOOL run_async(STRPTR file)
{
   BPTR ofh = NULL, ifh = NULL;
   BOOL success = FALSE;

   if(ofh = Open("NIL:", MODE_NEWFILE))
   {
      if(ifh = Open("NIL:", MODE_OLDFILE))
      {
         if(SystemTags(file,
            SYS_Output     , ofh,
            SYS_Input      , ifh,
            SYS_Asynch     , TRUE,
            NP_StackSize   , 8192,
            TAG_DONE) != -1)
               success = TRUE;

         if(!success)
            Close(ifh);
      }
      if(!success)
         Close(ofh);
   }
   return(success);
}

///
/// run_wb
BOOL run_wb(STRPTR cmd)
{
   struct MsgPort *hp, *mp;
   struct WBStartMsg wbsm;
   BOOL success = FALSE;

   if(mp = CreateMsgPort())
   {
      STRPTR buf, ptr;

      wbsm.wbsm_DirLock = NULL;
      if(buf = AllocVec(MAXPATHLEN + 1, MEMF_ANY | MEMF_CLEAR))
      {
         if(cmd)
            strncpy(buf, cmd, MAXPATHLEN);
         if(ptr = FilePart(buf))
            *ptr = NULL;
         wbsm.wbsm_DirLock = Lock(buf, SHARED_LOCK);

         FreeVec(buf);
      }

      wbsm.wbsm_Msg.mn_Node.ln_Pri  = 0;
      wbsm.wbsm_Msg.mn_ReplyPort    = mp;
      wbsm.wbsm_Name                = cmd;
      wbsm.wbsm_Stack               = 8192;
      wbsm.wbsm_Prio                = 0;
      wbsm.wbsm_NumArgs             = NULL;
      wbsm.wbsm_ArgList             = NULL;

      Forbid();
      if(hp = FindPort(WBS_PORTNAME))
         PutMsg(hp, (struct Message *)&wbsm);
      Permit();

      /* No WBStart-Handler, try to start it! */
      if(!hp)
      {
         if(run_async(WBS_LOADNAME))
         {
            int i;

            for(i = 0; i < 10; i++)
            {
               Forbid();
               if(hp = FindPort(WBS_PORTNAME))
                  PutMsg(hp, (struct Message *)&wbsm);
               Permit();
               if(hp)
                  break;
               Delay(25);
            }
         }
      }

      if(hp)
      {
         WaitPort(mp);
         GetMsg(mp);
         success = wbsm.wbsm_Stack;    // Has tool been started?
      }

      if(wbsm.wbsm_DirLock)
         UnLock(wbsm.wbsm_DirLock);
      DeleteMsgPort(mp);
   }
   return(success);
}

///
/// exec_command
VOID exec_command(STRPTR command, int how)
{
   char buf[MAXPATHLEN];

   *buf = NULL;
   switch(how)
   {
      case 1: // WB:
         run_wb(command);
         break;
      case 2: // SCRIPT
         strcpy(buf, "C:Execute ");
         strncat(buf, command, MAXPATHLEN);
         SystemTags(buf, TAG_DONE);
         break;
      case 3: // AREXX:
         strcpy(buf, "SYS:Rexxc/rx ");
         strncat(buf, command, MAXPATHLEN);
         SystemTags(buf, TAG_DONE);
         break;
      default: // EXEC_CLI
         SystemTags(command, TAG_DONE);
         break;
   }
}

///
/// exec_event
VOID exec_event(struct MinList *list, int event)
{
   if(list->mlh_TailPred != (struct MinNode *)list)
   {
      struct ScriptLine *sl = (struct ScriptLine *)list->mlh_Head;

      while(sl->sl_node.mln_Succ)
      {
         if(sl->sl_command == event)
            exec_command(sl->sl_contents, sl->sl_userdata);

         sl = (struct ScriptLine *)sl->sl_node.mln_Succ;
      }
   }
}

///
/// launch_amitcp
BOOL launch_amitcp(VOID)
{
   Object *window = NULL, *TX_Info = NULL;
   struct Library *SocketBase;
   int i;
   BOOL success = FALSE;

   if(Config.cnf_flags & CFL_ShowStatusWin)
   {
      if(window = WindowObject,
         MUIA_Window_Title       , NULL,
         MUIA_Window_Activate    , FALSE,
         MUIA_Window_CloseGadget , FALSE,
         MUIA_Window_LeftEdge    , MUIV_Window_LeftEdge_Centered,
         MUIA_Window_TopEdge     , MUIV_Window_TopEdge_Centered,
         MUIA_Window_DepthGadget , FALSE,
         MUIA_Window_SizeGadget  , FALSE,
         MUIA_Window_DragBar     , FALSE,
         WindowContents, VGroup,
            Child, TX_Info = MakeText(GetStr(MSG_TX_LoadingAmiTCP), TRUE),
         End,
      End)
      {
         set(TX_Info, MUIA_Text_PreParse, "\033c");
         DoMethod(app, OM_ADDMEMBER, window);
         set(window, MUIA_Window_Open, TRUE);
      }
   }

   if(!FindPort((STRPTR)AmiTCP_PortName))
   {
      if((SysBase->AttnFlags & AFF_68020) && (GetFileSize("AmiTCP:kernel/AmiTCP.020") > 0))
         run_async((Config.cnf_flags & CFL_Debug ? "AmiTCP:kernel/AmiTCP.020 debug" : "AmiTCP:kernel/AmiTCP.020"));
      else
         run_async((Config.cnf_flags & CFL_Debug ? "AmiTCP:kernel/AmiTCP debug" : "AmiTCP:kernel/AmiTCP"));
   }

   /* wait until AmiTCP is running */
   i = 0;
   while(!FindPort((STRPTR)AmiTCP_PortName) && i++ < (waitstack * 2))
      Delay(25);

   if(FindPort((STRPTR)AmiTCP_PortName))
   {
      if(!LockSocketBase)
      {
         LockSocketBase = NCL_OpenSocket();;
         if(SocketBase = LockSocketBase)
         {
            if(Config.cnf_flags & CFL_StartupLoopback)
            {
               if(TX_Info)
                  set(TX_Info, MUIA_Text_Contents, GetStr(MSG_TX_ConfiguringLoopback));
               config_lo0(SocketBase);
            }

            if(Config.cnf_flags & CFL_StartupTCP)
            {
               BOOL mount_tcp = TRUE;
               struct DosList *dlist;

               if(dlist = LockDosList(LDF_DEVICES | LDF_READ))
               {
                  if(FindDosEntry(dlist, "TCP", LDF_DEVICES))
                     mount_tcp = FALSE;
                  UnLockDosList(LDF_DEVICES | LDF_READ);
               }
               if(mount_tcp)
               {
                  if(TX_Info)
                     set(TX_Info, MUIA_Text_Contents, GetStr(MSG_TX_MountingTCP));
                  run_async("C:Mount TCP: from AmiTCP:Devs/Inet-mountlist");
               }
            }

            if(Config.cnf_startup)
            {
               if(TX_Info)
                  set(TX_Info, MUIA_Text_Contents, GetStr(MSG_TX_ExecutingStartup));
               exec_command(Config.cnf_startup, Config.cnf_startuptype);
            }

            if(Config.cnf_flags & CFL_StartupInetd)
            {
               if(TX_Info)
                  set(TX_Info, MUIA_Text_Contents, GetStr(MSG_TX_LaunchingInetd));
               run_async("AmiTCP:bin/inetd");
            }

            SocketBase = NULL;
            success = TRUE;
         }
      }
      else
         success = TRUE;
   }
   if(window)
   {
      set(window, MUIA_Window_Open, FALSE);
      DoMethod(app, OM_REMMEMBER, window);
      MUI_DisposeObject(window);
   }
   return(success);
}

///
/// syslog_AmiTCP
VOID syslog_AmiTCP(struct Library *SocketBase, ULONG level, const STRPTR format, ...)
{
   va_list va;

   va_start(va, format);
   if(SocketBase)
      vsyslog(level, format, (LONG *)va);
   va_end(va);
}

///
/// amirexx_do_command
LONG amirexx_do_command(const char *fmt, ...)
{
   char buf[256];
   struct MsgPort *port; /* our reply port */
   struct MsgPort *AmiTCP_Port;
   struct RexxMsg *rmsg;
   LONG rc = 20;         /* fail */

   vsprintf(buf, fmt, (STRPTR)(&fmt + 1));

   if(port = CreateMsgPort())
   {
      port->mp_Node.ln_Name = "BOOTPCONFIG";
      if(rmsg = CreateRexxMsg(port, NULL, (STRPTR)AmiTCP_PortName))
      {
         rmsg->rm_Action = RXCOMM;
         rmsg->rm_Args[0] = (STRPTR)buf;
         if(FillRexxMsg(rmsg, 1, 0))
         {
            Forbid();
            if(AmiTCP_Port = FindPort((STRPTR)AmiTCP_PortName))
            {
               PutMsg(AmiTCP_Port, (struct Message *)rmsg);
               Permit();
               do
               {
                  WaitPort(port);
               } while(GetMsg(port) != (struct Message *)rmsg);
               rc = rmsg->rm_Result1;
            }
            else
               Permit();

            ClearRexxMsg(rmsg, 1);
         }
         DeleteRexxMsg(rmsg);
      }
      DeleteMsgPort(port);
   }
   return(rc);
}

///
/// safe_put_to_port
BOOL safe_put_to_port(struct Message *message, STRPTR portname)
{
   struct MsgPort *port;

   Forbid();
   port = FindPort(portname);
   if(port)
      PutMsg(port, message);
   Permit();
   return((BOOL)(port ? TRUE : FALSE));
}

///

#define DECODE(c) ((c - 0x20) & 0x3F)
/// decrypt
VOID decrypt(STRPTR in, STRPTR out)
{
   STRPTR s, t;
   LONG l, c;

   s = in;
   t = out;
   c = *s++;
   l = DECODE(c);
   if (c != '\n' && l > 0)
   {
      while (l >= 4)
      {
         c = DECODE(s[0]) << 2 | DECODE(s[1]) >> 4;
         *t++ = c;
         c = DECODE(s[1]) << 4 | DECODE(s[2]) >> 2;
         *t++ = c;
         c = DECODE(s[2]) << 6 | DECODE(s[3]);
         *t++ = c;

         s += 4;
         l -= 3;
      }
      c = DECODE(s[0]) << 2 | DECODE(s[1]) >> 4;
      if (l >= 1)
         *t++ = c;
      c = DECODE(s[1]) << 4 | DECODE(s[2]) >> 2;
      if (l >= 2)
         *t++ = c;
      c = DECODE(s[2]) << 6 | DECODE(s[3]);
      if (l >= 3)
         *t++ = c;
      s += 4;
   }
   *t = NULL;
}

///

/// DoMainMethod
ULONG DoMainMethod(Object *obj, LONG MethodID, APTR data1, APTR data2, APTR data3)
{
   struct MainMessage *message, *reply_message;
   struct MsgPort *reply_port;
   ULONG ret = NULL;
   BOOL finished = FALSE;
   ULONG sig;

   if(reply_port = CreateMsgPort())
   {
      if(message = AllocVec(sizeof(struct MainMessage), MEMF_ANY | MEMF_CLEAR))
      {
         message->msg.mn_ReplyPort  = reply_port;
         message->msg.mn_Length     = sizeof(struct MainMessage);

         message->obj      = obj;
         message->MethodID = MethodID;
         message->data1    = data1;
         message->data2    = data2;
         message->data3    = data3;

         PutMsg(MainPort, (struct Message *)message);

         while(!finished)
         {
            sig = Wait(1L << reply_port->mp_SigBit);  // must not be interuptable by ctrl_c because HandleMainMethod() needs the reply_port and the message to be still alive when it does a ReplyMsg()

            if(sig & (1L << reply_port->mp_SigBit))
            {
               while(reply_message = (struct MainMessage *)GetMsg(reply_port))
               {
                  if(reply_message->MethodID == MUIM_Genesis_Handshake)
                  {
                     ret = reply_message->result;
                     finished = TRUE;
                  }
               }
            }
         }
         FreeVec(message);
      }
      DeleteMsgPort(reply_port);
   }
   return(ret);
}

///
/// set_window
VOID set_window(Object *window, int state)
{
   switch(state)
   {
      case 1:  // open
         set(app, MUIA_Application_Iconified, FALSE);
         set(window, MUIA_Window_Open, TRUE);
         break;
      case 2:  // close
         set(window, MUIA_Window_Open, FALSE);
         break;
      case 3:  // iconify
         set(app, MUIA_Application_Iconified, TRUE);
         set(window, MUIA_Window_Open, TRUE);
         break;
   }
}

///

/// txtobjfunc
SAVEDS ASM LONG txtobjfunc(register __a2 Object *list, register __a1 Object *txt)
{
   char *x, *s;
   int i;

   get(txt, MUIA_Text_Contents, &s);

   i = 0;
   FOREVER
   {
      DoMethod(list, MUIM_List_GetEntry, i, &x);
      if(!x)
      {
         set(list, MUIA_List_Active, MUIV_List_Active_Off);
         break;
      }
      else
      {
         if(!stricmp(x, s))
         {
            set(list, MUIA_List_Active, i);
            break;
         }
      }

      i++;
   }
   return(TRUE);
}
struct Hook txtobj_hook = { { 0,0 }, (VOID *)txtobjfunc , NULL, NULL };

///
/// objtxtfunc
SAVEDS ASM VOID objtxtfunc(register __a2 Object *list,register __a1 Object *txt)
{
   char *x;

   DoMethod(list, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &x);
   if(x)
      set(txt, MUIA_Text_Contents, x);
}
struct Hook objtxt_hook = { { 0,0 }, (VOID *)objtxtfunc , NULL, NULL };

///
/// des_func
SAVEDS ASM VOID des_func(register __a2 APTR pool, register __a1 APTR ptr)
{
   if(ptr)
      FreeVec(ptr);
}

///

