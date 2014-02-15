"Resource/HudLayout.res"
{
	HudHealth
	{
		"fieldName"			"HudHealth"
		"visible" 			"1"
		"enabled" 			"1"

		"xpos"			"60"
		"ypos"			"90"
		"wide"			"100"
		"tall" 			"50"		
		"digit_xpos" 		"0"
		"digit_ypos"		"0"
	}
	HudChrono
	{
		"fieldName"			"HudChrono"
		"visible" 			"1"
		"enabled" 			"1"

		"xpos"			"600"
		"ypos"			"90"
		"wide"			"100"
		"tall" 			"50"		
		"digit_xpos" 		"0"
		"digit_ypos"		"0"
		"NumberFont"		"HudChrono"

	}	
	HudChronoSeconds
	{
		"fieldName"			"HudChronoSeconds"
		"visible" 			"1"
		"enabled" 			"1"

		"xpos"			"620"
		"ypos"			"90"
		"wide"			"100"
		"tall" 			"50"		
		"digit_xpos" 		"0"
		"digit_ypos"		"0"
		"NumberFont"		"HudChrono"
	}

	HudHealthLogo
   	{
      	"fieldName" 		"HudHealthLogo"
     		"visible"			"1"
      	"enabled" 			"1"

		"xpos" 			"20"
      	"ypos" 			"35"
      	"wide" 			"128"
      	"tall" 			"128"
   	}

	HudChronoLogo
   	{
      	"fieldName" 		"HudChronoLogo"
     	"visible"			"1"
      	"enabled" 			"1"

		"xpos" 			"100"
      	"ypos" 			"20"
      	"wide" 			"256"
      	"tall" 			"128"
   	}

	HudSuit
	{
		"fieldName"			"HudSuit"		
		"visible" 			"1"
		"enabled" 			"1"

		"xpos"			"118"
		"ypos"			"40"
		"wide"			"100"
		"tall"  			"50"
		"digit_xpos" 		"0"
		"digit_ypos" 		"0"

		"TextColor"			"ColorWhite"
		"NumberFont"		"HudAmmoNumbers"

	}

	HudArmorLogo
   	{
      	"fieldName" 		"HudArmorLogo"
      	"xpos" 			"80"
      	"ypos" 			"0"
      	"wide" 			"128"
      	"tall" 			"128"
     		"visible"			"1"
      	"enabled" 			"1"
   	}
	HudDOMLogo
   	{
      	"fieldName" 		"HudArmorLogo"
      	"xpos" 			"100"
      	"ypos" 			"0"
      	"wide" 			"128"
      	"tall" 			"128"
     		"visible"			"1"
      	"enabled" 			"1"
   	}

	HudAmmoLogo
   	{
      	"fieldName" 		"HudAmmoLogo"
      	"xpos" 			"-10"
      	"ypos" 			"325"
      	"wide" 			"256"
      	"tall" 			"256"
     	"visible"			"1"
      	"enabled" 			"1"
   	}

	HudAmmo
	{
		"fieldName" 		"HudAmmo"
		"visible" 			"1"
		"enabled" 			"1"
		

		"xpos"			"40"
		"ypos"			"405"
		"wide"			"150"
		"tall"  			"150"
		"digit_xpos" 		"2"
		"digit_ypos" 		"0"
		"digit2_xpos" 		"20"
		"digit2_ypos" 		"15"
		
		"FgColor"			"ColorBlue"
		"TextColor"			"0 0 0 250"
		"TextColor2"		"0 0 0 255"
		
		"NumberFont"		"HudAmmoNumbers"
	}
	
	HudAmmoSecondary
	{
		"fieldName" 		"HudAmmoSecondary"
		"visible" 			"1"
		"enabled" 			"1"
		"xpos"			"40"
		"ypos"			"405"
		"wide"			"150"
		"tall"  			"150"
		"digit_xpos" 		"2"
		"digit_ypos" 		"0"
		"digit2_xpos" 		"20"
		"digit2_ypos" 		"15"

		"FgColor"			"ColorBlue"
		"TextColor"			"0 0 0 250"
		"TextColor2"		"0 0 0 255"
		
		"NumberFont"		"HudAmmoNumbers"
	}

	HudWeaponSelection
	{
		"fieldName" 		"HudWeaponSelection"
		"visible" 			"1"
		"enabled" 			"1"

		"NumberFont"		"HudSelectionNumbers"
		"TextFont"			"HudSelectionText"
		"NumberColor"		"ColorWhite"
		"TextColor"			"ColorWhite"
		
		"xpos"			"-8"
		"ypos"			"0"

		"BoxWidth"			"128" //Largeur d'une boîte
		"BoxHeight"			"128" //Hauteur d'une boîte
		"BoxHOffset"		"20" 	//Décalage horizontal entre les boîtes
		"BoxVOffset"		"5" 	//Décalage vertical entre les boîtes

		"SelectionNumberXPos" "4"
		"SelectionNumberYPos" "4"
		"SelectionGrowTime"	"0.4"

		"SelectionNumberXPos" 	"0"
		"SelectionNumberYPos" 	"0"
	}

	TargetID
	{
		"fieldName"			"TargetID"
		"wide"			"640"
		"tall" 			"480"
		"visible" 			"1"
		"enabled" 			"1"

		"PaintBackgroundType"	"2"
	}

	HudPosture
	{
		"fieldName" 		"HudPosture"
		"visible" 			"1"
		"PaintBackgroundType"	"2"
		"xpos"			"16"
		"ypos"			"316"
		"tall"  			"36"
		"wide"			"36"
		"font"			"WeaponIconsSmall"
		"icon_xpos"			"10"
		"icon_ypos" 		"0"
	}

	HudDamageIndicator
	{
		"fieldName" 		"HudDamageIndicator"
		"visible" 			"1"
		"enabled" 			"1"
		"DmgColorLeft" 		"255 0 0 0"
		"DmgColorRight" 		"255 0 0 0"
		
		"dmg_xpos" 			"30"
		"dmg_ypos" 			"100"
		"dmg_wide" 			"36"
		"dmg_tall1" 		"240"
		"dmg_tall2" 		"200"
	}

	HudZoom
	{
		"fieldName" 		"HudZoom"
		"visible" 			"1"
		"enabled" 			"1"

		"Circle1Radius" 		"66"
		"Circle2Radius"		"74"
		"DashGap"			"16"
		"DashHeight" 		"4"		
		"BorderThickness" 	"88"
	}

	HudCrosshair
	{
		"fieldName"  		"HudCrosshair"
		"visible" 	 		"1"
		"enabled" 	 		"1"

		"wide"	 		"640"
		"tall"	 		"480"
	}

	HudDeathNotice
	{
		"fieldName" 		"HudDeathNotice"
		"visible" 			"1"
		"enabled" 			"1"

		"wide"	 		"640"
		"tall"	 		"480"

		"TextFont"			"HudDeathNoticeFont"
		"MaxDeathNotices" 	"4"
	}

	ScorePanel
	{
		"fieldName" 		"ScorePanel"
		"visible" 			"1"
		"enabled" 			"1"

		"wide"			"640"
		"tall"	 		"480"
	}

	HudTrain
	{
		"fieldName" 		"HudTrain"
		"visible" 			"1"
		"enabled" 			"1"
		"wide"			"640"
		"tall"	 		"480"
	}

	HudMOTD
	{
		"fieldName" 		"HudMOTD"
		"visible" 			"1"
		"enabled" 			"1"
		"wide"	 		"640"
		"tall"	 		"480"
	}

	HudMessage
	{
		"fieldName" 		"HudMessage"
		"visible" 			"1"
		"enabled" 			"1"
		"wide"	 		"640"
		"tall"	 		"480"
	}

	HudMenu
	{
		"fieldName" 		"HudMenu"
		"visible" 			"1"
		"enabled" 			"1"
		"wide"	 		"640"
		"tall"	 		"480"
	}

	HudCloseCaption
	{
		"fieldName" 		"HudCloseCaption"
		"visible"			"1"
		"enabled"			"1"
		"xpos"			"c-250"
		"ypos"			"276"
	
		"wide"			"500"
		"tall"			"136"

		"BgAlpha"			"128"

		"GrowTime"			"0.25"
		"ItemHiddenTime"		"0.2"  // Nearly same as grow time so that the item doesn't start to show until growth is finished
		"ItemFadeInTime"		"0.15" // Once ItemHiddenTime is finished, takes this much longer to fade in
		"ItemFadeOutTime"		"0.3"
		"topoffset"			"0"

	}

	//HudChat
	//{
//		"fieldName" 		"HudChat"
		//"visible" 			"0"
//		"enabled" 			"1"
		//"xpos"			"0"
//		"ypos"			"0"
		//"wide"	 		"4"
//		"tall"	 		"4"
	//}
	"HudChat"
	{
		"ControlName"		"EditablePanel"
		"fieldName" 		"HudChat"
		"visible" 		"1"
		"enabled" 		"1"
		"xpos"			"10"
		"ypos"			"275"
		"wide"	 		"320"
		"tall"	 		"120"
		"PaintBackgroundType"	"2"
	}
	HudHistoryResource
	{
		"fieldName" 		"HudHistoryResource"
		"visible" 			"1"
		"enabled" 			"1"
		"xpos"			"r252"
		"ypos"			"40"
		"wide"	 		"248"
		"tall"	 		"320"

		"history_gap"		"56"
		"icon_inset"		"38"
		"text_inset"		"36"
		"text_inset"		"26"
		"NumberFont"		"HudNumbersSmall"
	}

	HUDQuickInfo
	{
		"fieldName" 		"HUDQuickInfo"
		"visible" 			"1"
		"enabled" 			"1"
		"wide"	 		"640"
		"tall"	 		"480"
	}

	HudWeapon
	{
		"fieldName" 		"HudWeapon"
		"visible" 			"1"
		"enabled" 			"1"
		"wide"	 		"640"
		"tall"	 		"480"
	}

	HudAnimationInfo
	{
		"fieldName" 		"HudAnimationInfo"
		"visible" 			"1"
		"enabled" 			"1"
		"wide"	 		"640"
		"tall"	 		"480"
	}

	HudPredictionDump
	{
		"fieldName" 		"HudPredictionDump"
		"visible" 			"1"
		"enabled" 			"1"
		"wide"	 		"640"
		"tall"	 		"480"
	}

	HudPoisonDamageIndicator
	{
		"fieldName"			"HudPoisonDamageIndicator"
		"visible"			"0"
		"enabled" 			"1"
		"xpos"			"16"
		"ypos"			"346"
		"wide"			"136"
		"tall"			"38"
		"text_xpos"			"8"
		"text_ypos"			"8"
		"text_ygap" 		"14"
		"TextColor"			"255 170 0 220"
		"PaintBackgroundType"	"2"
	}

	HudCredits
	{
		"fieldName"			"HudCredits"
		"TextFont"			"Default"
		"visible"			"1"
		"xpos"			"0"
		"ypos"			"0"
		"wide"			"640"
		"tall"			"480"
		"TextColor"			"255 255 255 192"

	}

	HudCommentary
	{
		"fieldName" 		"HudCommentary"
		"xpos"			"c-190"
		"ypos"			"350"
		"wide"			"380"
		"tall"  			"40"
		"visible" 			"1"
		"enabled" 			"1"
		
		"PaintBackgroundType"	"2"
		
		"bar_xpos"			"50"
		"bar_ypos"			"20"
		"bar_height"		"8"
		"bar_width"			"320"
		"speaker_xpos"		"50"
		"speaker_ypos"		"8"
		"count_xpos_from_right"	"10"	// Counts from the right side
		"count_ypos"		"8"
		
		"icon_texture"		"vgui/hud/icon_commentary"
		"icon_xpos"			"0"
		"icon_ypos"			"0"		
		"icon_width"		"40"
		"icon_height"		"40"
	}

	AchievementNotificationPanel	
	{
		"fieldName"			"AchievementNotificationPanel"
		"visible"			"1"
		"enabled"			"1"
		"xpos"			"0"
		"ypos"			"180"
		"wide"			"f10"
		"tall"			"100"
	}

	HudVoiceStatus
	{
		
	}

	HudVoiceSelfStatus
	{
		
	}
}
