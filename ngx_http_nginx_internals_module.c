#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_cycle.h>
#include <ngx_http.h>
#include "ngx_http_lua_api.h"
#include "ngx_http_lua_common.h"

#define NO_LUA_ARGS \
    if (lua_gettop(L) != 0) return luaL_error(L, "no argument expected")

static ngx_http_lua_main_conf_t *get_lua_conf() {
    ngx_http_conf_ctx_t *conf_ctx;
    int http_module_index = ngx_http_module.index;
    int lua_module_ctx_index = ngx_http_lua_module.ctx_index;

    conf_ctx = (ngx_http_conf_ctx_t *) ngx_cycle->conf_ctx[http_module_index];
    return (ngx_http_lua_main_conf_t *) conf_ctx->main_conf[lua_module_ctx_index];
}

static int ngx_http_nginx_internals_pending_timers(lua_State *L) {
    NO_LUA_ARGS;
    lua_pushnumber(L, get_lua_conf()->pending_timers);
    return 1;
}

static int ngx_http_nginx_internals_max_pending_timers(lua_State *L) {
    NO_LUA_ARGS;
    lua_pushnumber(L, get_lua_conf()->max_pending_timers);
    return 1;
}

static int ngx_http_nginx_internals_running_timers(lua_State *L) {
    NO_LUA_ARGS;
    lua_pushnumber(L, get_lua_conf()->running_timers);
    return 1;
}

static int ngx_http_nginx_internals_max_running_timers(lua_State *L) {
    NO_LUA_ARGS;
    lua_pushnumber(L, get_lua_conf()->max_running_timers);
    return 1;
}

static int ngx_http_nginx_internals_max_connections(lua_State *L) {
    NO_LUA_ARGS;
    lua_pushnumber(L, ngx_cycle->connection_n);
    return 1;
}

static int ngx_http_nginx_internals_free_connections(lua_State *L) {
    NO_LUA_ARGS;
    lua_pushnumber(L, ngx_cycle->free_connection_n);
    return 1;
}

static int ngx_http_nginx_internals_used_connections(lua_State *L) {
    NO_LUA_ARGS;
    lua_pushnumber(L, ngx_cycle->connection_n - ngx_cycle->free_connection_n);
    return 1;
}

static int ngx_http_nginx_internals_table(lua_State *L) {
    lua_newtable(L);

    lua_pushcfunction(L, ngx_http_nginx_internals_pending_timers);
    lua_setfield(L, -2, "pending_timers");

    lua_pushcfunction(L, ngx_http_nginx_internals_max_pending_timers);
    lua_setfield(L, -2, "max_pending_timers");

    lua_pushcfunction(L, ngx_http_nginx_internals_running_timers);
    lua_setfield(L, -2, "running_timers");

    lua_pushcfunction(L, ngx_http_nginx_internals_max_running_timers);
    lua_setfield(L, -2, "max_running_timers");

    lua_pushcfunction(L, ngx_http_nginx_internals_max_connections);
    lua_setfield(L, -2, "max_connections");

    lua_pushcfunction(L, ngx_http_nginx_internals_free_connections);
    lua_setfield(L, -2, "free_connections");

    lua_pushcfunction(L, ngx_http_nginx_internals_used_connections);
    lua_setfield(L, -2, "used_connections");

    return 1;
}

static ngx_int_t ngx_http_nginx_internals_init(ngx_conf_t *cf) {
    if (ngx_http_lua_add_package_preload(cf, "ngx.internals", ngx_http_nginx_internals_table) != NGX_OK) {
        return NGX_ERROR;
    }

    return NGX_OK;
}

static ngx_http_module_t ngx_http_nginx_internals_module_ctx = {
    NULL, ngx_http_nginx_internals_init, NULL, NULL, NULL, NULL, NULL, NULL
};

ngx_module_t ngx_http_nginx_internals_module = {
    NGX_MODULE_V1,
    &ngx_http_nginx_internals_module_ctx,
    NULL,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};
