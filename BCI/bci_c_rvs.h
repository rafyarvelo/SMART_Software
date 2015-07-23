
#ifndef BCI_C_RVS
#define BCI_C_RVS

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
		return new LED_Group(LED_FORWARD, LED_FORWARD_FREQ_DEFAULT);
	}
	
	static LED_Group* Backward()
	{
		return new LED_Group(LED_BACKWARD, LED_BACKWARD_FREQ_DEFAULT);
	}
	
	static LED_Group* Right()
	{
		return new LED_Group(LED_RIGHT, LED_RIGHT_FREQ_DEFAULT);
	}
	
	static LED_Group* Left()
	{
		return new LED_Group(LED_LEFT, LED_LEFT_FREQ_DEFAULT);
	}

	static LED_Group** DefaultGroups()
	{
		LED_Group* defaultGroups[] = 
		{
			Forward(),
			Backward(),
			Right(),
			Left()
		};
		
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

    void emitRVS(LED_Group_ID id);
    void SetFrequency(LED_Group_ID id, float frequency);

private:
    LED_Group* ledGroups[NUM_LED_GROUPS];

};

#endif // BCI_C_RVS

