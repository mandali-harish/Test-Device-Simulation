#include <mosquitto.h>
#include<iostream>
#include <ctime>
#include<cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

class Device {

public:
    Device(int id)
    {
        DeviceID = id;
    }


    void set_status(bool status_)
    {
        status = status_;


    }

    bool get_status()
    {
        return status;

    }

    int get_energy()
    {
        device_run();
        return energy;
    }
private:
    int DeviceID;
    bool status = false;
    int energy = 0;
    void device_run()
    {
        if (status)
        {
            srand((unsigned)time(0));
            int randomNumber;
            randomNumber = (rand() % 50) + 1;
            energy = randomNumber;
        }
        else
            energy = 0;

    }


};


void on_connect(struct mosquitto* mosq, void* obj, int rc) {
    printf("ID: %d\n", *(int*)obj);
    if (rc) {
        printf("Error with result code: %d\n", rc);
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "test/D1", 0);
}

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {
    printf("New message with topic %s: %s\n", msg->topic, (char*)msg->payload);

   /*
   if (msg->topic == "test/D1")
    {
        char* msg1 = (char*)msg->payload;

        if (msg1 == "ON" || msg1 == "On" || msg1 == "on")
        {
            
        }
    }
    */
    
}
int main()
{
    Device d1(1);

    //Publisher
    int rc_pub;
    struct mosquitto* mosq_pubd;

    mosquitto_lib_init();

    mosq_pubd = mosquitto_new("publisher-device", true, NULL);
    rc_pub = mosquitto_connect(mosq_pubd, "localhost", 1883, 60);
    if (rc_pub != 0) {
        printf("Client could not connect to broker! Error Code: %d\n", rc_pub);
        mosquitto_destroy(mosq_pubd);
        return -1;
    }
    printf("Device Publisher connected to the broker!\n");

    //Subscriber
    int rc, id = 12;

    mosquitto_lib_init();

    struct mosquitto* mosq_subd;

    mosq_subd = mosquitto_new("subscribe-device", true, &id);
    mosquitto_connect_callback_set(mosq_subd, on_connect);
    mosquitto_message_callback_set(mosq_subd, on_message);

    rc = mosquitto_connect(mosq_subd, "localhost", 1883, 60);
    if (rc) {
        printf("Could not connect to Broker with return code %d\n", rc);
        return -1;
    }
    
    mosquitto_loop_start(mosq_subd);
    printf("Press Enter to quit...\n");
    getchar();
    mosquitto_loop_stop(mosq_subd, true);

    mosquitto_disconnect(mosq_pubd);
    mosquitto_destroy(mosq_pubd);
    mosquitto_disconnect(mosq_subd);
    mosquitto_destroy(mosq_subd);
    mosquitto_lib_cleanup();

    
    return 0;
}