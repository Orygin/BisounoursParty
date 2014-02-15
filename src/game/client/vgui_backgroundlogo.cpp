
 #include "cbase.h"
 using namespace vgui;
 #include <vgui/IVGui.h>
 #include <vgui_controls/Frame.h>
 #include <vgui_controls/TextEntry.h>
 #include "ienginevgui.h"


 class CBackgroundImagePanel : public vgui::Frame
 {
 	DECLARE_CLASS_SIMPLE(CBackgroundImagePanel, vgui::Frame); 

 	CBackgroundImagePanel(vgui::VPANEL parent);
 	~CBackgroundImagePanel(){};

 protected:
	 virtual void Paint();
	 vgui::ImagePanel *logo;
 };

CBackgroundImagePanel::CBackgroundImagePanel(vgui::VPANEL parent) : BaseClass(NULL, "BackgroundImage")
{
	SetParent( parent );
	SetParent( parent );
	SetTitleBarVisible( false );
	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );
	SetCloseButtonVisible( false );
	SetSizeable( false );
	SetMoveable( false );
	SetProportional( true );
	SetVisible( true );
	SetKeyBoardInputEnabled( false );
	SetMouseInputEnabled( false );

	


	//SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	
	//LoadControlSettings("resource/UI/Feedback.res");
}
void CBackgroundImagePanel::Paint()
{
	

}