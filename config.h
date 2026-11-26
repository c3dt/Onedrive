#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }

static const int sloppyfocus               = 1;
static const int bypass_surface_visibility = 0;
static const int smartgaps                 = 0;
static const int monoclegaps               = 0;

static const unsigned int borderpx         = 1;

static const unsigned int gappih           = 6;
static const unsigned int gappiv           = 6;
static const unsigned int gappoh           = 6;
static const unsigned int gappov           = 6;

static const unsigned int snap             = 32;
static const float rootcolor[]             = COLOR(0x222222ff);
static const float bordercolor[]           = COLOR(0x00000000);
static const float focuscolor[]            = COLOR(0xadadadff);
static const float urgentcolor[]           = COLOR(0xff0000ff);
static const float fullscreen_bg[]         = {0.0f, 0.0f, 0.0f, 1.0f};

#define TAGCOUNT (9)

static int log_level = WLR_ERROR;

static const char *const autostart[] = {
	"wlr-randr", "--output", "DP-1", "--mode", "1920x1080@320", NULL,
	"fcitx5", "-d", NULL,
    "swaybg", "-i", "/home/doc/walls/wall1.jpg", "-m", "fill", NULL,
	NULL
};

static const Rule rules[] = {
	{ "Gimp_EXAMPLE",     NULL,       0,            1,           -1 },
	{ "firefox_EXAMPLE",  NULL,       1 << 8,       0,           -1 },
};

static const Layout layouts[] = {
	{ "[]=",      tile },
	{ "><>",      NULL },
	{ "[M]",      monocle },
};

static const MonitorRule monrules[] = {
	{ NULL,       0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

static const struct xkb_rule_names xkb_rules = {
	.options = NULL,
};

static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT;
static const double accel_speed = 0.0;

static const int repeat_rate = 35;
static const int repeat_delay = 200;

static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;

static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, KEY,            tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,KEY,toggletag,  {.ui = 1 << TAG} }

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static const char *termcmd[] = { "alacritty", NULL };
static const char *filecmd[] = { "nemo", NULL };
static const char *menucmd[] = { "rofi", "-show", "run", NULL };
static const char *screenshotcmd[] = { "sh", "-c", "mkdir -p ~/Pictures/screenshots && grim -g \"$(slurp)\" -s 1 -t png - | tee ~/Pictures/screenshots/$(date +%Y-%m-%d_%H-%M-%S).png | wl-copy", NULL };

static const Key keys[] = {
	{ MODKEY,                    XKB_KEY_d,           spawn,            {.v = menucmd} },
	{ MODKEY,                    XKB_KEY_Return,      spawn,            {.v = termcmd} },
        { MODKEY,                    XKB_KEY_e,           spawn,            {.v = filecmd} },
        { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_S,           spawn,            {.v = screenshotcmd} },
	{ MODKEY,                    XKB_KEY_j,           focusstack,       {.i = +1} },
	{ MODKEY,                    XKB_KEY_k,           focusstack,       {.i = -1} },
	{ MODKEY,                    XKB_KEY_i,           incnmaster,       {.i = +1} },
	{ MODKEY,                    XKB_KEY_p,           incnmaster,       {.i = -1} },
	{ MODKEY,                    XKB_KEY_h,           setmfact,         {.f = -0.05f} },
	{ MODKEY,                    XKB_KEY_l,           setmfact,         {.f = +0.05f} },
	{ MODKEY,                    XKB_KEY_Return,      zoom,             {0} },
	{ MODKEY,                    XKB_KEY_Tab,         view,             {0} },
	{ MODKEY,                    XKB_KEY_q,           killclient,       {0} },
	{ MODKEY,                    XKB_KEY_t,           setlayout,        {.v = &layouts[0]} },
	{ MODKEY,                    XKB_KEY_y,           setlayout,        {.v = &layouts[1]} },
	{ MODKEY,                    XKB_KEY_u,           setlayout,        {.v = &layouts[2]} },
	{ MODKEY,                    XKB_KEY_space,       setlayout,        {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,       togglefloating,   {0} },
	{ MODKEY,                    XKB_KEY_f,           togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_0,           view,             {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_0,           tag,              {.ui = ~0} },
	{ MODKEY,                    XKB_KEY_comma,       focusmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,      focusmon,         {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_comma,       tagmon,           {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_period,      tagmon,           {.i = WLR_DIRECTION_RIGHT} },
	TAGKEYS(                     XKB_KEY_1,           0),
	TAGKEYS(                     XKB_KEY_2,           1),
	TAGKEYS(                     XKB_KEY_3,           2),
	TAGKEYS(                     XKB_KEY_4,           3),
	TAGKEYS(                     XKB_KEY_5,           4),
	TAGKEYS(                     XKB_KEY_6,           5),
	TAGKEYS(                     XKB_KEY_7,           6),
	TAGKEYS(                     XKB_KEY_8,           7),
	TAGKEYS(                     XKB_KEY_9,           8),
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_q,           quit,             {0} },

	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_BackSpace, quit, {0} },

#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_F##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};

static const Axis axes[] = {
	{ 0, 0, NULL, {0} },
};
