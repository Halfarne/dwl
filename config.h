#include "fibonacci.c"

/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int monoclegaps               = 0;
static const unsigned int borderpx         = 2;  /* border pixel of windows */

static const int smartgaps                 = 0;
static const unsigned int gappih           = 10; /* horiz inner gap between windows */
static const unsigned int gappiv           = 10; /* vert inner gap between windows */
static const unsigned int gappoh           = 10; 
static const unsigned int gappov           = 10;

static const float rootcolor[]             = {0.45, 0.88, 0.95, 1.0};
static const float bordercolor[]           = {0.45, 0.88, 0.95, 1.0};
static const float focuscolor[]            = {0.1, 0.6, 0.65, 1.0};
/* To conform the xdg-protocol, set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1, 0.1, 0.1, 1.0};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* app_id     title       tags mask     isfloating   monitor */
	/* examples:
	{ "Gimp",     NULL,       0,            1,           -1 },
	*/
	{ "firefox",  NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[D]",      dwindle },
	{ "[M]",      monocle },    
	{ "[F]",      NULL }, /* means floating*/
};

/* monitors */
static const MonitorRule monrules[] = {
	/* name       mfact nmaster scale layout       rotate/reflect */
	/* example of a HiDPI laptop monitor:
	{ "eDP-1",    0.5,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL },
	*/
	/* defaults */
	{ NULL,       0.55, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.layout = "cz",
	.options = NULL,
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE       
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS       
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER 
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;
/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *term[] = { "kitty", NULL };
static const char *browser[] = { "firefox", NULL };

static const char *menu[] = { "rofi", "-show", "drun", NULL };
static const char *power[] = { "rofi", "-show", "power-menu", "-modi", "power-menu:/~/.config/rofi/rofi-power-menu", NULL };
static const char *help[] = { "rofi", "-show", "Pomoc", "-modi", "Pomoc:/~/.config/rofi/rofi-help-tab", NULL };

static const char *prnscr[] = { "/bin/sh", "-c", "grim -g \"$(slurp)\"", NULL};

static const char *volp[] = { "pamixer", "-i", "5", NULL };
static const char *volm[] = { "pamixer", "-d", "5", NULL };
static const char *tmic[] = { "pamixer", "--source", "alsa_input.pci-0000_0a_00.3.analog-stereo", "-t", NULL };


static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                  key                 function        argument */
	{ MODKEY,                    XKB_KEY_space,	 spawn,          {.v = menu} },
	{ MODKEY,		     XKB_KEY_a,		 spawn,          {.v = term} },
	{ MODKEY,		     XKB_KEY_f,		 spawn,          {.v = browser} },
	{ MODKEY,		     XKB_KEY_s,		 spawn,          {.v = power} },
	{ MODKEY,		     XKB_KEY_h,		 spawn,          {.v = help} },
	{ MODKEY,		     XKB_KEY_p,		 spawn,          {.v = prnscr} },

	{ MODKEY,		     XKB_KEY_k,          killclient,     {0} },

	{ MODKEY|WLR_MODIFIER_ALT,   XKB_KEY_t,		 setlayout,      {.v = &layouts[0]} },
	{ MODKEY|WLR_MODIFIER_ALT,   XKB_KEY_f,		 setlayout,      {.v = &layouts[1]} },
	{ MODKEY|WLR_MODIFIER_ALT,   XKB_KEY_m,		 setlayout,      {.v = &layouts[2]} },

	{ MODKEY,		     XKB_KEY_v,		 togglefloating, {0} },
	{ MODKEY,                    XKB_KEY_l,          togglefullscreen, {0} },

	{ MODKEY|WLR_MODIFIER_ALT,   XKB_KEY_q,          quit,           {0} },

	TAGKEYS(          XKB_KEY_q, XKB_KEY_Q,				  0),
	TAGKEYS(          XKB_KEY_w, XKB_KEY_W,				  1),
	TAGKEYS(          XKB_KEY_e, XKB_KEY_E,				  2),
	TAGKEYS(          XKB_KEY_r, XKB_KEY_R,				  3),
	TAGKEYS(          XKB_KEY_t, XKB_KEY_T,				  4),

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,	moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE,	togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,	moveresize,     {.ui = CurResize} },

	/*{ MODKEY, BTN_SIDE,	moveresize,     {.ui = CurResize} },
	{ MODKEY, BTN_EXTRA,	moveresize,     {.ui = CurResize} },*/

	{ 0, BTN_MIDDLE,	spawn,		{.v = tmic} },
	{ 0, BTN_SIDE,		spawn,		{.v = volm} },
	{ 0, BTN_EXTRA,		spawn,		{.v = volp} },
};
