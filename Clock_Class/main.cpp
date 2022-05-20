#include <iostream>


class Clock_t
{
private:
    int year;
    int month;
    int day;

    int dayow;

    int hour;
    int minute;
    int second;

public:

    Clock_t()
    {
        year = 0;
        month = 0;
        day = 0;

        dayow = 0;

        hour = 0;
        minute = 0;
        second = 0;

    }

    Clock_t(int h, int m, int s, int dw = 5, int dd = 20, int mm = 5, int yy = 22)
    {
        year = yy;
        month = mm;
        day = dd;

        dayow = dw;

        hour = h;
        minute = m;
        second = s;

    }

    void show()
    {
        std::cout << hour << ":" << minute << ":" << second << std::endl;
        std::cout << dayow << std::endl;
        std::cout << day << ". " << month << ". " << year << std::endl;

    }


};


int main()
{
    Clock_t clock(10, 42, 0);
    std::cout << "Hello world!" << std::endl;

    clock.show();
    return 0;
}
