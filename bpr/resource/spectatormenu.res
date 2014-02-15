"spectatormenu.res"
{
	"menuitem1"
	{
		"label"		"#Valve_Close"
		"command"	"spec_menu 0"
	}
	"menuitem2"
	{
		"label"		"#Valve_Settings"
		"menuitem21"
		{
			"label"		"#Valve_Chat_Messages"
			"toggle"	"hud_saytext"
		}
		"menuitem22"
		{
			"label"		"#Valve_Show_Status"
			"toggle"	"spec_drawstatus"
		}
		"menuitem23"
		{
			"label"		"#Valve_Overview_Locked"
			"toggle"	"overview_locked"
		}
		"menuitem24"
		{
			"label"		"#Valve_Overview_Names"
			"toggle"	"overview_names"
		}
		"menuitem25"
		{
			"label"		"#Valve_Overview_Health"
			"toggle"	"overview_health"
		}
		"menuitem26"
		{
			"label"		"#Valve_Overview_Tracks"
			"toggle"	"overview_tracks"
		}
	}
	"menuitem3"
	{
		"label"		"#Valve_Overview"
		"menuitem31"
		{
			"label"		"#Valve_Overview_Off"
			"command"	"overview_mode 0"
		}
		"menuitem32"
		{
			"label"		"#Valve_Overview_Small"
			"command"	"overview_mode 1"
		}
		"menuitem33"
		{
			"label"		"#Valve_Overview_Large"
			"command"	"overview_mode 2"
		}
		"menuitem34"
		{
			"label"		"#Valve_Overview_ZoomIn"
			"command"	"overview_zoom 1.1 0.1 rel"
		}
		"menuitem35"
		{
			"label"		"#Valve_Overview_ZoomOut"
			"command"	"overview_zoom 0.9 0.1 rel"
		}
	}
	"menuitem4"
	{
		"label"		"#Valve_Auto_Director"
		"toggle"	"spec_autodirector"
	}
	"menuitem5"
	{
		"label"		"#Valve_Show_Scores"
		"command"	"togglescores"
	}
}
