#ifndef TRIGGER_VOIDFALL_H
#define TRIGGER_VOIDFALL_H

class CTriggerVoidfall : public CTriggerMultiple
{
public:
	DECLARE_CLASS( CTriggerVoidfall, CTriggerMultiple );
	DECLARE_DATADESC();
	//DECLARE_SERVERCLASS();

	void Activate(void);
	void FallTouch(CBaseEntity *pEntity);
private:
	COutputEvent m_OnFall;
};

LINK_ENTITY_TO_CLASS( trigger_voidfall, CTriggerVoidfall );
#endif