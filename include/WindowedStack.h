#ifndef WindowedStack_h
#define WindowedStack_h

class WindowedStack
{
    public:
        explicit WindowedStack(const int size);
        void push(const float value);
        float* getData();
        int count();
        float average();
        int windowSize();
        ~WindowedStack();
    private:
        int _n, _i, _count;
        float* a;
};

#endif