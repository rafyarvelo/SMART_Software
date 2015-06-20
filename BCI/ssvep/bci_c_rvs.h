#ifndef BCI_C_RVS
#define BCI_C_RVS

typedef enum
{
    LED_FORWARD,
    LED_BACKWARD,
    LED_RIGHT,
    LED_LEFT,
    NUM_LED_GROUPS
} LED_Group_ID;

typedef struct LED_Group
{
    float        frequency;
    LED_Group_ID id;

    LED_Group(LED_Group_ID id, float frequency)
    {
        this->id        = id;
        this->frequency = frequency;
    }
} LED_Group;

class C_RVS
{
public:
     LED_Group();
    ~LED_Group();

    //id < 0 indicates all groups
    void emitRVS(LED_Group_ID id=-1);
    void SetFrequency(LED_Group_ID id, float frequency);

private:
    LED_Group ledGroups[NUM_LED_GROUPS];

};

#endif // BCI_C_RVS

