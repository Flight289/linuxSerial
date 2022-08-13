#include <stdio.h>
#include <string>
#include <cstring>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

int serialJob(double& value1, double& value2, double& value3, double& value4)
{

    int_fast32_t fd = open("/dev/ttyACM0", O_RDWR, 777);
    if(fd < 0)
    {
        printf("could not open serial port.\n");
        return -1;
    }


    char start_buf{};
    uint_fast32_t available_size{};

    struct serial_struct serial_setting;
    ioctl(fd, TIOCGSERIAL, &serial_setting);
    serial_setting.flags |= ASYNCB_SPD_VHI;
    serial_setting.flags |= ASYNC_LOW_LATENCY;
    ioctl(fd, TIOCSSERIAL, &serial_setting);

    ioctl(fd, FIONREAD, &available_size);
    if(available_size > 0)read(fd, &start_buf, 1);
    if(start_buf == ' ')
    {
        std::string read_buf{};
        char end_buf{};
        while(true)
        {
            read(fd, &end_buf, 1);
            if(end_buf == '@')
            {
                break;
            }
            else
            {
                read_buf.push_back(end_buf);
            }
        }


        char read_value1[255] = "";
        char read_value2[255] = "";
        char read_value3[255] = "";
        char read_value4[255] = "";
        uint_fast8_t num{};
        uint_fast8_t count{};

        for(char x : read_buf)
        {

            if(x == ',')
            {
                num++;
                count = 0;
                continue;
            }
            switch(num)
            {
                case 0:
                    read_value1[count] = x;
                break;

                case 1:
                     read_value2[count] = x;
                break;

                case 2:
                     read_value3[count] = x;
                break;

                case 3:
                     read_value4[count] = x;
                break;

            }
            count++;
        }

        value1 = atof(read_value1);
        value2 = atof(read_value2);
        value3 = atof(read_value3);
        value4 = atof(read_value4);
        read_buf.clear();

    }
}
