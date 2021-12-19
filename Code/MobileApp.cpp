#include <mosquitto.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include ""

using namespace std;

struct Device {

    int DeviceID = 0;
    int energy = 0;
    bool status = false;

};

class MobileApp {

public:
    Device devices[10];
    Device* device = devices;

    bool Turn_ON_Device(int deviceID, mosquitto * mosq_pub)
    {
        mosquitto_publish(mosq_pub, NULL, "test/D1", 3, "ON", 0, false);
        struct mosquitto_message* msg;
        mosquitto_subscribe_simple(&msg, 1, false, "test/App", 0, "localhost", 1883, NULL, 10, true, NULL, NULL, NULL, NULL);
        char* msg1 = (char*)msg->payload;
        if (msg1 == "ON")
        {
            cout << "Device Turned On" << endl;
            return true;
        }
        else
        {
            cout << "Device couldn't be turned ON" << endl;
            return false;
        }
            

        
    }

    bool Turn_OFF_Device(int deviceID, mosquitto* mosq_pub)
    {
        mosquitto_publish(mosq_pub, NULL, "test/D1", 3, "OFF", 0, false);
        struct mosquitto_message* msg;
        mosquitto_subscribe_simple(&msg, 1, false, "test/App", 0, "localhost", 1883, NULL, 10, true, NULL, NULL, NULL, NULL);
        char* msg1 = (char*)msg->payload;
        if (msg1 == "OFF")
        {
            cout << "Device Turned Off" << endl;
            return true;
        }
        else
        {
            cout << "Device couldn't be turned Off" << endl;
            return false;
        }
    }

    char* get_energy_consumption(int deviceID, mosquitto* mosq_pub)
    {
        mosquitto_publish(mosq_pub, NULL, "test/D1", 7, "Power", 0, false);
        struct mosquitto_message* msg;
        mosquitto_subscribe_simple(&msg, 1, false, "test/App", 0, "localhost", 1883, NULL, 10, true, NULL, NULL, NULL, NULL);
        char* power = (char*)msg->payload;
        cout << "Power Consumption is :" << power << endl;
        return power;

    }

private:

};


int main()
{
    MobileApp App;
    App.devices[0].DeviceID = 1;    


    /*Publisher Setup*/
    int rc_pub;
    struct mosquitto * mosq_pub;

    mosquitto_lib_init();

    mosq_pub = mosquitto_new("publisher-app", true, NULL);
    rc_pub = mosquitto_connect(mosq_pub, "localhost", 1883, 60);
    if (rc_pub != 0) {
        printf("Client could not connect to broker! Error Code: %d\n", rc_pub);
        mosquitto_destroy(mosq_pub);
        return -1;
    }
    printf("We are now connected to the broker!\n");



    string in;

    while (1)
    {
        std::cout << "Enter Commands(ON/OFF/Power/Exit):" << endl;
        cin >> in;
        if (in == "ON" || in == "On" || in == "on")
        {
           //if (App.Turn_ON_Device(1, mosq_pub))
             //  App.devices[0].status = true;

            mosquitto_publish(mosq_pub, NULL, "test/D1", 3, "ON", 0, false);
            struct mosquitto_message* msg;
            mosquitto_subscribe_simple(&msg, 1, false, "test/App", 0, "localhost", 1883, NULL, 10, true, NULL, NULL, NULL, NULL);
            char* msg1 = (char*)msg->payload;
            if (msg1 == "ON")
            {
                cout << "Device Turned On" << endl;
                return true;
            }
            else
            {
                cout << "Device couldn't be turned ON" << endl;
                return false;
            }
            
            
        }
        else if (in == "OFF" || in == "Off" || in == "off")
        {
            if (App.Turn_OFF_Device(1, mosq_pub))
                App.devices[0].status = false;
        }
        else if (in == "Power" || in == "POWER" || in == "power")
        {
            char* power;
            power = App.get_energy_consumption(1, mosq_pub);
            App.devices[0].energy = int(power);
        }
        else if (in == "Exit")
        {
            cout << "Exiting APP" << endl;
            break;
        }
        else
        {
            cout << "Wrong commands! Enter again" << endl;
        }
    }

    //mosquitto_publish(mosq, NULL, "test/t1", 6, "Hello", 0, false);

    mosquitto_disconnect(mosq_pub);
    mosquitto_destroy(mosq_pub);



    mosquitto_lib_cleanup();
    return 0;
    
}