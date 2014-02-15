 #include <vgui/IVGui.h>
 #include <vgui_controls/Frame.h>

class IPrivacyPanel
{
public:
	virtual void		Create( vgui::VPANEL parent ) = 0;
	virtual void		Destroy( void ) = 0;
	virtual void		Activate( void ) = 0;
};
 
extern IPrivacyPanel* Privacypanel;