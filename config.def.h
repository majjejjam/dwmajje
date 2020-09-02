/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 5;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]			= {"Siji:style=Regular:size=12" ,
									"scientifica:style=Medium:size=14" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";

static const char onedark1[]		= "#ABB2BF";
static const char onedark2[]		= "#282C34";
static const char onedark3[]		= "#ABB2BF";
static const char onedark4[]		= "#5C6370";
static const char onedark5[]		= "#4B5263";
static const char onedark6[]		= "#E06C75";
static const char onedark7[]		= "#BE5046";
static const char onedark8[]		= "#9CB38C";
static const char onedark9[]		= "#ACCF93";
static const char onedark10[]		= "#E5C07B";
static const char onedark11[]		= "#D19A66";
static const char onedark12[]		= "#61AFEF";
static const char onedark13[]		= "#C678DD";
static const char onedark14[]		= "#56B6C2";
static const char onedark15[]		= "#ABB2BF";
static const char onedark16[]		= "#3E4452";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]	= { onedark4,	onedark2,	onedark16 },
	[SchemeSel]		= { onedark1,	onedark2,	onedark15 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol	arrange		function */
	{ "",		tile },		/* first entry is default */
	{ "",		NULL },		/* no layout function means floating behavior */
	{ "",		monocle },
};

/* this implements <alt-Tab> for dwm, put it in config.h  */
static int alt_tab_count = 0;

/* focus and restack a client */
static void
focus_restack(Client *c)
   { if (c) { focus(c); restack(selmon); } }

static void
start_alt_tab(const Arg *arg)
   { alt_tab_count = 0; }

static
Client *next_visible(Client *c)
{
   for(/* DO_NOTHING */; c && !ISVISIBLE(c); c=c->snext);
   return c;
}

static int
count_visible(void)
{
   int count = 0;
   for (Client *c=next_visible(selmon->stack); c; c = next_visible(c->snext))
		count += 1;
   return count;
}

static
Client *get_nth_client(int n)
{
   Client *c;
   for (c=next_visible(selmon->stack); c && n--; c = next_visible(c->snext));
   return c;
}

static void
alt_tab(const Arg *arg)
{
	if (!selmon->sel)
		return;
   // put all of the windows back in their original focus/stack position */
   for (int i=0; i<alt_tab_count; i+=1)
		focus_restack(get_nth_client(alt_tab_count));

   // focus and restack the nth window */
   alt_tab_count = (alt_tab_count + 1) % count_visible();
   focus_restack(get_nth_client(alt_tab_count));
}

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_r,      spawn,          SHCMD("st -e n") },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_n,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_n,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_Escape, view,           {0} },
	{ MODKEY,                       XK_w,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_i,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ 0,							XK_Super_L,	start_alt_tab,		{0} },
	{ MODKEY,						XK_Tab,		alt_tab,			{0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      spawn,          SHCMD("st -e 'killall xinit'") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

