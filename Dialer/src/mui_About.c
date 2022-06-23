/// includes
#include "/includes.h"

#include "/Genesis.h"
#include "/genesis.lib/pragmas/nc_lib.h"
#include "rev.h"
#include "Strings.h"
#include "mui.h"
#include "mui_About.h"
#include "mui_MainWindow.h"
#include "protos.h"

///
/// external variables
extern struct Library *NetConnectBase;
extern Object *app, *win;

///

/// About_New
ULONG About_New(struct IClass *cl, Object *obj, Msg msg)
{
   struct About_Data tmp;
   char info1[512], info2[512];
#ifdef DEMO
   char demo[256];
   ULONG days_running;
#endif

   strncpy(info1, GetStr(MSG_TX_About1a), 512);
   strncat(info1, " " VERTAG, 512);
#ifdef NETCONNECT
   strncat(info1, " - NetConnect", 512);
#else
#ifdef VT
   strncat(info1, " - special OEM version for VillageTronic", 512);
#endif
#endif
   strncat(info1, "\n\n\033n\033cCopyright � 1997-99 by\n", 512);
   strncat(info1, "\0338Michael Neuweiler & Active Technologies\0332\033n\033c\n", 512);
   strncat(info1, GetStr(MSG_TX_About1b), 512);

   strncpy(info2, GetStr(MSG_TX_About2), 512);
   strncat(info2, NCL_GetOwner(), 512);
   strncat(info2, "\n(", 512);
   strncat(info2, NCL_GetSerial(), 512);
   strncat(info2, ")\n\n\033n\033cARexx port:\n\033i", 512);
   strncat(info2, (STRPTR)xget(app, MUIA_Application_Base), 512);
   strncat(info2, "'\n", 512);
#ifdef DEMO
   if((days_running = check_date()) > WARN_DAYS)
      sprintf(demo, "\n\033b\033cDEMO VERSION\nWARNING: Timeout limit exceeded !\nThis demo will become inoperative in %ld days.\n", MAX_DAYS - days_running);
   else
      sprintf(demo, "\n\033b\033cDEMO VERSION\033n\nThis version will timeout in %ld days.\n", WARN_DAYS - days_running);
#ifndef BETA
   strcat(demo, "Each session is limited to one hour.\n");
#endif
#endif

   if(obj = (Object *)DoSuperNew(cl, obj,
      MUIA_Window_Title       , "GENESiS � Copyright Information",
      MUIA_Window_ID          , MAKE_ID('A','B','O','U'),
      MUIA_Window_RefWindow   , win,
      MUIA_Window_LeftEdge    , MUIV_Window_LeftEdge_Centered,
      MUIA_Window_TopEdge     , MUIV_Window_TopEdge_Centered,
      MUIA_Window_Height      , MUIV_Window_Height_MinMax(50),
      WindowContents, VGroup,
         MUIA_Background, MUII_RequesterBack,
         Child, ScrollgroupObject,
            MUIA_CycleChain, 1,
            MUIA_Background, MUII_ReadListBack,
            MUIA_Scrollgroup_FreeHoriz, FALSE,
            MUIA_Scrollgroup_Contents, VirtgroupObject,
               ReadListFrame,
               Child, TextObject,
                  MUIA_Text_Contents, info1,
               End,
#ifdef DEMO
               Child, MUI_MakeObject(MUIO_HBar, 2),
               Child, TextObject,
                  MUIA_Text_Contents, demo,
                  MUIA_Font         , MUIV_Font_Big,
               End,
#endif
               Child, MUI_MakeObject(MUIO_HBar, 2),
               Child, TextObject,
                  MUIA_Text_Contents, info2,
               End,
               Child, MUI_MakeObject(MUIO_HBar, 2),
               Child, TextObject,
                  MUIA_Text_Contents, GetStr(MSG_TX_About3),
               End,
               Child, MUI_MakeObject(MUIO_HBar, 2),
               Child, TextObject,
                  MUIA_Text_Contents, GetStr(MSG_TX_About4),
               End,
            End,
         End,
         Child, HGroup,
            Child, HSpace(0),
            Child, tmp.BT_Button = MakeButton(MSG_BT_Okay),
            Child, HSpace(0),
         End,
      End))
   {
      struct About_Data *data = INST_DATA(cl,obj);

      *data = tmp;

      set(tmp.BT_Button, MUIA_CycleChain, 1);
      set(obj, MUIA_Window_ActiveObject, tmp.BT_Button);

      DoMethod(obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE ,
         MUIV_Notify_Application, 6, MUIM_Application_PushMethod,
         win, 3, MUIM_MainWindow_DisposeWindow, obj);
      DoMethod(data->BT_Button, MUIM_Notify, MUIA_Pressed, FALSE ,
         MUIV_Notify_Application, 6, MUIM_Application_PushMethod,
         win, 3, MUIM_MainWindow_DisposeWindow, obj);
   }
   return((ULONG)obj);
}

///
/// About_Dispatcher
SAVEDS ASM ULONG About_Dispatcher(register __a0 struct IClass *cl, register __a2 Object *obj, register __a1 Msg msg)
{
   if(msg->MethodID == OM_NEW)
      return(About_New(cl,obj,(APTR)msg));

   return(DoSuperMethodA(cl, obj, msg));
}

///

