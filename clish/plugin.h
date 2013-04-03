/*
 * plugin.h
 */
#ifndef _clish_plugin_h
#define _clish_plugin_h

#include "lub/types.h"

/* Symbol */

/* Symbol types. Functions with different definition. */
typedef enum {
	CLISH_SYM_TYPE_NONE = 0, /* None */
	CLISH_SYM_TYPE_ACTION, /* Common builtin symbol */
	CLISH_SYM_TYPE_INIT,
	CLISH_SYM_TYPE_FINI,
	CLISH_SYM_TYPE_ACCESS,
	CLISH_SYM_TYPE_CONFIG,
	CLISH_SYM_TYPE_LOG,
	CLISH_SYM_TYPE_MAX /* Number of elements */
} clish_sym_type_e;

typedef struct clish_sym_s clish_sym_t;
typedef struct clish_plugin_s clish_plugin_t;

/* Plugin types */

/* Name of init function within plugin */
#define CLISH_PLUGIN_INIT_FNAME clish_plugin_init
#define CLISH_PLUGIN_INIT_NAME "clish_plugin_init"
#define CLISH_PLUGIN_INIT_FUNC(name) int name(clish_plugin_t *plugin)
#define CLISH_PLUGIN_INIT CLISH_PLUGIN_INIT_FUNC(CLISH_PLUGIN_INIT_FNAME)

#define CLISH_PLUGIN_SYM(name) int name(void *clish_context, const char *script, char **out)
#define CLISH_HOOK_INIT(name) int name(void *clish_context)
#define CLISH_HOOK_FINI(name) int name(void *clish_context)
#define CLISH_HOOK_ACCESS(name) int name(void *clish_context, const char *access)
#define CLISH_HOOK_CONFIG(name) int name(void *clish_context)
#define CLISH_HOOK_LOG(name) int name(void *clish_context, const char *line, int retcode)

/* Default syms */
#define CLISH_DEFAULT_SYM "clish_script@clish" /* Builtin symbol to use by default */
#define CLISH_DEFAULT_ACCESS "clish_hook_access@clish"
#define CLISH_DEFAULT_CONFIG "clish_hook_config@clish"
#define CLISH_DEFAULT_LOG "clish_hook_log@clish"

/* typedef void clish_shell_cmd_line_fn_t(clish_context_t *context, const char *cmd_line); */

typedef CLISH_PLUGIN_INIT_FUNC(clish_plugin_init_t);
typedef CLISH_PLUGIN_SYM(clish_hook_action_fn_t);
typedef CLISH_HOOK_INIT(clish_hook_init_fn_t);
typedef CLISH_HOOK_FINI(clish_hook_fini_fn_t);
typedef CLISH_HOOK_ACCESS(clish_hook_access_fn_t);
typedef CLISH_HOOK_CONFIG(clish_hook_config_fn_t);
typedef CLISH_HOOK_LOG(clish_hook_log_fn_t);

/* Helpers */
#define SYM_FN(TYPE,SYM) (*((clish_hook_##TYPE##_fn_t *)(clish_sym__get_func(SYM))))

/* Symbol */

int clish_sym_compare(const void *first, const void *second);
clish_sym_t *clish_sym_new(const char *name, void *func, int type);
void clish_sym_free(clish_sym_t *instance);
void clish_sym__set_func(clish_sym_t *instance, void *func);
void *clish_sym__get_func(clish_sym_t *instance);
void clish_sym__set_name(clish_sym_t *instance, const char *name);
char *clish_sym__get_name(clish_sym_t *instance);
void clish_sym__set_permanent(clish_sym_t *instance, bool_t permanent);
bool_t clish_sym__get_permanent(clish_sym_t *instance);
void clish_sym__set_plugin(clish_sym_t *instance, clish_plugin_t *plugin);
clish_plugin_t *clish_sym__get_plugin(clish_sym_t *instance);
void clish_sym__set_type(clish_sym_t *instance, int type);
int clish_sym__get_type(clish_sym_t *instance);
int clish_sym_clone(clish_sym_t *dst, clish_sym_t *src);

/* Plugin */

clish_plugin_t *clish_plugin_new(const char *name, const char *file);
void clish_plugin_free(clish_plugin_t *instance);
void *clish_plugin_load(clish_plugin_t *instance);
clish_sym_t *clish_plugin_get_sym(clish_plugin_t *instance,
	const char *name, int type);
clish_sym_t *clish_plugin_add_generic(clish_plugin_t *instance,
	void *func, const char *name, int type, bool_t permanent);
clish_sym_t *clish_plugin_add_sym(clish_plugin_t *instance,
	clish_hook_action_fn_t *func, const char *name);
clish_sym_t *clish_plugin_add_psym(clish_plugin_t *instance,
	clish_hook_action_fn_t *func, const char *name);
clish_sym_t *clish_plugin_add_hook(clish_plugin_t *instance,
	void *func, const char *name, int type);
clish_sym_t *clish_plugin_add_phook(clish_plugin_t *instance,
	void *func, const char *name, int type);
void clish_plugin_dump(const clish_plugin_t *instance);
char *clish_plugin__get_name(const clish_plugin_t *instance);
char *clish_plugin__get_file(const clish_plugin_t *instance);

#endif				/* _clish_plugin_h */
/** @} clish_plugin */