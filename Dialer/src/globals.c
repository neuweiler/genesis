/// Includes
#include "/includes.h"
#pragma header

#include "Strings.h"
#define USE_SCRIPT_COMMANDS
#define USE_EVENT_COMMANDS
#include "/Genesis.h"
#include "mui.h"
#include "protos.h"

///

/// Libraries
struct   Library              *MUIMasterBase = NULL;
struct   Library              *TimerBase     = NULL;
struct   Library              *SocketBase    = NULL;
struct   Library              *LockSocketBase= NULL;
struct   Library              *OwnDevUnitBase= NULL;
#ifdef DEMO
struct   Library              *BattClockBase = NULL;
#endif

///

/// Other data
struct Config Config;

struct Process *proc;
struct StackSwapStruct StackSwapper;

struct Catalog       *cat       = NULL;   /* pointer to our locale catalog */
struct MsgPort       *MainPort  = NULL;   /* port to comunicate with mainProcess */
struct IOExtSer      *SerReq    = NULL;   /* Serial IORequest */
struct MsgPort       *SerPort   = NULL;   /* Serial reply port */
struct timerequest   *TimeReq   = NULL;
struct MsgPort       *TimePort  = NULL;

ULONG  LogNotifySignal = -1, ConfigNotifySignal = -1;
struct NotifyRequest log_nr, config_nr;
ULONG sigs = NULL;

const char AmiTCP_PortName[] = "AMITCP";
char config_file[MAXPATHLEN];
char connectspeed[41];

BOOL dialup = 0, SerialLocked = FALSE;

int h_errno;

///

/// MainMenu
struct NewMenu MainMenu[] =
{
   { NM_TITLE  , MSG_MENU_PROJECT   , 0                  , 0, 0, (APTR)0               },
   { NM_ITEM   , MSG_MENU_ABOUT     , "  ?", 0, 0, (APTR)MEN_ABOUT       },
   { NM_ITEM   , NM_BARLABEL        , 0                  , 0, 0, 0                     },
   { NM_ITEM   , MSG_MENU_ABOUTMUI  , 0                  , 0, 0, (APTR)MEN_ABOUT_MUI   },
   { NM_ITEM   , NM_BARLABEL        , 0                  , 0, 0, 0                     },
   { NM_ITEM   , MSG_MENU_QUIT      , "  Q" , 0, 0, (APTR)MEN_QUIT        },

   { NM_TITLE  , MSG_MENU_SETTINGS  , 0                  , 0, 0, (APTR)0               },
   { NM_ITEM   , MSG_MENU_GENESIS   , "  G"  , 0, 0, (APTR)MEN_GENESIS         },
   { NM_ITEM   , MSG_MENU_MUI       , "  M"  , 0, 0, (APTR)MEN_MUI         },

   { NM_END    , NULL               , 0                  , 0, 0, (APTR)0               },
};
///

/// MUI Class Pointers
struct MUI_CustomClass  *CL_MainWindow          = NULL;
struct MUI_CustomClass  *CL_Online              = NULL;
struct MUI_CustomClass  *CL_IfaceReq            = NULL;
struct MUI_CustomClass  *CL_Led                 = NULL;

///
/// MUI stuff
Object   *app        = NULL;
Object   *win        = NULL;
Object   *status_win = NULL;

struct Hook des_hook   = { {NULL, NULL}, (VOID *)des_func  , NULL, NULL};

///

