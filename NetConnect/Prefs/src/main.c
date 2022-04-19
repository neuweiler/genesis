#include "globals.c"

extern VOID exit_classes(VOID);
extern BOOL init_classes(VOID);


/*
 * close the libraries
 */

VOID exit_libs(VOID)
{
	if(cat)					CloseCatalog(cat);

	if(DataTypesBase)		CloseLibrary(DataTypesBase);
	if(IFFParseBase)		CloseLibrary(IFFParseBase);
	if(IntuitionBase)		CloseLibrary(IntuitionBase);
	if(UtilityBase)		CloseLibrary(UtilityBase);
	if(MUIMasterBase)		CloseLibrary(MUIMasterBase);
	if(LocaleBase)			CloseLibrary(LocaleBase);

	cat			= NULL;
	IFFParseBase	= IntuitionBase	= UtilityBase		=
	MUIMasterBase	= LocaleBase		= DataTypesBase	= NULL;
}


/*
 * open all needed libraries
 */

BOOL init_libs(VOID)
{
	if(LocaleBase	= OpenLibrary("locale.library", 38))
		cat = OpenCatalog(NULL, "netconnect.catalog", OC_BuiltInLanguage, "english", TAG_DONE);

	MUIMasterBase	= OpenLibrary("muimaster.library"	, 11);
	UtilityBase		= OpenLibrary("utility.library"		, 36);
	IntuitionBase	= OpenLibrary("intuition.library"	, 36);
	IFFParseBase	= OpenLibrary("iffparse.library"		, 0);
	DataTypesBase	= OpenLibrary("datatypes.library"	, 39);

	if(MUIMasterBase && UtilityBase && IntuitionBase && IFFParseBase)
		return(TRUE);
	/* the program will still work without locale.library */

	exit_libs();
	return(FALSE);
}


/*
 * extract the correct string from a locale made by "cat2h"
 */

STRPTR GetStr(STRPTR idstr)
{
	STRPTR local;

	local = idstr + 2;

	if(LocaleBase)
		return((STRPTR)GetCatalogStr(cat, *(UWORD *)idstr, local));

	return(local);
}

VOID LocalizeNewMenu(struct NewMenu *nm)
{
	for (;nm->nm_Type!=NM_END;nm++)
		if (nm->nm_Label != NM_BARLABEL)
			nm->nm_Label = GetStr(nm->nm_Label);
}

/*
 * program entry point
 */

LONG main(VOID)
{
	ULONG sigs = NULL;

	if(init_libs())
	{
		if(init_classes())
		{
			app = ApplicationObject,
				MUIA_Application_Author			, "Michael Neuweiler",
				MUIA_Application_Base			, "NetConnect_Config",
				MUIA_Application_Title			, "NetConnect Config",
				MUIA_Application_Version		, "$VER: NetConnect Config 1.0 (13.06.96)",
				MUIA_Application_Copyright		, GetStr(MSG_AppCopyright),
				MUIA_Application_Description	, GetStr(MSG_AppDescription),
				MUIA_Application_Window			, win = NewObject(CL_AmiTCPPrefs->mcc_Class, NULL, TAG_DONE),
				End;

			if(app)
			{
				DoMethod(win, MUIM_ServerPrefs_PopList_Update, "AmiTCP:Providers", MUIV_ServerPrefs_PopString_Country);
				DoMethod(win, MUIM_ServerPrefs_PopList_Update, "AmiTCP:Providers", MUIV_ServerPrefs_PopString_PoP);
				DoMethod(win, MUIM_AmiTCPPrefs_LoadConfig, "AmiTCP:db/Provider.conf");

				set(win, MUIA_Window_Open, TRUE);
				while(DoMethod(app, MUIM_Application_NewInput, &sigs) != MUIV_Application_ReturnID_Quit)
				{
					if(sigs)
					{
						sigs = Wait(sigs | SIGBREAKF_CTRL_C);
						if(sigs & SIGBREAKF_CTRL_C)
							break;
					}
				}
				set(win, MUIA_Window_Open, FALSE);
				MUI_DisposeObject(app);
				app = NULL;
			}
			exit_classes();
		}
		exit_libs();
	}
	else
		return(RETURN_FAIL);

	return(RETURN_OK);
}
