#include "mem.h"
#include "private.h"
#include "utils.h"

#include "quickjs.h"

#include <string.h>

static JSValue tjs_custom_logger_info_func(JSContext *ctx, JSValue new_target, int argc, JSValue *argv) {
    TJSRuntime *qrt = TJS_GetRuntime(ctx);
    if (qrt->custom_logger_info_cb) {
        size_t size;
        uint8_t *buf = JS_GetUint8Array(ctx, &size, argv[0]);
        if (!buf)
            return JS_EXCEPTION;

        char *log_str = (char *) malloc(size + 1);
        if (!log_str) {
            return JS_EXCEPTION;
        }

        for (size_t i = 0; i < size; i++) {
            log_str[i] = buf[i];
        }
        log_str[size] = '\0';

        qrt->custom_logger_info_cb(log_str);
        free(log_str);
    }
    JSValue ret = JS_UNDEFINED;
    return TJS_NewResolvedPromise(ctx, 1, &ret);
}

void tjs__mod_ext_init(JSContext *ctx, JSValue ns) {
    JSValue custom_logger = JS_NewObject(ctx);
    JSValue info_func = JS_NewCFunction2(ctx,
                                         tjs_custom_logger_info_func,
                                         "info",
                                         1,
                                         JS_CFUNC_generic_magic,
                                         0);
    CHECK_EQ(JS_SetPropertyStr(ctx, custom_logger, "info", info_func), true);
    CHECK_EQ(JS_SetPropertyStr(ctx, ns, "CustomLogger", custom_logger), true);

    // JSValue global_obj = JS_GetGlobalObject(ctx);
    //JSValue custom_logger = JS_NewObjectProto(ctx, JS_NULL);

    //CHECK_EQ(JS_DefinePropertyValueStr(ctx, ns, "CustomLogger", custom_logger, JS_PROP_C_W_E), true);
    //obj = JS_NewCFunction2(ctx, tjs_custom_logger_info_func, "info", 1, JS_CFUNC_constructor, 0);
    //CHECK_EQ(JS_DefinePropertyValueStr(ctx, custom_logger, "info", obj, JS_PROP_C_W_E), true);
}
