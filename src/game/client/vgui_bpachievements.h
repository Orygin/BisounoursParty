 #include <vgui/IVGui.h>
 #include <vgui_controls/Frame.h>
 #include "Cloud\bp_achievements.h"

class IAchievPanel
{
public:
	virtual void		Create( vgui::VPANEL parent ) = 0;
	virtual void		Destroy( void ) = 0;
	virtual void		Activate( void ) = 0;
	virtual void		AddStruct(Achievement_struct structure[64], int max) = 0;
};
 
extern IAchievPanel* achievpanel;