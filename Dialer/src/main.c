/// includes
#include "/includes.h"

#include <libraries/owndevunit.h>
#include "/Genesis.h"
#include "/genesis.lib/libraries/genesis.h"
#include "/genesis.lib/pragmas/genesis_lib.h"
#include "/genesis.lib/proto/genesis.h"
#include "/genesis.lib/pragmas/nc_lib.h"
#include "rev.h"
#include "Strings.h"
#include "mui.h"
#include "mui_MainWindow.h"
#include "mui_Online.h"
#include "mui_IfaceReq.h"
#include "mui_Led.h"
#include "mui_About.h"
#include "mui_NetInfo.h"
#include "protos.h"

///
/// defines
#define NEWSTACK_SIZE 16384

///
/// external variables
extern struct Process *proc;
extern struct StackSwapStruct StackSwapper;
extern struct ExecBase *SysBase;
extern struct Catalog    *cat;
extern struct Library    *MUIMasterBase, *LockSocketBase, *OwnDevUnitBase, *GenesisBase;
extern struct MUI_CustomClass *CL_MainWindow, *CL_Online, *CL_IfaceReq, *CL_Led, *CL_About, *CL_NetInfo;
extern struct MsgPort     *MainPort;
extern struct timerequest *TimeReq;
extern struct MsgPort     *TimePort;
extern ULONG  LogNotifySignal, ConfigNotifySignal;
extern struct NotifyRequest log_nr, config_nr;
extern struct NewMenu MainMenu[];
extern Object *app, *win;
extern struct Config Config;
extern ULONG sigs;
extern char config_file[], connectspeed[], default_provider[];
extern struct CommandLineInterface *LocalCLI;
extern BPTR OldCLI;
extern struct MUI_Command arexx_list[];
extern int waitstack;

#ifdef DEMO
extern struct Library *BattClockBase;
#endif
#ifdef NETCONNECT
extern struct Library *NetConnectBase;
#endif

///

/// exit_libs
VOID exit_libs(VOID)
{
   if(cat)
      CloseCatalog(cat);
   cat = NULL;

   if(GenesisBase)
      CloseLibrary(GenesisBase);
   GenesisBase = NULL;

   if(OwnDevUnitBase)
      CloseLibrary(OwnDevUnitBase);
   OwnDevUnitBase = NULL;

   if(MUIMasterBase)
      CloseLibrary(MUIMasterBase);
   MUIMasterBase = NULL;

#ifdef NETCONNECT
   if(NetConnectBase)
      CloseLibrary(NetConnectBase);
   NetConnectBase = NULL;
#endif
}

///
/// init_libs
BOOL init_libs(VOID)
{
   if(LocaleBase)
      cat = OpenCatalog(NULL, "Genesis.catalog", OC_BuiltInLanguage, "english", TAG_DONE);
#ifdef NETCONNECT
   if(!(NetConnectBase = OpenLibrary("netconnect.library", 5)))
      Printf(GetStr(MSG_TX_CouldNotOpenX), "netconnect.library\n");
#endif
   if(!(MUIMasterBase = OpenLibrary("muimaster.library", 11)))
      Printf(GetStr(MSG_TX_CouldNotOpenX), "muimaster.library\n");
   OwnDevUnitBase = OpenLibrary(ODU_NAME, 0);
   if(!(GenesisBase = OpenLibrary(GENESISNAME, 2)))
      Printf(GetStr(MSG_TX_CouldNotOpenX), "genesis.library (ver 2.0+)\n");

#ifdef NETCONNECT
   if(MUIMasterBase && GenesisBase && NetConnectBase)
   {
      if(NCL_GetOwner())
         return(TRUE);
      Printf("NetConnect registration failed.\n");
   }
#else
   if(MUIMasterBase && GenesisBase)
      return(TRUE);
#endif

   exit_libs();
   return(FALSE);
}

///

/// exit_classes
VOID exit_classes(VOID)
{
   if(CL_MainWindow)          MUI_DeleteCustomClass(CL_MainWindow);
   if(CL_Online)              MUI_DeleteCustomClass(CL_Online);
   if(CL_IfaceReq)            MUI_DeleteCustomClass(CL_IfaceReq);
   if(CL_Led)                 MUI_DeleteCustomClass(CL_Led);
   if(CL_About)               MUI_DeleteCustomClass(CL_About);
   if(CL_NetInfo)             MUI_DeleteCustomClass(CL_NetInfo);

   CL_MainWindow = CL_Online = CL_IfaceReq = CL_Led = CL_About = CL_NetInfo = NULL;
}

///
/// init_classes
BOOL init_classes(VOID)
{
   CL_MainWindow     = MUI_CreateCustomClass(NULL, MUIC_Window, NULL, sizeof(struct MainWindow_Data), MainWindow_Dispatcher);
   CL_Online         = MUI_CreateCustomClass(NULL, MUIC_Window, NULL, sizeof(struct Online_Data), Online_Dispatcher);
   CL_IfaceReq       = MUI_CreateCustomClass(NULL, MUIC_Window, NULL, sizeof(struct IfaceReq_Data), IfaceReq_Dispatcher);
   CL_About          = MUI_CreateCustomClass(NULL, MUIC_Window, NULL, sizeof(struct About_Data), About_Dispatcher);
   CL_NetInfo        = MUI_CreateCustomClass(NULL, MUIC_Window, NULL, sizeof(struct NetInfo_Data), NetInfo_Dispatcher);
   CL_Led            = MUI_CreateCustomClass(NULL, MUIC_Group , NULL, sizeof(struct Led_Data), Led_Dispatcher);

   if(CL_MainWindow && CL_Online && CL_IfaceReq && CL_Led && CL_About && CL_NetInfo)
      return(TRUE);

   exit_classes();
   return(FALSE);
}

///
/// exit_ports
VOID exit_ports(VOID)
{
   if(MainPort)
      DeleteMsgPort(MainPort);
   MainPort = NULL;

   if(TimeReq)
   {
#ifdef DEMO
      if(!CheckIO((struct IORequest *)TimeReq))
      {
         AbortIO((struct IORequest *)TimeReq);
         WaitIO((struct IORequest *)TimeReq);
      }
#endif

      CloseDevice((struct IORequest *)TimeReq);
      DeleteIORequest(TimeReq);
      TimerBase = NULL;
      TimeReq = NULL;
   }
   if(TimePort)
      DeleteMsgPort(TimePort);
   TimePort = NULL;
}

///
/// init_ports
BOOL init_ports(VOID)
{
   if(MainPort = CreateMsgPort())
   {
      if(TimePort = (struct MsgPort *)CreateMsgPort())
      {
         if(TimeReq = (struct timerequest *)CreateIORequest(TimePort, sizeof(struct timerequest)))
         {
            if(!(OpenDevice("timer.device", UNIT_VBLANK, (struct IORequest *)TimeReq, 0)))
            {
               TimerBase = &TimeReq->tr_node.io_Device->dd_Library;
#ifdef DEMO
               // add 1 hour timeout
               TimeReq->tr_node.io_Command   = TR_ADDREQUEST;
               TimeReq->tr_time.tv_secs      = 3600;
               TimeReq->tr_time.tv_micro     = NULL;
               SetSignal(0, 1L << TimePort->mp_SigBit);
               SendIO((struct IORequest *)TimeReq);
#endif

               return(TRUE);
            }
         }
      }
   }

   exit_ports();
   return(FALSE);
}

///

/// check_date
#ifdef DEMO
#include <resources/battclock.h>
#include <clib/battclock_protos.h>
ULONG check_date(VOID)
{
   struct FileInfoBlock *fib;
   ULONG days_running = 0;
   BOOL set_comment = FALSE;
   char file[50];
   BPTR lock;
   ULONG clock = NULL;

   strcpy(file, "libs:locale.library");

   if(BattClockBase = OpenResource("battclock.resource"))
   {
      clock = ReadBattClock();

      if(fib = AllocDosObject(DOS_FIB, NULL))
      {
         if(lock = Lock(file, ACCESS_READ))
         {
            Examine(lock, fib);
            UnLock(lock);

            if(strlen(fib->fib_Comment) > 4)
            {
               ULONG inst;

               inst = atol((STRPTR)(fib->fib_Comment + 2));
               if(inst > 8000000)   // did we put something in there once ?
               {
                  if((fib->fib_Comment[0] == '0') && (fib->fib_Comment[1] == '2'))
                     days_running = (clock - inst)/86400;
                  else
                     set_comment = TRUE;
               }
               else
                  set_comment = TRUE;
            }
            else
               set_comment = TRUE;
         }
         FreeDosObject(DOS_FIB, fib);
      }

      if(set_comment)
      {
         char buffer[21];

         sprintf(buffer, "02%ld", clock);
         SetComment(file, buffer);
      }
   }

   return(days_running);
}
#endif

///

/// LocalizeNewMenu
VOID LocalizeNewMenu(struct NewMenu *nm)
{
   for(; nm && nm->nm_Type!=NM_END; nm++)
   {
      if(nm->nm_Label != NM_BARLABEL)
         nm->nm_Label = GetStr(nm->nm_Label);
      if(nm->nm_CommKey)
         nm->nm_CommKey = GetStr(nm->nm_CommKey);
   }
}

///
/// HandleMainMethod
VOID HandleMainMethod(struct MsgPort *port)
{
   struct MainMessage *message;

   while(message = (struct MainMessage *)GetMsg(port))
   {
      if(message->MethodID == TCM_INIT ||
         message->MethodID == MUIM_MainWindow_SetStates ||
         message->MethodID == MUIM_List_Clear)
      {
         message->result = DoMethod(message->obj, message->MethodID);
      }
      else if(message->MethodID == MUIM_List_Remove ||
              message->MethodID == MUIM_List_Redraw)
      {
         message->result = DoMethod(message->obj, message->MethodID, message->data1);
      }
      else if(message->MethodID == MUIM_Set ||
              message->MethodID == TCM_WRITE ||
              message->MethodID == MUIM_List_InsertSingle ||
              message->MethodID == MUIM_List_GetEntry)
      {
         message->result = DoMethod(message->obj, message->MethodID, message->data1, message->data2);
      }
      else if(message->MethodID == MUIM_List_Select)
      {
         message->result = DoMethod(message->obj, message->MethodID, message->data1, message->data2, message->data3);
      }
      else if(message->MethodID == MUIM_Genesis_Get)
      {
         message->result = xget(message->obj, (LONG)message->data1);
      }
      message->MethodID = MUIM_Genesis_Handshake;
      ReplyMsg((struct Message *)message);
   }
}

///

/// Handler
VOID Handler(VOID)
{
   ULONG id;

   if(init_libs())
   {
      if(init_classes())
      {
         if(init_ports())
         {
            LocalizeNewMenu(MainMenu);

            if(app = ApplicationObject,
               MUIA_Application_Author       , "Michael Neuweiler",
               MUIA_Application_Base         , "GENESiS",
               MUIA_Application_Title        , "GENESiS",
               MUIA_Application_SingleTask   , TRUE,
#ifdef DEMO
               MUIA_Application_Version      , "$VER:GENESiS "VERTAG" (DEMO)",
#else
               MUIA_Application_Version      , "$VER:GENESiS "VERTAG,
#endif
               MUIA_Application_Copyright    , "Michael Neuweiler & Active Technologies 1997,98",
               MUIA_Application_Description  , GetStr(MSG_AppDescription),
               MUIA_Application_Commands     , arexx_list,
               MUIA_Application_Window       , win = NewObject(CL_MainWindow->mcc_Class, NULL, TAG_DONE),
            End)
            {
               struct MainWindow_Data *data = INST_DATA(CL_MainWindow->mcc_Class, win);
               struct User *user;

               DoMethod(app, MUIM_Notify, MUIA_Application_DoubleStart, MUIV_EveryTime, MUIV_Notify_Application, 2, MUIM_Application_ReturnID, ID_DOUBLESTART);

               connectspeed[0]      = NULL;
               default_provider[0]  = NULL;
               waitstack = 10;
               bzero(&Config, sizeof(struct Config));
               strcpy(config_file, DEFAULT_CONFIGFILE);
               DoMethod(win, MUIM_MainWindow_LoadConfig);

               if(parse_arguments())
               {
                  DeleteFile("T:AmiTCP.log");
                  if(launch_amitcp())
                  {
                     log_nr.nr_Name     = "T:AmiTCP.log";
                     log_nr.nr_FullName = NULL;
                     log_nr.nr_UserData = NULL;
                     log_nr.nr_Flags    = NRF_SEND_SIGNAL;

                     if((LogNotifySignal = AllocSignal(-1)) != -1)
                     {
                        log_nr.nr_stuff.nr_Signal.nr_Task = (struct Task *)FindTask(NULL);
                        log_nr.nr_stuff.nr_Signal.nr_SignalNum = LogNotifySignal;
                        StartNotify(&log_nr);
                     }

                     config_nr.nr_Name     = config_file;
                     config_nr.nr_FullName = NULL;
                     config_nr.nr_UserData = NULL;
                     config_nr.nr_Flags    = NRF_SEND_SIGNAL;

                     if((ConfigNotifySignal = AllocSignal(-1)) != -1)
                     {
                        config_nr.nr_stuff.nr_Signal.nr_Task = (struct Task *)FindTask(NULL);
                        config_nr.nr_stuff.nr_Signal.nr_SignalNum = ConfigNotifySignal;
                        StartNotify(&config_nr);
                     }

                     if(Config.cnf_flags & CFL_StartupIconify)
                        set(app, MUIA_Application_Iconified, TRUE);
                     if(Config.cnf_flags & CFL_StartupOpenWin)
                        set(win, MUIA_Window_Open, TRUE);

                     if(!(user = GetGlobalUser()))
                     {
                        char buf[41];

                        GetUserName(0, buf, 40);
                        user = GetUser(buf, NULL, NULL, NULL);
                     }
                     if(user)
                     {
                        SetGlobalUser(user);
                        set(data->TX_User, MUIA_Text_Contents, user->us_name);
                        FreeUser(user);

#ifdef DEMO
                        {
                           ULONG days_running;

                           days_running = check_date();

                           if(days_running > MAX_DAYS)
                              MUI_Request(app, win, 0, 0, GetStr(MSG_ReqBT_Okay), "Sorry, this demo version has timed out !");
                           else
                           {
                              DoMethod(win, MUIM_MainWindow_About);
#endif
                        if(*default_provider)
                           set(data->TX_Provider, MUIA_Text_Contents, default_provider);
                        else
                        {
                           STRPTR ptr;

                           DoMethod(data->LI_Providers, MUIM_List_GetEntry, 0, &ptr);
                           set(data->TX_Provider, MUIA_Text_Contents, ptr);
                        }

                        while((id = DoMethod(app, MUIM_Application_NewInput, &sigs)) != MUIV_Application_ReturnID_Quit)
                        {
#ifdef NETCONNECT
                           NCL_CallMeFrequently();
#endif
                           if(id == ID_DOUBLESTART)
                           {
                              if(!xget(win, MUIA_Window_Open))
                                 set(win, MUIA_Window_Open, TRUE);
                              MUI_Request(app, win, NULL, NULL, GetStr(MSG_ReqBT_Okay), GetStr(MSG_TX_GenesisAlreadyRunning));
                           }

                           if(sigs)
                           {
#ifdef DEMO
                              sigs |= (1L << TimePort->mp_SigBit);
#endif
                              sigs = Wait(sigs | SIGBREAKF_CTRL_C | 1L << MainPort->mp_SigBit | 1L << LogNotifySignal | 1L << ConfigNotifySignal );

                              if(sigs & SIGBREAKF_CTRL_C)
                                 break;
                              if(sigs & (1L << MainPort->mp_SigBit))
                                 HandleMainMethod(MainPort);
                              if(sigs & (1L << LogNotifySignal))
                                 DoMethod(win, MUIM_MainWindow_UpdateLog);
                              if(sigs & (1L << ConfigNotifySignal))
                              {
                                 Delay(50);  // to give some time when saving
                                 DoMethod(win, MUIM_MainWindow_LoadConfig);
                                 DoMethod(win, MUIM_MainWindow_ChangeProvider, xget(data->TX_Provider, MUIA_Text_Contents));
                              }
#ifdef DEMO
                              if(sigs & (1L << TimePort->mp_SigBit))
                              {
                                 if(CheckIO((struct IORequest *)TimeReq))
                                 {
                                    WaitIO((struct IORequest *)TimeReq);

                                    // put ifaces offline, otherwise the user simply wouldn't have to press "okay"
                                    iterate_ifacelist(&data->isp.isp_ifaces, 0);
                                    DoMethod(win, MUIM_MainWindow_PutOffline);
                                    MUI_Request(app, win, NULL, NULL, GetStr(MSG_ReqBT_Okay), "\033cSorry, the 1 hour session limit has been reached.\n\nTerminating GENESiS...");
                                    break;
                                 }
                              }
#endif
                           }
                        }
#ifdef DEMO
                           }
                        }
#endif
                        set(win, MUIA_Window_Open, FALSE);

                        if(Config.cnf_shutdown)
                           exec_command(Config.cnf_shutdown, Config.cnf_shutdowntype);

                        iterate_ifacelist(&data->isp.isp_ifaces, 0);
                        DoMethod(win, MUIM_MainWindow_PutOffline);
                     }

                     if(LockSocketBase)      // gets opened in launch_amitcp()
                        CloseLibrary(LockSocketBase);
                     LockSocketBase = NULL;

                     amirexx_do_command("KILL");

                     clear_config(&Config);
                     if(ConfigNotifySignal != -1)
                     {
                        EndNotify(&config_nr);
                        FreeSignal(ConfigNotifySignal);
                     }
                     if(LogNotifySignal != -1)
                     {
                        EndNotify(&log_nr);
                        FreeSignal(LogNotifySignal);
                     }
                  }
                  else
                     MUI_Request(NULL, NULL, NULL, NULL, GetStr(MSG_BT_Abort), GetStr(MSG_TX_ErrorAmiTCPLaunch));
               }
               else
                  MUI_Request(NULL, NULL, NULL, NULL, GetStr(MSG_BT_Abort), GetStr(MSG_TX_ErrorParseArgs));

               MUI_DisposeObject(app);
               app = NULL;
            }
            exit_ports();
         }
         exit_classes();
      }
      else
         MUI_Request(NULL, NULL, NULL, NULL, GetStr(MSG_BT_Abort), GetStr(MSG_TX_CouldNotCreateMUIClasses));

      exit_libs();
   }
}

///

/// main
int main(int argc, char *argv[])
{
   proc = (struct Process *)FindTask(NULL);

   if(!proc->pr_CLI)
   {
      if(LocalCLI = CloneCLI(&WBenchMsg->sm_Message))
      {
         OldCLI = proc->pr_CLI;
         proc->pr_CLI = MKBADDR(LocalCLI);
      }
   }

   if(((ULONG)proc->pr_Task.tc_SPUpper - (ULONG)proc->pr_Task.tc_SPLower) < NEWSTACK_SIZE)
   {
      if(!(StackSwapper.stk_Lower = AllocVec(NEWSTACK_SIZE, MEMF_ANY)))
         exit(20);
      StackSwapper.stk_Upper   = (ULONG)StackSwapper.stk_Lower + NEWSTACK_SIZE;
      StackSwapper.stk_Pointer = (APTR)StackSwapper.stk_Upper;
      StackSwap(&StackSwapper);

      Handler();

      StackSwap(&StackSwapper);
      FreeVec(StackSwapper.stk_Lower);
   }
   else
      Handler();

   if(LocalCLI)
   {
      proc->pr_CLI = OldCLI;
      DeleteCLI(LocalCLI);
   }
}

///

