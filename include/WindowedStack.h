#ifndef WindowedStack_h
#define WindowedStack_h

class WindowedStack
{
    public:
        WindowedStack(int size);
        void push(float value);
        float* getData();
        int Count();
        int WindowSize();
        ~WindowedStack();
    private:
        int _n, _i, _count;
        float* a;
};

#endif