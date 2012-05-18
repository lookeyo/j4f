/*
 * Copyright (C) 2012 Zork
 */

#include "ScriptPCH.h"

enum Encounter
{
	BOSS_ZORK
};

enum Spells
{
    SPELL_STRIKE				= 109211,
    SPELL_FRENZY				= 81173,
};

enum Events
{
    EVENT_NONE,
    EVENT_STRIKE,
    EVENT_FRENZY,
};

class boss_zork : public CreatureScript
{
public:
    boss_zork() : CreatureScript("boss_zork") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_zorkAI (creature);
    }

    struct boss_zorkAI : public BossAI
    {
		boss_zorkAI(Creature* creature) : BossAI(creature, BOSS_ZORK) {}

        bool enraged;

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();
            enraged = false;
            events.ScheduleEvent(EVENT_FRENZY, 16000);
            events.ScheduleEvent(EVENT_STRIKE, 20000);

        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || !CheckInRoom())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FRENZY:
						DoCast(me, SPELL_FRENZY, true);
                        events.ScheduleEvent(EVENT_FRENZY, 16000);
                        break;
                    case EVENT_STRIKE:
						DoCastVictim(SPELL_STRIKE);
                        events.ScheduleEvent(EVENT_STRIKE, 20000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_zork()
{
    new boss_zork();
}