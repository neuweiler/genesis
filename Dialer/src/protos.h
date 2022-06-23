/// amiga.c
LONG xget(Object *obj,ULONG attribute);
ULONG DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...);
Object *MakeButton(STRPTR string);
Object *MakeText(STRPTR string, BOOL set_min);
STRPTR GetStr(STRPTR idstr);
BOOL SetEnvDOS(STRPTR name, STRPTR string, LONG len, BOOL save);
LONG GetEnvDOS(STRPTR name, STRPTR buffer, LONG max_len);
LONG amirexx_do_command(const char *fmt, ...);
VOID EscapeString(STRPTR buffer, STRPTR str);
BOOL parse_arguments(VOID);
VOID clear_config(struct Config *conf);
VOID clear_isp(struct ISP *isp);
VOID iterate_ifacelist(struct MinList *list, int set_mode);
BOOL is_one_online(struct MinList *list);
struct Interface *find_by_name(struct MinList *list, STRPTR name);
struct ServerEntry *find_server_by_name(struct MinList *list, STRPTR name);
struct ServerEntry *add_server(struct MinList *list, STRPTR name);
BOOL run_async(STRPTR file);
BOOL run_wb(STRPTR cmd);
VOID exec_command(STRPTR command, int how);
VOID exec_event(struct MinList *list, int event);
BOOL launch_amitcp(VOID);
VOID syslog_AmiTCP(ULONG level, const STRPTR format, ...);
VOID decrypt(STRPTR in, STRPTR out);
ULONG DoMainMethod(Object *obj, LONG MethodID, APTR data1, APTR data2, APTR data3);
SAVEDS ASM VOID des_func(register __a2 APTR pool, register __a1 APTR ptr);
LONG   get_file_size(STRPTR file);

///
/// main.c
VOID HandleMainMethod(struct MsgPort *port);
#ifdef DEMO
ULONG check_date(VOID);
#endif

///
/// mui
SAVEDS ASM ULONG MainWindow_Dispatcher(register __a0 struct IClass *cl, register __a2 Object *obj, register __a1 Msg msg);
SAVEDS ASM ULONG Online_Dispatcher(register __a0 struct IClass *cl, register __a2 Object *obj, register __a1 Msg msg);
SAVEDS ASM ULONG IfaceReq_Dispatcher(register __a0 struct IClass *cl, register __a2 Object *obj, register __a1 Msg msg);
SAVEDS ASM ULONG Led_Dispatcher(register __a0 struct IClass *cl, register __a2 Object *obj, register __a1 Msg msg);
SAVEDS ASM ULONG About_Dispatcher(register __a0 struct IClass *cl, register __a2 Object *obj, register __a1 Msg msg);
SAVEDS ASM ULONG NetInfo_Dispatcher(register __a0 struct IClass *cl, register __a2 Object *obj, register __a1 Msg msg);
SAVEDS ASM APTR provider_rxfunc(register __a0 struct Hook *hook, register __a2 Object *appl, register __a1 ULONG *arg);
SAVEDS ASM APTR user_rxfunc(register __a0 struct Hook *hook, register __a2 Object *appl, register __a1 ULONG *arg);
SAVEDS ASM APTR connect_rxfunc(register __a0 struct Hook *hook, register __a2 Object *appl, register __a1 ULONG *arg);
SAVEDS ASM APTR disconnect_rxfunc(register __a0 struct Hook *hook, register __a2 Object *appl, register __a1 ULONG *arg);
SAVEDS ASM APTR status_rxfunc(register __a0 struct Hook *hook, register __a2 Object *appl, register __a1 ULONG *arg);
SAVEDS ASM APTR window_rxfunc(register __a0 struct Hook *hook, register __a2 Object *appl, register __a1 ULONG *arg);

///
/// serial.c
VOID serial_stopread(VOID);
VOID serial_startread(STRPTR data, LONG len);
VOID serial_send(STRPTR cmd, LONG len);
BOOL serial_waitfor(STRPTR string, int secs);
BOOL serial_carrier(VOID);
BOOL serial_dsr(VOID);
VOID serial_hangup(VOID);
VOID serial_clear(VOID);
BOOL serial_create(VOID);
VOID serial_delete(VOID);

///
BOOL config_lo0(VOID);

/// fixpath.c

VOID __regargs DeleteCLI(struct CommandLineInterface *CLI);
struct CommandLineInterface * __regargs CloneCLI(struct Message *Message);
///

