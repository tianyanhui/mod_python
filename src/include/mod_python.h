#ifndef Mp_MOD_PYTHON_H
#define Mp_MOD_PYTHON_H

/*
 * Copyright 2004 Apache Software Foundation 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You
 * may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.  See the License for the specific language governing
 * permissions and limitations under the License.
 *
 * Originally developed by Gregory Trubetskoy.
 *
 *
 * mod_python.h 
 *
 * $Id$
 *
 * See accompanying documentation and source code comments 
 * for details.
 *
 */

/*
 * NOTE - NOTE - NOTE - NOTE
 *
 * If you are looking at mod_python.h, it is an auto-generated file on
 * UNIX.  This file is kept around for the Win32 platform which
 * doesnot use autoconf. Any changes to mod_python.h must also be
 * reflected in mod_python.h.in.
 */


/* Apache headers */
#include "httpd.h"
#define CORE_PRIVATE
#include "http_config.h"
#include "http_core.h"
#include "http_main.h"
#include "http_connection.h"
#include "http_protocol.h"
#include "http_request.h"
#include "util_script.h"
#include "util_filter.h"
#include "http_log.h"
#include "apr_strings.h"
#include "apr_lib.h"
#include "apr_hash.h"
#include "apr_fnmatch.h"
#include "scoreboard.h"
#include "ap_mpm.h"
#include "ap_mmn.h"
#include "mod_include.h"
#if !defined(OS2) && !defined(WIN32) && !defined(BEOS) && !defined(NETWARE)
#include "unixd.h"
#endif

#if !AP_MODULE_MAGIC_AT_LEAST(20050127,0)
/* Debian backported ap_regex_t to Apache 2.0 and
 * thus made official version checking break. */
#ifndef AP_REG_EXTENDED
typedef regex_t ap_regex_t;
#define AP_REG_EXTENDED REG_EXTENDED
#define AP_REG_ICASE REG_ICASE
#endif
#endif

/* Python headers */
/* this gets rid of some compile warnings */
#if defined(_POSIX_THREADS)
#undef _POSIX_THREADS
#endif
#include "Python.h"
#include "structmember.h"

#if defined(WIN32) && !defined(WITH_THREAD)
#error Python threading must be enabled on Windows
#endif

#if !defined(WIN32)
#include <sys/socket.h>
#endif

/* pool given to us in ChildInit. We use it for 
   server.register_cleanup() */
extern apr_pool_t *child_init_pool;

/* Apache module declaration */
extern module AP_MODULE_DECLARE_DATA python_module;

#include "mpversion.h"
#include "util.h"
#include "hlist.h"
#include "hlistobject.h"
#include "tableobject.h"
#include "serverobject.h"
#include "connobject.h"
#include "_apachemodule.h"
#include "requestobject.h"
#include "filterobject.h"
#include "_pspmodule.h"

/** Things specific to mod_python, as an Apache module **/

#define MP_CONFIG_KEY "mod_python_config"
#define VERSION_COMPONENT "mod_python/" MPV_STRING
#define MODULENAME "mod_python.apache"
#define INITFUNC "init"
#define MAIN_INTERPRETER "main_interpreter"
#define FILTER_NAME "MOD_PYTHON"

/* used in python_directive_handler */
#define SILENT 1
#define NOTSILENT 0

/* MAX_LOCKS can now be set as a configure option
 * ./configure --with-max-locks=INTEGER
 */
#define MAX_LOCKS 8 

/* MUTEX_DIR can be set as a configure option
 * ./configure --with-mutex-dir=/path/to/dir
 */
#define MUTEX_DIR "/tmp" 

/* python 2.3 no longer defines LONG_LONG, it defines PY_LONG_LONG */
#ifndef LONG_LONG
#define LONG_LONG PY_LONG_LONG
#endif

/* structure to hold interpreter data */
typedef struct {
    PyInterpreterState *istate;
    PyObject *obcallback;
} interpreterdata;

/* global configuration parameters */
typedef struct
{
    apr_global_mutex_t **g_locks;
    int                  nlocks;
    int                  parent_pid;
} py_global_config;

/* structure describing per directory configuration parameters */
typedef struct {
    int           authoritative;
    char         *config_dir;
    apr_table_t  *directives;
    apr_table_t  *options;
    apr_hash_t   *hlists; /* hlists for every phase */
    apr_hash_t   *in_filters;
    apr_hash_t   *out_filters;
    apr_table_t  *imports;  /* for PythonImport */
} py_config;

/* register_cleanup info */
typedef struct
{
    request_rec  *request_rec;
    server_rec   *server_rec;
    PyObject     *handler;
    const char   *interpreter;
    PyObject     *data;
} cleanup_info;

/* request config structure */
typedef struct
{
    requestobject *request_obj;
    apr_hash_t    *dynhls;     /* dynamically registered handlers
                                  for this request */
    apr_hash_t   *in_filters;  /* dynamically registered input filters
                                  for this request */
    apr_hash_t   *out_filters; /* dynamically registered output filters
                                  for this request */

} py_req_config;

/* filter context */
typedef struct
{
    char *name;
    int transparent;
} python_filter_ctx;

/* a structure to hold a handler, 
   used in configuration for filters */
typedef struct
{
    char *handler;
    char *directory;
    int d_is_fnmatch;
    ap_regex_t *regex;
} py_handler;

apr_status_t python_cleanup(void *data);
PyObject* python_interpreter_name(void);
requestobject *python_get_request_object(request_rec *req, const char *phase);

#endif /* !Mp_MOD_PYTHON_H */

/*
# makes emacs go into C mode
### Local Variables:
### mode:c
### End:
*/
