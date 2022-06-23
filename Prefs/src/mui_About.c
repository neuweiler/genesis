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
#include "images/logo.h"

///
/// external variables
extern struct Library *NetConnectBase;
extern ULONG logo_colors[];
extern UBYTE logo_body[];
extern Object *win, *app;

///

/// About_New
ULONG About_New(struct IClass *cl, Object *obj, Msg msg)
{
   struct About_Data tmp;
#ifdef NETCONNECT
   char info[512];

   sprintf(info, "\n\033cRegistered to:\n\033i%ls\n(%ls)\n\n\033n\033cARexx port:\n\033i'%ls'\n", NCL_GetOwner(), NCL_GetSerial(), xget(app, MUIA_Application_Base));
#endif

   if(obj = (Object *)DoSuperNew(cl, obj,
      MUIA_Window_Title       , "GENESiSPrefs � Copyright Information",
      MUIA_Window_ID          , MAKE_ID('A','B','O','U'),
      MUIA_Window_RefWindow   , win,
      MUIA_Window_LeftEdge    , MUIV_Window_LeftEdge_Centered,
      MUIA_Window_TopEdge     , MUIV_Window_TopEdge_Centered,
      MUIA_Window_Height      , MUIV_Window_Height_MinMax(50),
      WindowContents, VGroup,
         MUIA_Background, MUII_RequesterBack,
         Child, HGroup,
            TextFrame,
            MUIA_Background, MUII_GroupBack,
            Child, HVSpace,
            Child, BodychunkObject,
               MUIA_FixWidth             , LOGO_WIDTH ,
               MUIA_FixHeight            , LOGO_HEIGHT,
               MUIA_Bitmap_Width         , LOGO_WIDTH ,
               MUIA_Bitmap_Height        , LOGO_HEIGHT,
               MUIA_Bodychunk_Depth      , LOGO_DEPTH ,
               MUIA_Bodychunk_Body       , (UBYTE *)logo_body,
               MUIA_Bodychunk_Compression, LOGO_COMPRESSION,
               MUIA_Bodychunk_Masking    , LOGO_MASKING,
               MUIA_Bitmap_SourceColors  , (ULONG *)logo_colors,
               MUIA_Bitmap_Transparent   , 0,
            End,
            Child, HVSpace,
         End,
         Child, ScrollgroupObject,
            MUIA_CycleChain, 1,
            MUIA_Background, MUII_ReadListBack,
            MUIA_Scrollgroup_FreeHoriz, FALSE,
            MUIA_Scrollgroup_Contents, VirtgroupObject,
               ReadListFrame,
               Child, TextObject,
                  MUIA_Text_Contents, "\033c\033b\033u\nGENESiS Preferences\033n\033c\n\n" \
                                      "\033cVersion " VERTAG "\n\n"\
                                      "\033n\033cCopyright � 1997-98 by\n\0338Michael Neuweiler & Active Technologies\0332\033n\033c\n"\
                                      "All Rights Reserved\n",
               End,
#ifdef DEMO
               Child, MUI_MakeObject(MUIO_HBar, 2),
               Child, TextObject,
                  MUIA_Text_Contents, "\n\033b\033cDEMO VERSION\n",
                  MUIA_Font         , MUIV_Font_Big,
               End,
#endif
#ifdef NETCONNECT
               Child, MUI_MakeObject(MUIO_HBar, 2),
               Child, TextObject,
                  MUIA_Text_Contents, info,
               End,
#endif
               Child, MUI_MakeObject(MUIO_HBar, 2),
               Child, TextObject,
                  MUIA_Text_Contents, "\n\033cSupport site:\n\033ihttp://www.active-net.co.uk\n\n"\
                                      "\033nThanks go to:\nOliver Wagner, NSDI Group, Simone Tellini, Niels Heuer,\nChristoph Dietz, Thomas Bickel and many more.",
               End,
               Child, MUI_MakeObject(MUIO_HBar, 2),
               Child, TextObject,
                  MUIA_Text_Contents, "\n\033cGENESiS uses the MUI object library\n"\
                                      "MUI is � 1992-97 by Stefan Stunz <stuntz@sasg.com>\n"\
                                      "\n"\
                                      "NList.mcc, NListview.mcc are � 1996-98 Gilles Masson\n"\
                                      "\n"\
                                      "AmiTCP is � by NSDI\n"\
                                      "GENESiS uses code that was kindly provided by NSDI",
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
         win, 3, MUIM_MainWindow_AboutFinish, obj);
      DoMethod(data->BT_Button, MUIM_Notify, MUIA_Pressed, FALSE ,
         MUIV_Notify_Application, 6, MUIM_Application_PushMethod,
         win, 3, MUIM_MainWindow_AboutFinish, obj);
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

