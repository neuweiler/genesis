STRPTR GetStr(STRPTR idstr);

SAVEDS ASM int BrokerFunc(REG(a1) CxMsg *msg);
ULONG __stdargs DoSuperNew(struct IClass *cl, Object *obj, ULONG tag1, ...);
LONG xget(Object *obj, ULONG attribute);
Object *MakeKeyLabel2(STRPTR label, STRPTR control_char);
Object *MakeButton(STRPTR string);
Object *MakeKeyString(STRPTR string, LONG len, STRPTR control_char);
Object *MakeKeyCycle(STRPTR *array, STRPTR control_char);
Object *MakeKeySlider(LONG min, LONG max, LONG level, STRPTR control_char);
Object *MakePopAsl(Object *string, STRPTR title, BOOL drawers_only);
BOOL load_icon(struct Icon *icon);
Object *create_bodychunk(struct Icon *icon, BOOL frame);
VOID init_icon(struct Icon *icon, Object *list);
LONG get_file_size(STRPTR file);
STRPTR LoadFile(STRPTR file);
int find_max(VOID);
SAVEDS ASM LONG AppMsgFunc(REG(a2) APTR obj, REG(a1) struct AppMessage **x);
SAVEDS ASM LONG Editor_AppMsgFunc(REG(a2) APTR obj, REG(a1) struct AppMessage **x);
SAVEDS ASM LONG Button_AppMsgFunc(REG(a2) APTR obj, REG(a1) struct AppMessage **x);
BOOL editor_load(STRPTR file, Object *editor);
BOOL editor_save(STRPTR file, Object *editor);
BOOL editor_checksave(STRPTR file, Object *editor);
VOID play_sound(STRPTR file, LONG volume);
