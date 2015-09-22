
#ifndef BCI_C_RVS
#define BCI_C_RVS

#include "../smart_config.h"
#include "../smart_debug_log.h"

typedef enum
{
    LED_FORWARD=0,
    LED_BACKWARD,
    LED_RIGHT,
    LED_LEFT,
    NUM_LED_GROUPS
} LED_Group_ID;

#define	LED_FORWARD_FREQ_DEFAULT  10
#define	LED_BACKWARD_FREQ_DEFAULT 20
#define	LED_RIGHT_FREQ_DEFAULT    30
#define	LED_LEFT_FREQ_DEFAULT     40

class LED_Group
{
public:
    LED_Group(LED_Group_ID id, unsigned short frequency)
    {
        this->id        = id;
        this->frequency = frequency;
    }
	
	//Factory Constructors
	static LED_Group* Forward()
	{
		return createLED_Group(LED_FORWARD, LED_FORWARD_FREQ_DEFAULT);
	}
	
	static LED_Group* Backward()
	{
		return createLED_Group(LED_BACKWARD, LED_BACKWARD_FREQ_DEFAULT);
	}
	
	static LED_Group* Right()
	{
		return createLED_Group(LED_RIGHT, LED_RIGHT_FREQ_DEFAULT);
	}
	
	static LED_Group* Left()
	{
		return createLED_Group(LED_LEFT, LED_LEFT_FREQ_DEFAULT);
	}
	
    static LED_Group* createLED_Group(LED_Group_ID id, unsigned short freq)
	{
		return new LED_Group(id, freq);
	}

	static LED_Group** DefaultGroups()
	{
		int i = 0;
		LED_Group** defaultGroups = new LED_Group*[NUM_LED_GROUPS];
		
		defaultGroups[i++] = Forward();
		defaultGroups[i++] = Backward();
		defaultGroups[i++] = Right();
		defaultGroups[i++] = Left();
		
		return defaultGroups;
	}
	
    unsigned short frequency;
    LED_Group_ID   id;
};

class C_RVS
{
public:
      C_RVS();
     ~C_RVS();

	static C_RVS* Instance(){ return new C_RVS; }
    LED_Group*  GetLEDGroup(LED_Group_ID id);
    LED_Group** GetAllLEDGroups();
    void SetFrequency(LED_Group_ID id, unsigned short frequency);

    void Generate(); //Creates RVS frequencies

protected:
	SMART_DEBUG_LOG* debugLog;
    LED_Group**      ledGroups;

};

class LED_Group_Fwd : public LED_Group
{
public:
    LED_Group_Fwd()
        :LED_Group(LED_FORWARD, LED_FORWARD_FREQ_DEFAULT)
    {
    }
};

class LED_Group_Bwd : public LED_Group
{
public:
    LED_Group_Bwd()
        :LED_Group(LED_BACKWARD, LED_BACKWARD_FREQ_DEFAULT)
    {
    }
};

class LED_Group_Rgt : public LED_Group
{
public:
    LED_Group_Rgt()
        : LED_Group(LED_RIGHT, LED_RIGHT_FREQ_DEFAULT)
    {

    }
};

class LED_Group_Lft : public LED_Group
{
public:
    LED_Group_Lft()
        :LED_Group(LED_LEFT, LED_LEFT_FREQ_DEFAULT)
    {
    }
};
#endif // BCI_C_RVS

