#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER amf

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./amf-tp.hpp"

#if !defined(_AMF_TP_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _AMF_TP_H

#include <lttng/tracepoint.h>

#if 0
#define TP_TYPE(x)	TRACEPOINT_EVENT(	\
    			amf,			\
    			x,			\
    			TP_ARGS(		\
        			const char*, log_string	\
    				),			\
    			TP_FIELDS(		\
        			ctf_string(log, log_string)	\
    				)			\
			)

TP_TYPE(warn)
TP_TYPE(info)
TP_TYPE(error)
TP_TYPE(trace)
TP_TYPE(debug)
TP_TYPE(startup)
#endif
TRACEPOINT_EVENT(amf,
    		warn,
    		TP_ARGS(
        		const char*, log_string	
    			),
    		TP_FIELDS(
        		ctf_string(log, log_string)
    			)
		)

TRACEPOINT_EVENT(amf,
    		info,
    		TP_ARGS(
        		const char*, log_string	
    			),
    		TP_FIELDS(
        		ctf_string(log, log_string)
    			)
		)

TRACEPOINT_EVENT(amf,
    		error,
    		TP_ARGS(
        		const char*, log_string	
    			),
    		TP_FIELDS(
        		ctf_string(log, log_string)
    			)
		)
TRACEPOINT_EVENT(amf,
    		trace,
    		TP_ARGS(
        		const char*, log_string	
    			),
    		TP_FIELDS(
        		ctf_string(log, log_string)
    			)
		)
TRACEPOINT_EVENT(amf,
    		debug,
    		TP_ARGS(
        		const char*, log_string	
    			),
    		TP_FIELDS(
        		ctf_string(log, log_string)
    			)
		)
TRACEPOINT_EVENT(amf,
    		startup,
    		TP_ARGS(
        		const char*, log_string	
    			),
    		TP_FIELDS(
        		ctf_string(log, log_string)
    			)
		)

#endif /* _PROXY_TP_H */

#include <lttng/tracepoint-event.h>
