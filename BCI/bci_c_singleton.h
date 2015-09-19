#ifndef BCI_C_SINGLETON_H
#define BCI_C_SINGLETON_H

//Implement the Singleton Design Pattern to Ensure only one of each
//of our main classes is instantiated during any given run
template <typename T>
class C_Singleton
{
public:
    //Create a Single Instance of a class if none exists already
    static T* Instance()
    {
        static T* ptr = 0;

        if (!ptr)
        {
            ptr = new T;
        }
        return ptr;
    }
};

#endif // BCI_C_SINGLETON_H
